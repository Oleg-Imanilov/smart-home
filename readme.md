# This is my hobby "Smart Home" project. 

## How it works

![System diagram](https://github.com/Oleg-Imanilov/smart-home/blob/master/images/system-draft.png "System diagram")

Each IOT device may be configured by IOT configurator (Also esp8266 device). When you installing or moving one of IOT devices across your house, you just need to start it in "install" mode when IOT configurator is on. Devices will automatically connect and configuration will be copied from configurator into IOT device flash memory. 

Configuration contains:
* WiFi SSID / WiFi Password 
* MQTT IP
* Device Name (to recognize it in management)
* Location ( for example: home/2ndfloor/kids-room ) - Location will be used as MQTT topic prefix


## System components
1. Management Web Application
2. IOT devices
3. MQTT broker 
4. NodeMcu IOT Configurator

### 1. Management Web Application

Manage and monitor your house from web.

### 2. IOT devices

Any type of IOT devices which may subscribe for MQTT topics or trigger/push messages to MQTT broker.

### 3. MQTT broker 

Lightweight MQTT service. Running on raspberry PI.

### 4. NodeMcu IOT Configurator

This is autonomus device to configure other IOT devices. It includes small oled display where user can see configuration. When it is on any IOT may connect to it and get this configuration.
![IOT Configurator](https://github.com/Oleg-Imanilov/smart-home/blob/master/images/iot-configurator-device.png "IOT Configurator")

Here is example of screen:

![IOT Configurator](https://github.com/Oleg-Imanilov/smart-home/blob/master/images/iot-configurator-proto.png "IOT Configurator")


