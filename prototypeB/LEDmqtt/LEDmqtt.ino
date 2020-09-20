#include <WiFi.h>
#include <PubSubClient.h>
const char ssid[] = "iPhone";
const char passwd[] = "inari1717";
const char* mqtt_server = "118.27.6.252";
const char* topic = "message";   

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {


  Serial.begin(115200);

  connectWiFi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}
void loop() {
   if ( WiFi.status() == WL_DISCONNECTED ) {
            connectWiFi(); 
        }
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

 
    //client.publish("esp32/temperature", "ddd");

    
  }

void connectWiFi()
{
  WiFi.begin(ssid, passwd);
  Serial.print("WiFi connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.print(" connected. ");
  Serial.println(WiFi.localIP());
}
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();


}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
   Serial.println("Connecting to MQTT...");
    String clientId = "ESP32-" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("msg");
      mqttClient.publish(topic);
    } 
     delay(1000);
            randomSeed(micros());
  }
}
