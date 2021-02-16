#include <Adafruit_Sensor.h>
#include <ArduinoJson.h>
#include <RH_ASK.h>
#include <SPI.h>
#include <DHT.h>

#define DHT_PIN A0
#define DHT_TYPE DHT11
DHT dht(DHT_PIN, DHT_TYPE);

RH_ASK driver;

void setup() {
  Serial.begin(115200);
  dht.begin();
  driver.init();
}

void loop() {
  char msg[64];

  //Creates an object
  StaticJsonDocument<64> doc;
  JsonObject object = doc.to<JsonObject>();
  object["Temp°C"]= dht.readTemperature();//Adding the temperature data from the DHT reading to a JsonObject.
  object["Hum%"]= dht.readHumidity();//Adding the humidity data from the DHT reading to a JsonObject.
  serializeJson(doc, msg); // serialize the object and send the result to msg

  //sending data through RF433-mhz
  driver.send((uint8_t *)msg, strlen(msg));
  driver.waitPacketSent();  

  Serial.print("Sent: ");
  Serial.println(msg);
  delay(1000);
  
}
