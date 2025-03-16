## English documentation

### StepperMotor mDNS
This project consists of a web page opened via mDNS method to control the steps and operation of a stepper motor connected to the esp32. 

#### Part mDNS and WiFi
`WiFi.begin(ssid, password);`
- Start connecting to the WiFi network using the SSID (network name) and password defined previously in the code.

`WiFi.status() != WL_CONNECTED`
- Check if the connection was successful

`(!MDNS.begin("name"))`
- Allows you to assign a local domain name to the ESP32, avoiding having to type the IP every time

`server.send_P(200, "text/html", index_html);`
- 200 → HTTP code for "OK".
- "text/html" → indicates that the content is a web page.
- index_html → is the HTML code that will be displayed, it's the name with which the char parameter is indicated

`server.begin();` 
- Start the http server

## Documentazione in italiano

### StepperMotor mDNS
Questo progetto consiste in una pagina web aperta tramite il metodo mDNS per controllare i passi e il funzionamento di un motore stepper connesso a un esp32.

#### Librerie
`<WiFi.h>`
- Libreria per gestire la connessione WiFi

`<WebServer.h>`
- Libreria per creare un server web con l'ESP32

`<ESPmDNS.h>`
- Libreria per la connessione tramite metodo mDNS

#### Configurazione WiFi
`const char* ssid = "your_ssid";` --- `const char* password = "your_password";`
Le due variabili in cui vanno sostituite le credenziali del tuo WiFi

#### Creazione del server web
`WebServer server(80);`
Crea un server web sulla porta 80, cioè la porta standard per l'HTTP

#### Definizione dei pin
`#define INX n`  
Definizione dei pin collegati all'ESP32, gli INX corrispondono agli ingressi dell'L298, il ponte-H.
Da modificare in base al proprio circuito e alle proprie preferenze

#### Definizione dei metodi di funzionamento
`const int metod[4][4]={{A,B,C,D}}`
Ogni sequenza è formata da una matrice 4x4 dove ogni riga indica una sequenza e ogni colonna indica una bobina, 1 indica la bobina da cui entra la corrente, 0 indica la corrente da cui esce. Le sequenze sono commentate nel codice.

#### Interfaccia web
L'interfaccia web è definita all'interno del codice come una stringa memorizzata in PROGMEM per ottimizzare l'uso della RAM dell'ESP32. La pagina web è contenuta all'interno della variabile `index_html` definita con `const char index_html[] PROGMEM = R"rawliteral(...)`, la raw string literal permette di scrivere stringhe multi-linea senza dover usare caratteri di escape (\n, \", etc.).

`<!DOCTYPE html>`
- Definisce che il documento segue lo standard HTML5.

`<html lang="it">`
- Specifica che la lingua della pagina è l'italiano.

`<meta charset="UTF-8">`
- Imposta la codifica dei caratteri necessaria per supportare lettere accentate e simboli speciali.

`<meta name="viewport" content="width=device-width, initial-scale=1.0">`
- Permette la corretta visualizzazione su dispositivi mobili.

`<title>Controllo Motore Stepper</title>`
- Definisce il titolo della pagina.








#### Spiegazioni mDNS e WiFi
`WiFi.begin(ssid, password);`
- Inizia a connettersi alla rete wifi utilizzando il nome e la passowrd della rete.

`WiFi.status() != WL_CONNECTED`
- Controlla se la connessione è avvenuta con successo

`(!MDNS.begin("name"))`
- Ti permette di assegnare un nome che verra assegnato alla pagina web, viene stampato nel monitor seriale e va incollato nella barra di ricerca per aprire la pagina web, al posto dell'indirizzo IP

`server.send_P(200, "text/html", index_html);`
- 200 → codice HTTP che indica lo stato "ok"
- "text/html" → indica che il contenuto della pagina è di tipo web.
- index_html → è il nome con cui no inizializziamo la variabile char per scrivere la parte di html

`server.begin();` 
- avvia il server HTTP








