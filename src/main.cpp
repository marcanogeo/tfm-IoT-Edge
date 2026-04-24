#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "DHTesp.h"

#include "config.h"
#include "mqtt_config.h"
#include "node_metadata.h"
#include "topics.h"
#include "mq2_sensor.h"
#include "app_config.h"
#include "app_publish.h"
#include "app_loop.h"

WiFiClient espClient;
PubSubClient client(espClient);
DHTesp dhtSensor;
MQ2Sensor mq2(MQ2_ANALOG_PIN, MQ2_DIGITAL_PIN, MQ2_VCC, MQ2_RL, MQ2_R0);

unsigned long lastPublish = 0;

void mqttCallback(char* topic, byte* message, unsigned int length) {
  Serial.print("Mensaje recibido [");
  Serial.print(topic);
  Serial.print("]: ");

  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
  }
  Serial.println();
}

void reconnectMQTT() {
  while (!client.connected()) {
   // Serial.print("Conectando MQTT...");
   // String clientId = "ESP32_" + String(DEVICE_ID) + "_" + String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println(" conectado");
      client.subscribe(topicCmd().c_str());
      client.publish(topicStatus().c_str(), "online", true);
    } else {
      Serial.print(" fallo rc=");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(SERIAL_BAUDRATE);
  delay(500);

  randomSeed(esp_random());
  analogReadResolution(ADC_RESOLUTION_BITS);

  Serial.println("Nodo: " + String(DEVICE_ID));
  Serial.println("Zona: " + String(ZONE_ID));

  connectWiFi();
  initTime();
  printLocalTime();

  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  mq2.begin();

  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(mqttCallback);
  client.setBufferSize(1024); // Aumentar el tamaño del buffer para mensajes más grandes
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  if (!client.connected()) {
    reconnectMQTT();
  }

  client.loop();
  runMainLoop();
}