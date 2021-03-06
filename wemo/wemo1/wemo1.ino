#include <ESP8266WiFi.h>
#include "fauxmoESP.h"
 
#define WIFI_SSID "oleg-24"
#define WIFI_PASS "mierda01"

#define SERIAL_BAUDRATE                 115200
 
fauxmoESP fauxmo;
 
void wifiSetup() {
    // Set WIFI module to STA mode
    WiFi.mode(WIFI_STA);
 
    // Connect
    Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
 
    // Wait
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
 
    // Connected!
    Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
}
 
void callback(uint8_t device_id, const char * device_name, bool state) {
  Serial.print("Device "); Serial.print(device_name); 
  Serial.print(" state: ");
  if (state) {
    Serial.println("ON");
  } else {
    Serial.println("OFF");
  }
}
 
void setup() {
    // Init serial port and clean garbage
    Serial.begin(SERIAL_BAUDRATE);
    Serial.println("FauxMo demo sketch");
    Serial.println("After connection, ask Alexa/Echo to 'turn <devicename> on' or 'off'");
 
    // Wifi
    wifiSetup();
 
    // Fauxmo
    fauxmo.addDevice("relay");
    fauxmo.addDevice("pixels");
    fauxmo.onMessage(callback);
}
 
void loop() {
  fauxmo.handle();
}
