#include <ESP8266WiFi.h>

#include <ESP8266WebServer.h>
#include <PubSubClient.h>

#define LIGHT  A0
#define MOTION D1

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  pinMode(MOTION, INPUT);
  
  WiFi.begin("oleg-24", "mierda01");
  Serial.print("Connecting to WiFi ");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  
  client.setServer("192.168.0.188", 1883);
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
}

char _topic[128];

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    String clientId = String("master-bedroom");
        
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe("command/master-bedroom");
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

  int light = analogRead(LIGHT);  
  int motion = digitalRead(MOTION);
  
  if(abs(light - oldLight)>10) {
    Serial.print("Light: ");
    Serial.println(light);
    oldLight = light;
    sprintf (msg, "%i", light);
    client.publish("sensor/master-bedroom/light", msg);
  }
  
  if(motion != oldMotion) {
    if(motion==HIGH) {
      motionStart = millis();
      client.publish("sensor/master-bedroom/motion", "1");
    } else {
      client.publish("sensor/master-bedroom/motion", "0");
    }
    oldMotion = motion;
  }
  delay(500);
}


