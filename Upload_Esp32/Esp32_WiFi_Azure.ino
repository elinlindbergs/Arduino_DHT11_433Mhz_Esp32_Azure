#include <AzureIotHub.h>
#include <Esp32MQTTClient.h>
#include <ArduinoJson.h>
#include <RH_ASK.h>
#include <SPI.h>
#include <WiFi.h>

RH_ASK driver(2000, 4, 5, 0); //2000=speed, 4=receiver, 5=transmitter

char* ssid = "Network name";
char* pass = "Network password";
static char* connectionString = "connection string to azure";
static bool _connected = false;

float temp = 0;
float prev = 0;

void setup() {
  Serial.begin(115200);
  initWiFi();
  initIoTHub();
  driver.init();
}

//counting the diff
bool countDiff() {
  if (temp > (prev + 0.5) || temp < (prev - 0.5)) {
    prev = temp;
    return true;

  }
  return false;
}

void loop() {
  char payload[64];
  char message[64];
  uint8_t buf[64] = "\0";
  uint8_t buflen = sizeof(buf);

  //Checking if message is receive from 433mhz.
  if (driver.recv(buf, &buflen)) {
    Serial.println("\nReceived: ");
    Serial.println((char *)buf);
    strcpy(payload, (char*)buf);

    StaticJsonDocument<64> doc;
    deserializeJson(doc, payload);
    temp = doc["Temp°C"];
    //float humidity = doc["Hum%"];
    serializeJson(doc, message);

    //If the countDiff function returns true then message will upload to Azure IoT Explorer*/
    if (countDiff()) {
      if (Esp32MQTTClient_SendEvent(message))
      {
        Serial.println(message);
      }
    }
    delay(5000); // waiting 5 sec
  }

}
