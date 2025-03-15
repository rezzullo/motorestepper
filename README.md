## English documentation
### StepperMotor mDNS
This project consists of a web page opened via mDNS method to control the steps and operation of a stepper motor connected to the esp32. 
#### Part mDNS and WiFi
WiFi.begin(ssid, password);
Start connecting to the WiFi network using the SSID (network name) and password defined previously in the code.

WiFi.status() != WL_CONNECTED
check if the connection was successful

(!MDNS.begin("name"))
allows you to assign a local domain name to the ESP32, avoiding having to type the IP every time

server.send_P(200, "text/html", index_html);
200 → HTTP code for "OK".
"text/html" → indicates that the content is a web page.
index_html → is the HTML code that will be displayed, it's the name with which the char parameter is indicated

server.begin(); 
start the http server







