#include "Wifi_S08_v2.h"

#define SSID "6s08" // Put network SSID here
#define PASSWORD "iesc6s08" // Put password here or "" if no password

ESP8266 wifi = ESP8266(0, true);
String HOST = "iesc-s2.mit.edu";
int PORT = 80;
String PATH = "/hello.html";

unsigned long IOT_INTERVAL = 8000; // Send request every 8 sec
elapsedMillis time_since_request = IOT_INTERVAL; // Start timer at 8 sec

void setup() {
  wifi.begin();
  wifi.connectWifi(SSID, PASSWORD);
  while (!wifi.isConnected()); //wait for connection
}

void loop() {
	if (wifi.hasResponse()) { // Check for response from the previous request
      String resp = wifi.getResponse();
      Serial.print("Got response at t=");
      Serial.println(millis());
      Serial.println(resp);		
	}

	// Send out a request if 8 seconds has elapsed since last request
	if (time_since_request > IOT_INTERVAL && !wifi.isBusy()) {
    Serial.print("Sending request at t=");
    Serial.println(millis());
    
    String params = "";  // Format is param1=value1&param2=value2
    wifi.sendRequest(GET, HOST, PORT, PATH, params);
    time_since_request = 0;
	}
} // No delays or while loops in loop()!
