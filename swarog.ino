/*
  Swarog - IoT lamp and sunrise alarm.
  Jaroslaw Surkont
*/

#define NDEBUG

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <string.h>

#include "Device.h"

#define PIN_POWER 12
#define PIN_BRIGHTEN 13
#define PIN_DIM 14

// WiFi configuration
const char* ssid = "";
const char* password = "";

// MQTT server URL or IP address
const char* mqtt_server = "";
// Topic listened by all devices, e.g., /devices
const char* mqtt_topic_common = "";
// Topic listened only by this device, e.g., /devices/58b49208ee7827a3
const char* mqtt_topic_device = "";
// Topic where the device logs its activity, e.g., /devices/58b49208ee7827a3/log
const char* mqtt_topic_log = "";
// Topic where the device writes its help message,
//  e.g., /devices/58b49208ee7827a3/help
const char* mqtt_topic_help = "";

// NOTE: 128 bytes is the default maximum message size in the MQTT library,
//  it can be changed in the library header file (MQTT_MAX_PACKET_SIZE in
//  PubSubClient.h)
char msg[64];
char msg_log[8];
const char* msg_help =
"name: Swarog\n"
"description: IoT lamp and sunrise alarm.\n"
"actions:\n"
"- name: on\n"
"  description: Turn the light on\n"
"- name: off\n"
"  description: Turn the light off\n"
"- name: brighten\n"
"  description: Brighten the light\n"
"- name: dim\n"
"  description: Dim the light\n"
"- name: rise\n"
"  description: Gradually increase light brightness (full brightness in 10 minutes)\n";

Button buttonPower;
Button buttonBrighten;
Button buttonDim;
Lamp lamp;

WiFiClient clientWifi;
PubSubClient client(clientWifi);

#ifndef NDEBUG
int lastLampState = LAMP_BRIGHTNESS_LOW;
#endif

void connectWifi() {
  delay(10);
  #ifndef NDEBUG
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  #endif
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    #ifndef NDEBUG
    Serial.print(".");
    #endif
  }
  randomSeed(micros());
  #ifndef NDEBUG
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  #endif
}

void reconnectMqtt() {
  while (!client.connected()) {
    #ifndef NDEBUG
    Serial.print("Attempting MQTT connection...");
    #endif
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      #ifndef NDEBUG
      Serial.println("connected");
      #endif
      client.publish(mqtt_topic_help, msg_help);
      client.subscribe(mqtt_topic_common);
      client.subscribe(mqtt_topic_device);
    } else {
      #ifndef NDEBUG
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      #endif
      delay(5000);
    }
  }
}

void callback(char* topic, uint8_t* payload, unsigned int length) {
  if (strcmp(topic, mqtt_topic_common) == 0) {
    for (unsigned int i = 0; i < length; ++i) msg[i] = (char) payload[i];
    msg[length] = '\0';
    if (strcmp(msg, "detect") == 0) client.publish(mqtt_topic_help, msg_help);
  } else if (strcmp(topic, mqtt_topic_device) == 0) {
    for (unsigned int i = 0; i < length; ++i) msg[i] = (char) payload[i];
    msg[length] = '\0';
    if (strcmp(msg, "on") == 0) lamp.on();
    else if (strcmp(msg, "off") == 0) lamp.off();
    else if (strcmp(msg, "brighten") == 0) lamp.brighten();
    else if (strcmp(msg, "dim") == 0) lamp.dim();
    else if (strcmp(msg, "rise") == 0) lamp.rise();
    sprintf(msg_log, "%d", lamp.getState());
    client.publish(mqtt_topic_log, msg_log);
  }
}

void setup() {
  Serial.begin(9600);

  buttonPower = Button(PIN_POWER);
  buttonBrighten = Button(PIN_BRIGHTEN);
  buttonDim = Button(PIN_DIM);
  lamp = Lamp();
  lamp.off();

  connectWifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) reconnectMqtt();
  client.loop();

  if (buttonPower.isPressed()) {
    if (lamp.isOff()) {
      lamp.on();
      sprintf(msg_log, "%d", lamp.getState());
      client.publish(mqtt_topic_log, msg_log);
    } else {
      lamp.off();
      sprintf(msg_log, "%d", lamp.getState());
      client.publish(mqtt_topic_log, msg_log);
    }
  }
  if (buttonBrighten.isPressed()) {
    lamp.brighten();
    sprintf(msg_log, "%d", lamp.getState());
    client.publish(mqtt_topic_log, msg_log);
  }
  if (buttonDim.isPressed()) {
    lamp.dim();
    sprintf(msg_log, "%d", lamp.getState());
    client.publish(mqtt_topic_log, msg_log);
  }

  if (lamp.isRising()) lamp.riseUpdate();

  #ifndef NDEBUG
  if (lamp.getState() != lastLampState) {
    lastLampState = lamp.getState();
    Serial.print("Current state: ");
    Serial.println(lastLampState);
  }
  #endif
}
