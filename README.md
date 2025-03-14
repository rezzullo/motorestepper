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








