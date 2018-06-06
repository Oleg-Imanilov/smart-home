#ifndef __MAC_H__
#define __MAC_H__

#include <ESP8266WiFi.h>
 
byte MAC[6];
String S_MAC = "";

String IP = "";

String getIp() {
  if(IP.length()==0) {
    IP = WiFi.localIP().toString();
  }
  return IP;
}

byte *getMac() {
  WiFi.macAddress(MAC);
  return MAC;
}
 
String getSMac() {
  getMac();
  if(S_MAC.length() == 0) {
    S_MAC = String(MAC[0], HEX) + ":" +
       String(MAC[1], HEX) + ":" +
       String(MAC[2], HEX) + ":" +
       String(MAC[3], HEX) + ":" +
       String(MAC[4], HEX) + ":" +
       String(MAC[5], HEX);
  }
  return S_MAC;
}

#endif
