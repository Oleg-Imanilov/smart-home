#include <ESP8266WiFi.h>

#include <ESP8266WebServer.h>
#include <PubSubClient.h>

#include "cfg.h"
#include "mac.h"

CFG cfg;
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  pinMode(D1, INPUT);
  pinMode(D2, OUTPUT);  
  digitalWrite(D2, LOW);

  cfg.read();
  Serial.println("Config");
  Serial.println(cfg.json());
  
  WiFi.begin(cfg.getWifi(), cfg.getWifiPass());
  Serial.print("Connecting to WiFi ");
  Serial.println(cfg.getWifi());
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  
  Serial.print("Connecting to MQTT-IP:[");
  Serial.print(cfg.getMqttIp());
  Serial.println("]");  
  
  Serial.print("Location:[");
  Serial.print(cfg.getLocation());
  Serial.println("]");  

  
  client.setServer(cfg.getMqttIp(), 1883);
  client.setCallback(callback);  
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  String p((char *)payload);
  if(p.startsWith("light:on")){
    digitalWrite(D2, HIGH);
  } else if(p.startsWith("light:off")){
    digitalWrite(D2, LOW);
  }
}


char _topic[128];

char *topic(char *sensor) {
  sprintf(_topic, "%s/%s", cfg.getLocation(), sensor);
  return _topic;
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    String clientId = String(cfg.getLocation()) + "-" + cfg.getName() + "-";
    clientId += String(random(0xffff), HEX);
        
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe(cfg.getLocation());
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 second");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

int oldMotion = LOW;
int oldLight = 0;
int motionStart = 0;

char msg[128];

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  int light = analogRead(A0);  
  int motion = digitalRead(D1);
  

  if(abs(light - oldLight)>10) {
    Serial.print("Light: ");
    Serial.println(light);
    oldLight = light;
    sprintf (msg, "{\"light\": %i}", light);
    client.publish(topic("light"), msg);
  }
  
  if(motion != oldMotion) {
    if(motion==HIGH) {
      motionStart = millis();
      sprintf (msg, "{\"motion\": %i}", light);
      client.publish(topic("motion"), "{\"motion\":\"true\"}");
      Serial.println("Motion started");
    }
    oldMotion = motion;
  }
  delay(500);
}


