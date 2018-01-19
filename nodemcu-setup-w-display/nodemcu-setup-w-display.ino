// oled 128x64
// SDA -> D2
// SCL -> D1

// Rotary Encoder
// DT  -> D5
// Clk -> D6
// SW  -> D7

// D5 -> Cap -> Gnd
// D6 -> Cap -> Gnd
// D7 -> R220 -> Vcc


#include <ESP8266WebServer.h>

#include "SSD1306.h"
#include "keyboard1306.h"
#include "user_interface.h"

#include "cfg.h"

const char *ssid = "IOT-SETUP";
const char *password = "12345678";

CFG cfg;
ESP8266WebServer server(80);

os_timer_t myTimer;

int diff = 0;
bool click = false;

int oldP1 = HIGH;
int oldP2 = HIGH;
void checkRotaryEncoder() {
  int p1 = digitalRead(D5);
  int p2 = digitalRead(D6);
  if (p1 != oldP1 || p2 != oldP2) {
    if (oldP1 == 0 && oldP2 == 0) {
      if (p1 == 0 && p2 == 1) {
        diff += 1;
      } else if (p1 == 1 && p2 == 0) {
        diff -= 1;
      }
    }
    oldP1 = p1;
    oldP2 = p2;
  }
}

int oldBtn = HIGH;
void checkButton() {
  int btn = digitalRead(D7);
  if (oldBtn == HIGH && btn == LOW) {
    click = true;
  }
  oldBtn = btn;
}

void timerCallback(void *pArg) {
  checkButton();
  checkRotaryEncoder();
}

void initTimer() {
  os_timer_setfn(&myTimer, timerCallback, NULL);
  os_timer_arm(&myTimer, 2, true);
}

// D2 -> SDA
// D1 -> SCL
SSD1306 display(0x3C, D2, D1);

Keyboard1306 *kkk;

const char *menu[] = {"Wifi", "Wifi Password", "MQTT IP", "Name"};
String values[] = {"oleg-24", "password", "192.168.0.191", "IOT-1"};
int menuSel = 0;

void drawMenu() {
  int t = millis();

  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  for (int y = 0; y < 4; y++) {
    display.drawString(63, y * 15 + 2, menu[y]);
  }
  display.drawRect(0, menuSel * 15, 128, 15);
  display.display();
  t = millis() - t;
  Serial.print("draw menu:");
  Serial.println(t);
}

void drawKeyboard() {
  int t = millis();
  kkk->draw();
  t = millis() - t;
  Serial.print("draw keyboard:");
  Serial.println(t);
}

void handleRoot() {
  server.send(200, "text/html", cfg.json());
}

IPAddress    apIP(10, 10, 10, 10);

void setup() {
  Serial.begin(115200);
  pinMode(D7, INPUT);
  Serial.println();
  Serial.println();

  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

  kkk = new Keyboard1306(&display);
  initTimer();
  drawMenu();

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid, password);

  IPAddress apip = WiFi.softAPIP();
  Serial.print("visit: \n");
  Serial.println(apip);
  server.on("/", handleRoot);
  server.begin();

  bool isOk = cfg.read();
  if (isOk) {
    values[0] = cfg.getWifi();
    values[1] = cfg.getWifiPass();
    values[2] = cfg.getMqttIp();
    values[3] = cfg.getName();
  }
}

#define MENU  1
#define KEYBOARD 2
int mode = MENU;

void loop() {
  server.handleClient();
  if (diff != 0) {
    if (mode == MENU) {
      menuSel += diff;
      if (menuSel < 0) {
        menuSel = 3;
      }
      if (menuSel > 3) {
        menuSel = 0;
      }
      drawMenu();
    } else {
      kkk->rotate(diff);
      drawKeyboard();
    }
    diff = 0;
  } else if (click) {
    if (mode == MENU) {
      kkk->setValue(values[menuSel]);
      mode = KEYBOARD;
      drawKeyboard();
    } else {
      if (!kkk->click()) {
        values[menuSel] = kkk->getValue();

        cfg.setWifi(values[0], values[1]);
        cfg.setMqttIp(values[2]);
        cfg.setName(values[3]);
        
        if (cfg.write()) {
          Serial.println("Cfg saved!");
        } else {
          Serial.println("Cfg failed to save!");
        }
        mode = MENU;
        drawMenu();
      } else {
        drawKeyboard();
      }
    }
    click = false;
  }
}
