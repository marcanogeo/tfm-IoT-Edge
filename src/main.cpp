#include <Arduino.h>
#include "DHTesp.h"
#include <WiFi.h>
//#include <HTTPClient.h>
#include "time.h"
#include <PubSubClient.h>

#define MQ2pin 14 //Pin where the MQ2 sensor is connected

//Setting WiFi credentials
const char* SSID = "Wokwi-GUEST";
const char* psswd = "";
//String SSID = "Wokwi-GUEST";
//String psswd = "";

//MQTT Broker settings
//const char* mqtt_server = "broker.emqx.io";
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

const char* NODE_ID = "nodo1";
const char* ZONE_ID = "zona_norte";

const char* mqtt_topic_data = "tfm/ambiental/nodo1/telemetria";
const char* mqtt_topic_status = "tfm/ambiental/nodo1/estado";
const char* mqtt_topic_cmd = "tfm/ambiental/nodo1/cmd";

//Conexión a thingsboard.io
//String tbhost = "https://demo.thingsboard.io";
//String tbToken = "bqyi9n4pw2li4huo4019";

// Configuración NTP
const char* ntpServer = "pool.ntp.org"; // NTP server
const long  gmtOffset_sec = 3600;       // Time offset in seconds (Spain mainland: UTC+1)
const int   daylightOffset_sec = 3600;  // Daylight saving time: +1 hour

WiFiClient espClient;
PubSubClient client(espClient);
DHTesp dhtSensor; // Create DHT sensor object

const int DHT_PIN = 27; // Pin where the DHT22 sensor is connected

unsigned long lastPublish = 0;
const unsigned long publishInterval = 5000; // Publish every 5 seconds

int gasValue = 0; // Variable to store the gas sensor value
//int sensorValue; // variable to store the sensor value

 // WiFi connection function
void connectWiFi(){

  WiFi.mode(WIFI_STA);
  //WiFi.begin(SSID.c_str(), psswd.c_str());
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
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nWiFi connection failed");
  }
  Serial.println("WiFi status: " + String(WiFi.status()));
}

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

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting MQTT...");
    String clientId = "ESP32_" + String(NODE_ID)+"_"+String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      //client.subscribe("sensor/wokwi/#");
      Serial.println("Client ID: " + clientId);
      Serial.println("Topic Telemetria: " + String(mqtt_topic_data));
      Serial.println("Topic status: " + String(mqtt_topic_status));
    
      client.subscribe(mqtt_topic_cmd);

      bool ok1 = client.publish(mqtt_topic_status, "online", true);
     // bool ok2 = client.publish("tfm/debug/nodo1", "hola_desde_wokwi", true);

      Serial.println(ok1 ? "Estado retained ok": "Estado retained ERROR");
      //Serial.println(ok2 ? "Debug retained ok": "Debug retained ERROR");

      //client.publish(mqtt_topic_status, "online");
    } else {
      Serial.print("failed, rc=");
      Serial.println(client.state());
      Serial.println(" try again in 5 seconds");
      //delay(5000);
      millis() > 5000 ? delay(0) : delay(5000); // Non-blocking delay
    }
  }
}

void SensorMQ2(){ //MQ2 sensor initialization
  pinMode(MQ2pin, INPUT); // set the pin as input
  Serial.println("MQ2 warming up");
  delay(200);
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

  //client.publish("sensor/wokwi/test", "hola desde Wokwi");
  //Serial.println("Mensaje MQTT enviado");

  //print temperature and humidity
  //Serial.println("Temp " + String(data.temperature, 2) + "ºC");
  //Serial.println("Humidity " + String(data.humidity, 1) + "%");

  //print value sensor gas
  //sensorValue = digitalRead(MQ2pin); // read digital output pin
  gasValue = digitalRead(MQ2pin);
  unsigned long ts = time(nullptr);
  /*if(sensorValue){
    Serial.println("Smoke:  -");
  }else{
    Serial.println("Smoke: Detected");
  }
  Serial.println("--------------");*/

  //String payload = "{";
  //payload +="\nodo_id\":"+ String(random(0xffff), HEX) + ",";
  //payload += "\"timestamp\":" + String(time(nullptr));
  //payload += "\"temperature\":" + String(data.temperature, 2) + ",";
  //payload += "\"humidity\":" + String(data.humidity, 1) + ",";
  //payload += "\"gas\":" + String(sensorValue) + ",";
  //payload += "\"status\":" + String(client.state());
  //payload += "}";
  String payload = "{";
  payload += "\"node_id\":\"" + String(NODE_ID) + "\",";
  payload += "\"zone_id\":\"" + String(ZONE_ID) + "\",";
  payload += "\"timestamp\":" + String(ts) + ",";
  payload += "\"temperature_c\":" + String(data.temperature, 2) + ",";
  payload += "\"humidity_pct\":" + String(data.humidity, 1) + ",";
  payload += "\"gas_detected\":" + String(gasValue == 0 ? 1 : 0);
  payload += "}";
  //client.publish(mqtt_topic_temp, String(data.temperature, 2).c_str());
  //client.publish(mqtt_topic_hum, String(data.humidity, 1).c_str());
  //client.publish(mqtt_topic_gas, String(sensorValue).c_str());
  //client.publish(mqtt_topic_payload, payload.c_str());
  //client.publish("sensor/wokwi/payload", payload.c_str());
  bool ok = client.publish(mqtt_topic_data, payload.c_str());

  Serial.println("--------Telemetria MQTT --------");
  Serial.println("Temperature:" + String(data.temperature, 2) + "ºC");
  Serial.println("Humidity: " + String(data.humidity, 1) + " %");
  Serial.println(String("Gas: ") + (gasValue == 0 ? "SI" : "NO"));
  Serial.println("Payload: " + payload);
  Serial.println(ok ? "Publicación MQTT correcta" : "Error al publicar MQTT");
  Serial.println("------------------------");
  //Serial.println(ok ? "Topicos enviados" : "ERROR al enviar topicos");

}

void setup(){

  Serial.begin(115200); //port 115200

  randomSeed(esp_random()); // Initialize random seed for MQTT client ID

  connectWiFi();

  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  SensorMQ2();

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
