#include "ESP32_BME280_I2C.h"
#include <WiFi.h>                           
#include <WiFiUdp.h>                       
#include <PubSubClient.h>
#include "esp_sleep.h"
const uint8_t Address = 0x76;
const uint8_t sda = 21;
const uint8_t scl = 22;
const uint32_t frequency = 30000;
#define SLEEP_P 60*60*1000000ul
#define SSID "WIFIのSSIDを入れてね"                     
#define PASS "WIFIのパスワードを入れてね"

const char* mqttHost = "あなたのmqttブローカー(nodejsサーバー)のIPアドレスを入れてね";
const int mqttPort = 1883;       
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
const char* topic = "esp32";   
char* payload;
ESP32_BME280_I2C bme280i2c(Address, scl, sda, frequency);

void setup() {
  Serial.begin(115200);
  delay(1000);
  int waiting = 0;
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASS);                
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);                         
    wait++;                          
    if (wait % 10 == 0)Serial.print('.'); 
    if (wait > 300) sleep();         
  }
  Serial.println(WiFi.localIP());

  mqttClient.setServer(mqttHost, mqttPort);
  while ( ! mqttClient.connected() ) {
    Serial.println("Connecting to MQTT...");
    String clientId = "ESP32-" + String(random(0xffff), HEX);
    if ( mqttClient.connect(clientId.c_str()) ) {
      Serial.println("connected");
    }
    delay(1000);
    randomSeed(micros());
    uint8_t t_sb = 0;
    uint8_t filter = 4;
    uint8_t osrs_t = 2;
    uint8_t osrs_p = 5;
    uint8_t osrs_h = 1; 
    uint8_t Mode = 3; 
    bme280i2c.ESP32_BME280_I2C_Init(t_sb, filter, osrs_t, osrs_p, osrs_h, Mode);
    delay(1000);
  }} 

  void loop() {
   double temperature, pressure, humidity;
    bme280i2c.Read_All(&temperature, &pressure, &humidity);
    char temp_c[10], hum_c[10], pres_c[10];
    sprintf(temp_c, "%2.2lf", temperature);
    sprintf(hum_c, "%2.2lf", humidity);
    sprintf(pres_c, "%4.2lf", pressure);
    topic = "esp32temp"; 
    mqttClient.publish(topic, temp_c);
   topic = "esp32hum"; 
    mqttClient.publish(topic, hum_c);
  topic = "esp32pres"; 
    mqttClient.publish(topic, pres_c);
    delay(1000);
    mqttClient.loop();
   sleep();
  }

 
  
  void sleep() {
    delay(200);                            
    esp_deep_sleep(SLEEP_P);                
  }
