#include <Arduino.h>
#include "DHTesp.h"
#include <WiFi.h>
#include "time.h"
#include <PubSubClient.h>

#define MQ2_DIGITAL_PIN 14 //Pin where the MQ2 sensor is connected
const int DHT_PIN = 27; // Pin where the DHT22 sensor is connected
#define MQ2_ANALOG_PIN 35 //Pin for analog reading of MQ2 sensor (if needed)

//Setting WiFi credentials for Wokwi simulation
const char* SSID = "Wokwi-GUEST";
const char* psswd = "";

//Public MQTT Broker settings
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

#ifndef NODE_NUM
#define NODE_NUM 1
#endif

#if NODE_NUM == 1
  const char* NODE_ID = "nodo1";
  const char* ZONE_ID = "zona_norte";
#elif NODE_NUM == 2
  const char* NODE_ID = "nodo2";
  const char* ZONE_ID = "zona_sur";
#elif NODE_NUM == 3
  const char* NODE_ID = "nodo3";
  const char* ZONE_ID = "zona_este";
#elif NODE_NUM == 4
  const char* NODE_ID = "nodo4";
  const char* ZONE_ID = "zona_oeste";
#else
  const char* NODE_ID = "nodoX";
  const char* ZONE_ID = "zonaX";
#endif      

String mqtt_topic_data;
String mqtt_topic_status;
String mqtt_topic_cmd;
String mqtt_topic_alerts;

// Configuración NTP
const char* ntpServer = "pool.ntp.org"; // NTP server
const long  gmtOffset_sec = 3600;       // Time offset in seconds (Spain mainland: UTC+1)
const int   daylightOffset_sec = 3600;  // Daylight saving time: +1 hour

WiFiClient espClient;
PubSubClient client(espClient);
DHTesp dhtSensor; // Create DHT sensor object

unsigned long lastPublish = 0;
const unsigned long publishInterval = 5000; // Publish every 5 seconds

//int sensorValue; // variable to store the sensor value
//int gasValue = 0; // Variable to store the gas sensor value
int gasDetected = digitalRead(MQ2_DIGITAL_PIN); // Initialize gasValue with the current state of the MQ2 sensor
int rawValue = analogRead(MQ2_ANALOG_PIN); // Read the raw analog value from the MQ2 sensor

 // WiFi connection function
void connectWiFi(){

  if(WiFi.status() == WL_CONNECTED) return;

  WiFi.mode(WIFI_STA);

  WiFi.begin(SSID, psswd);
  
  Serial.println("Connecting to WiFi");
  int retries = 0;

  while (WiFi.status() != WL_CONNECTED && retries < 20) {
    delay(500);
    Serial.print(".");
    retries++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    delay(1000);
    Serial.println("\nConnected to WiFi");

    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nWiFi connection failed");
  }
  Serial.println("WiFi status: " + String(WiFi.status()));
}

// Function to print local time obtained from NTP
void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Error: get time NTP");
    return;
  }
  
  Serial.printf("Fecha y hora: %02d/%02d/%04d %02d:%02d:%02d\n",
                timeinfo.tm_mday,
                timeinfo.tm_mon + 1,
                timeinfo.tm_year + 1900,
                timeinfo.tm_hour,
                timeinfo.tm_min,
                timeinfo.tm_sec);
}

// MQTT callback function to handle incoming messages
void mqttCallback(char* topic, byte* message, unsigned int length) {
  Serial.print("Mensaje recibido [");
  Serial.print(topic);
  Serial.print("]:");

  String msg;
  for (unsigned int i = 0; i < length; i++) {
    msg += (char)message[i];
  }
  Serial.println(msg);
}

// Function to reconnect to MQTT broker if connection is lost
void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting MQTT...");
    String clientId = "ESP32_" + String(NODE_ID)+"_"+String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
    
      client.subscribe(mqtt_topic_cmd.c_str());

      bool ok1 = client.publish(mqtt_topic_status.c_str(), "online", true);
     // bool ok2 = client.publish("tfm/debug/nodo1", "hola_desde_wokwi", true);

      Serial.println(ok1 ? "Estado retained ok": "Estado retained ERROR");
      //Serial.println(ok2 ? "Debug retained ok": "Debug retained ERROR");

      //client.publish(mqtt_topic_status, "online");
    } else {
      Serial.print("failed, rc=");
      Serial.println(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
      //millis() > 5000 ? delay(0) : delay(5000); // Non-blocking delay
    }
  }
}
float CalculateRS(int adcValue, float vcc = 3.3, float rl = 5.0){
  float voltage = (adcValue / 4095.0) * vcc; // Convert ADC value to voltage
  if (voltage <= 0.01) return -1; // Avoid division by zero or very low voltage
  //freturn (vcc - voltage) * rl / voltage; // Calculate RS using the voltage divider formula
  return rs;
}

