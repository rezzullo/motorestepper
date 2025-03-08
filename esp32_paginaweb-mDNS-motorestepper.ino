#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

// Credenziali WiFi
const char* ssid = "iPhone di rezzullo";
const char* password = "patrizia";

// Server Web
WebServer server(80);

// Definizione pin del motore stepper
#define IN1 12
#define IN2 14
#define IN3 27
#define IN4 26

// Sequenze per i diversi metodi di funzionamento
const int waveDrive[4][4] = {
  {1, 0, 0, 0}, //ab
  {0, 0, 1, 0}, //cd
  {0, 1, 0, 0}, //ba
  {0, 0, 0, 1} //dc
};

const int fullStep[4][4] = {
  {1, 0, 1, 0}, //ab cd  
  {0, 1, 1, 0}, //ba cd
  {0, 1, 0, 1}, //ba dc
  {1, 0, 0, 1}  //ab dc
};

const int halfStep[8][4] = {
  {1, 0, 0, 0}, //ab
  {1, 0, 1, 0}, //ab cd
  {0, 0, 1, 0}, //cd
  {0, 1, 1, 0}, //ba cd
  {0, 0, 1, 0}, //ba
  {0, 0, 1, 1}, //ba dc
  {0, 0, 0, 1}, //dc
  {1, 0, 0, 1}  //ab dc
};

// Pagina Web HTML
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Controllo Stepper</title>
  <script>
    function invia() {
      var passi = document.getElementById("passi").value;
      var metodo = document.getElementById("metodo").value;
      fetch("/stepper?passi=" + passi + "&metodo=" + metodo);
    }
  </script>
</head>
<body>
  <h2>Controllo Motore Stepper</h2>
  <label>Numero di passi:</label>
  <input type="number" id="passi" value="100">
  
  <label>Metodo di funzionamento:</label>
  <select id="metodo">
    <option value="wave">Wave Drive</option>
    <option value="full">Full Step</option>
    <option value="half">Half Step</option>
  </select>
  <button onclick="invia()">Invia</button>
</body>
</html>
)rawliteral";

// Funzione per muovere lo stepper
void moveStepper(int passi, String metodo) {
  int delayTime = 20;  // Velocità del motore
  int stepCount;
  const int (*sequence)[4]; // Puntatore alla sequenza corretta

  // Selezione della modalità di funzionamento
  if (metodo == "wave") {
    sequence = waveDrive;
    stepCount = 4;
  } else if (metodo == "full") {
    sequence = fullStep;
    stepCount = 4;
  } else {
    sequence = halfStep;
    stepCount = 8;
  }

  // Movimento del motore
  for (int i = 0; i < passi; i++) {
    int stepIndex = i % stepCount;
    digitalWrite(IN1, sequence[stepIndex][0]);
    digitalWrite(IN2, sequence[stepIndex][1]);
    digitalWrite(IN3, sequence[stepIndex][2]);
    digitalWrite(IN4, sequence[stepIndex][3]);
    delay(delayTime);
  }

  // Spegne i pin per evitare surriscaldamento
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// Gestione richiesta HTTP per lo stepper
void handleStepper() {
  if (server.hasArg("passi") && server.hasArg("metodo")) {
    int passi = server.arg("passi").toInt();
    String metodo = server.arg("metodo");
    moveStepper(passi, metodo);
    Serial.print(passi);
    Serial.println("");
  }
  server.send(200, "text/plain", "Motore in movimento!");
}

void setup() {
  Serial.begin(115200);

  // Configura i pin del motore
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Connessione WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connessione a WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnesso al WiFi!");
  Serial.print("Indirizzo IP: ");
  Serial.println(WiFi.localIP());

  // Configura mDNS (Nome rete locale: esp32.local)
  if (!MDNS.begin("esp32")) {
    Serial.println("Errore avvio mDNS");
    return;
  }
  Serial.println("mDNS avviato: http://esp32.local");

  // Configura il server web
  server.on("/", []() {
    server.send_P(200, "text/html", index_html);
  });

  server.on("/stepper", handleStepper);

  server.begin();
  Serial.println("Server Web avviato!");
}

void loop() {
  server.handleClient();
}
