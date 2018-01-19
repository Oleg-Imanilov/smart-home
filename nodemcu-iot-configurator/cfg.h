#ifndef __CFG_H__
#define __CFG_H__

#include <FS.h>

#include "strutils.h"

class CFG {
private:
  String name = "";
  String wifi = "";
  String wifiPass = "";
  String mqttIp = "";
  bool _spiffOn = false;
  bool _read = false;
  bool spiffOn();
public:
  CFG();
  bool read();
  bool write();

  String json();
  String getName();
  String getWifi();
  String getWifiPass();
  String getMqttIp();
  
  void setName(String s);
  void setWifi(String w, String p);
  void setMqttIp(String ip);
};

CFG::CFG() {
}

bool CFG::spiffOn() {
  if(!_spiffOn) {
    _spiffOn = SPIFFS.begin();
  }  
  return _spiffOn;
}

String CFG::getName() {
  return name;  
}

String CFG::getWifi(){
  return wifi;
}
String CFG::getWifiPass(){
  return wifiPass;
}
String CFG::getMqttIp(){
  return mqttIp;
}

void CFG::setName(String s){
  name = s;
}

void CFG::setWifi(String w, String p){
  wifi = w;
  wifiPass = p;
}

void CFG::setMqttIp(String ip){
  mqttIp = ip;
}

bool CFG::read() {
  if(_read) {
    return true;
  }
  if(!spiffOn()) {
    return false;
  }
  File f = SPIFFS.open("/config", "r");
  if (!f) {
    return false;
  }
  wifi = f.readStringUntil('\n');
  wifi.trim();
  wifiPass = f.readStringUntil('\n');
  wifiPass.trim();
  mqttIp = f.readStringUntil('\n');
  mqttIp.trim();
  name = f.readStringUntil('\n');
  name.trim();
  f.close();
  _read = true;
  return true;
}

bool CFG::write() {
  if(!spiffOn()) {
    return false;
  }
  File f = SPIFFS.open("/config", "w");
  if (!f) {
    return false;
  }
  f.println(wifi); 
  f.println(wifiPass); 
  f.println(mqttIp);
  f.println(name);  
  f.close();
  return true;
}

String CFG::json() {
  return String("{") 
    + nv("wifi", wifi) 
    + nv("wifiPass", wifiPass) 
    + nv("mqttIp", mqttIp)
    + nv("name", name, false)    
    + String("}");
}

#endif