// Function to initialize MQ2 sensor
void SensorMQ2(){ 
  //int rawValue = analogRead(MQ2_ANALOG_PIN); // Read the raw analog value from the MQ2 sensor
  //float gasPercentage = (rawValue / 4095.0) * 100.0; // Convert raw value to percentage (assuming 12-bit ADC)
  //Serial.println("MQ2 raw value: " + String(rawValue));
  //Serial.println("MQ2 gas percentage: " + String(gasPercentage, 2) + "%");
  //float ratio = gasPercentage / 100.0; // Convert percentage to ratio (0.0 to 1.0)


  //pinMode(MQ2_DIGITAL_PIN, INPUT); // set the pin as input
  //Serial.println("MQ2 warming up");
  MQ2Sensor mq2(MQ2_ANALOG_PIN, 3.3, 5.0, 10.0); // Create MQ2 sensor object with specified parameters

  delay(2000);
}

void publishTelemetry(){

  if(!client.connected()){
    Serial.println("MQTT not connected, skipping telemetry publish");
    return;
  }

    TempAndHumidity data = dhtSensor.getTempAndHumidity();

 // Check if any reads failed and exit early (to try again).
  if (isnan(data.temperature) || isnan(data.humidity)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  //print value sensor gas
  //sensorValue = digitalRead(MQ2pin); // read digital output pin
  gasDetected = digitalRead(MQ2_DIGITAL_PIN);
  unsigned long ts = time(nullptr);

  String payload = "{";
  payload += "\"node_id\":\"" + String(NODE_ID) + "\",";
  payload += "\"zone_id\":\"" + String(ZONE_ID) + "\",";
  payload += "\"timestamp\":" + String(ts) + ",";
  payload += "\"sensors\"{";
  payload += "\"temperature_c\":" + String(data.temperature, 2) + ",";
  payload += "\"humidity_pct\":" + String(data.humidity, 1) + ",";
  payload += "\"gas_detected\":" + String(gasDetected == 0 ? 1 : 0);
  payload += "\"gas_mq2_raw\":" + String(gas.raw) + ",";
  payload += "\"gas_mq2_ratio\":" + String(gas.ratio, 2) + ",";
  payload += "\"gas_mq2_ppm\":" + String(gas.ppm);
  payload += "\"luz\":" + String(analogRead(LIGHT_SENSOR_PIN));
  payload += "}}";

  bool ok = client.publish(mqtt_topic_data.c_str(), payload.c_str());

  Serial.println("--------Telemetria MQTT --------");

  Serial.println("Payload: " + payload);
  Serial.println(ok ? "Publicación MQTT correcta" : "Error al publicar MQTT");
  Serial.println("------------------------");

}

void setup(){

  Serial.begin(115200); //port 115200

  randomSeed(esp_random()); // Initialize random seed for MQTT client ID

  mqtt_topic_data = "tfm/ambiental/" + String(ZONE_ID)"/" + String(NODE_ID) + "/telemetria";
  mqtt_topic_status = "tfm/ambiental/" + String(ZONE_ID)"/" + String(NODE_ID) + "/estado";
  mqtt_topic_cmd = "tfm/ambiental/" + String(ZONE_ID)"/" + String(NODE_ID) + "/cmd";
  mqtt_topic_alerts = "tfm/ambiental/" + String(ZONE_ID)"/" + String(NODE_ID) + "/alerta";

  Serial.println("Nodo configurado: " + String(NODE_ID));  
  Serial.println("Zona configurada: " + String(ZONE_ID));
  
  connectWiFi();

  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  SensorMQ2();
  analogReadResolution(12); // Set ADC resolution to 12 bits (0-4095) for ESP32

  // Initialize and synchronize time with NTP
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(mqttCallback);
  
}

void loop(){
  
  if(WiFi.status() != WL_CONNECTED){
    connectWiFi();
  }

   if(!client.connected()){
    reconnectMQTT();
  }

  client.loop();

  unsigned long now = millis();
  if (now - lastPublish >= publishInterval) {
    lastPublish = now;
    publishTelemetry();
  }
  //bool ok = client.publish("sensor/wokwi/test", "hola desde Wokwi");
  //Serial.println(ok ? "Mensaje MQTT enviado" : "publish failed");

}
