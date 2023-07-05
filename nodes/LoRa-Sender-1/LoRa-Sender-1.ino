// Based in: 
// https://github.com/sandeepmistry/arduino-LoRa

#include <LoRa.h>
#include <ArduinoJson.h>

#define LORA_SS 15
#define LORA_RST 16
#define LORA_DI0 5

void setup() {
  Serial.begin(115200);
  while (!Serial);

  LoRa.setPins(LORA_SS, LORA_RST, LORA_DI0);
  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa sender module started!");
}

void loop() {
  static char jsonBuffer[100];

  float temperature = 99;
  float humidity = 50;
  float pressure = 999;

  StaticJsonDocument<100> doc;

  doc["tempc"] = temperature;
  doc["hum"] = humidity;
  doc["pres"] = pressure;

  size_t len = measureJson(doc) + 1;
  serializeJson(doc, jsonBuffer, len);

  Serial.println(jsonBuffer);

  if (!LoRa.isTransmitting()) {
    LoRa.beginPacket();
    LoRa.print(jsonBuffer);
    LoRa.endPacket();
  }

  delay(10000);
}

// EOF
