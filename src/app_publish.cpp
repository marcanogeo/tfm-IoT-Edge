#include "app_publish.h"
#include <Arduino.h>
#include <PubSubClient.h>
#include "time.h"
#include "DHTesp.h"
#include "config.h"
#include "node_metadata.h"
#include "telemetry_builder.h"
#include "topics.h"
#include "mq2_sensor.h"

extern PubSubClient client;
extern DHTesp dhtSensor;
extern MQ2Sensor mq2;

void publishTelemetry() {
  if (!client.connected()) {
    Serial.println("MQTT no conectado");
    return;
  }

  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  if (isnan(data.temperature) || isnan(data.humidity)) {
    Serial.println("Error leyendo DHT22");
    return;
  }

  int lightRaw = analogRead(LDR_PIN);
  MQ2Reading gas = mq2.read();
  unsigned long ts = time(nullptr);

  String payload = buildTelemetryPayload(
    DEVICE_ID,
    ZONE_ID,
    ts,
    data.temperature,
    data.humidity,
    lightRaw,
    gas
  );

  //client.publish("test", "hi");
  //client.publish(topicTelemetry().c_str(), "{\"ok\":1");
  bool ok = client.publish(topicTelemetry().c_str(), payload.c_str());
  //bool ok = client.publish("test", "{\"ok\":1}");

  Serial.println("----- TELEMETRÍA -----");
  Serial.println(payload);
  Serial.println(ok ? "MQTT OK" : "MQTT ERROR");
}