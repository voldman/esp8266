#include "Wifi_S08_v2.h"

#define SSID "6s08" // Put network SSID here
#define PASSWORD "iesc6s08" // Put password here or "" if no password

ESP8266 wifi = ESP8266(0, true);
String HOST = "iesc-s2.mit.edu";
int PORT = 80;
String PATH = "/hello.html";

void setup() {
  wifi.begin();
  wifi.connectWifi(SSID, PASSWORD);
  while (!wifi.isConnected()); //wait for connection
}

void loop() {
  if (wifi.isConnected() && !wifi.isBusy()) {
    Serial.print("Sending request at t=");
    Serial.println(millis());
    
    String params = "";  // Format is param1=value1&param2=value2
    wifi.sendRequest(GET, HOST, PORT, PATH, params);
    
    elapsedMillis t = 0;
    while (!wifi.hasResponse() && t < 10000); // Wait up to 10s for response
    if (wifi.hasResponse()) {
      String resp = wifi.getResponse();
      Serial.print("Got response at t=");
      Serial.println(millis());
      Serial.println(resp);
    } else {
      Serial.println("No timely response");
    }
  }
  delay(5000);
}
