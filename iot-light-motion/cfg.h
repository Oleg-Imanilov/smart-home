#ifndef __CFG_H__
#define __CFG_H__

#include <FS.h>

#include "strutils.h"

char buffer[128];

class CFG {
  private:
    char* wifi = NULL;
    char* wifiPass = NULL;
    char* mqttIp = NULL;
    char* name = NULL;
    char* location = NULL;
    char *_json = NULL;
    
    bool _spiffOn = false;
    bool spiffOn();
    char * readLine(File f);
  public:
    CFG();
    // return success 
    bool read();
    // return success     
    bool write();

    char* json();

    char* getName();
    char* getWifi();
    char* getWifiPass();
    char* getMqttIp();
    char* getLocation();

    void setName(char* s);
    void setWifi(char* w, char* p);
    void setMqttIp(char* ip);
    void setLocation(char* loc);
};

CFG::CFG() {
}

bool CFG::spiffOn() {
  if (!_spiffOn) {
    _spiffOn = SPIFFS.begin();
  }
  return _spiffOn;
}

char* CFG::getName() {
  return name;
}

char* CFG::getWifi() {
  return wifi;
}
char* CFG::getWifiPass() {
  return wifiPass;
}

char* CFG::getMqttIp() {
  return mqttIp;
}

char* CFG::getLocation() {
  return location;
}

void CFG::setName(char* s) {
  name = s;
}

void CFG::setWifi(char* w, char* p) {
  wifi = w;
  wifiPass = p;
}

void CFG::setMqttIp(char* ip) {
  mqttIp = ip;
}

void  CFG::setLocation(char* loc) {
  location = loc;
}

char * CFG::readLine(File f) {
  String str = f.readStringUntil('\n');
  str.trim();
  return s2ch(str);
}

bool CFG::read() {
  if (!spiffOn()) {
    return false;
  }
  File f = SPIFFS.open("/config", "r");
  if (!f) {
    return false;
  }

  if (wifi) delete wifi;
  if (wifiPass) delete wifiPass;
  if (mqttIp) delete mqttIp;
  if (name) delete name;
  if (location) delete location;

  wifi = readLine(f);
  wifiPass = readLine(f);
  mqttIp = readLine(f);
  name = readLine(f);
  location = readLine(f);

  f.close();
  return true;
}

bool CFG::write() {
  if (!spiffOn()) {
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
  f.println(location);
  f.close();
  return true;
}

char* CFG::json() {
  String tmp = String("{")
               + nv("wifi", wifi)
               + nv("wifiPass", wifiPass)
               + nv("mqttIp", mqttIp)
               + nv("name", name)
               + nv("location", location, false)
               + String("}");
  if(_json) delete _json;
  _json = s2ch(tmp);
  return _json;
}

#endif
