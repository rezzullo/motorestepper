/*
This project is made for Esp32 microcontroller board using Arduino IDE
*/

#include <WiFi.h>         //libraries
#include <WebServer.h>
#include <ESPmDNS.h>

const char* ssid = "your_ssid";           //access to WiFi
const char* password = "your_password";

WebServer server(80);

#define IN1 12    //motor pin definition, changeable based  on pins used 
#define IN2 14
#define IN3 27
#define IN4 26

const int waveDrive[4][4] = {   //waveDrive
  {1, 0, 0, 0}, //ab
  {0, 0, 1, 0}, //cd
  {0, 1, 0, 0}, //ba
  {0, 0, 0, 1} //dc
};

const int fullStep[4][4] = {   //fullStepDrive
  {1, 0, 1, 0}, //ab cd  
  {0, 1, 1, 0}, //ba cd
  {0, 1, 0, 1}, //ba dc
  {1, 0, 0, 1}  //ab dc
};

const int halfStep[8][4] = {   //halfStepDrive
  {1, 0, 0, 0}, //ab
  {1, 0, 1, 0}, //ab cd
  {0, 0, 1, 0}, //cd
  {0, 1, 1, 0}, //ba cd
  {0, 0, 1, 0}, //ba
  {0, 0, 1, 1}, //ba dc
  {0, 0, 0, 1}, //dc
  {1, 0, 0, 1}  //ab dc
};

// HTML web page editable
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="it">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Controllo Motore Stepper</title>
  <style>
    body {
      background: linear-gradient(to bottom, #1e3c72, #2a5298);
      font-family: Arial, sans-serif;
      color: white;
      margin: 0;
      padding: 0;
      display: flex;
      justify-content: center;
      align-items: center;
      height: 100vh;
    }

    .container {
      background-color: rgba(255, 255, 255, 0.2);
      border-radius: 15px;
      padding: 40px;
      text-align: center;
      box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
      width: 400px;
    }

    h2 {
      font-size: 2em;
      margin-bottom: 20px;
      color: #a7c7e7;
    }

    label {
      display: block;
      margin: 10px 0;
      font-size: 1.1em;
      color: #d3e4f5;
    }

    input[type="number"], select {
      width: 100%;
      padding: 10px;
      border: 2px solid #3f70a4;
      border-radius: 8px;
      background-color: #ffffff;
      color: #333;
      font-size: 1em;
      margin: 10px 0;
      box-sizing: border-box;
      text-align: center; 
    }

    input[type="number"]:focus, select:focus {
      border-color: #5bc0de;
      outline: none;
    }

    select {
      text-align: center; 
    }

    button {
      padding: 12px 20px;
      background-color: #3f70a4;
      color: white;
      font-size: 1.2em;
      border: none;
      border-radius: 8px;
      cursor: pointer;
      width: 100%;
      transition: background-color 0.3s ease;
    }

    button:hover {
      background-color: #5bc0de;
    }

    button:active {
      background-color: #2a5298;
    }

    .input-container {
      margin-bottom: 20px;
    }
  </style>
  <script>
    function invia() {
      var passi = document.getElementById("passi").value;
      var metodo = document.getElementById("metodo").value;
      window.location.href = window.location.pathname + "?passi=" + passi + "&metodo=" + metodo;
    }
  </script>
</head>
<body>
  <div class="container">
    <h2>Controllo Motore Stepper</h2>

    <div class="input-container">
      <label for="passi">Numero di passi:</label>
      <input type="number" id="passi" value="100">
    </div>

    <div class="input-container">
      <label for="metodo">Metodo di funzionamento:</label>
      <select id="metodo">
        <option value="wave">Wave Drive</option>
        <option value="full">Full Step</option>
        <option value="half">Half Step</option>
      </select>
    </div>

    <button onclick="invia()">Invia</button>
  </div>
</body>
</html>
)rawliteral";

void moveStepper(int passi, String metodo) {       // function to move stepper
  int delayTime = 20;   // speed of the motor, editable, the higher the delay, the slower the motor
  int stepCount;
  const int (*sequence)[4]; // pointer to the drive

  // selection of the drive
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

  // movement of the motor
  for (int i = 0; i < passi; i++) {
    int stepIndex = i % stepCount;
    digitalWrite(IN1, sequence[stepIndex][0]);
    digitalWrite(IN2, sequence[stepIndex][1]);
    digitalWrite(IN3, sequence[stepIndex][2]);
    digitalWrite(IN4, sequence[stepIndex][3]);
    delay(delayTime);
  }

  // the pins are turned off to avoid overheating
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// HTTP management
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

  // pins configuration
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // WiFi connection
  WiFi.begin(ssid, password);
  Serial.print("Connessione a WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnesso al WiFi!"); // print ip address
  Serial.print("Indirizzo IP: ");
  Serial.println(WiFi.localIP());

  // mDNS configuration (example name: esp32.local)
  if (!MDNS.begin("StepperMotor")) {
    Serial.println("Errore avvio mDNS");
    return;
  }
  Serial.println("mDNS avviato: http://StepperMotor.local");

  // web server configuration
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
