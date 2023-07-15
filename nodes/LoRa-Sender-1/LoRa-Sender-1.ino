// Credits 
// https://github.com/sandeepmistry/arduino-LoRa

#include <LoRa.h>
#include <ArduinoJson.h>

#define LORA_SCK 14
#define LORA_MISO 12
#define LORA_MOSI 13

#define LORA_SS 15
#define LORA_RST 16
#define LORA_DI0 5

void setup() {
  Serial.begin(115200);
  while (!Serial);

  SPI.begin();
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DI0);
  LoRa.enableCrc();
  LoRa.setSyncWord(0x12);

  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa sender module started!");
}

void loop() {
  static char jsonBuffer[100];

  float tempc = 99;
  float hum = 50;
  float pres = 999;
  float up = millis() / 1000;

  StaticJsonDocument<100> doc;

  doc["id"] = "id7";
  doc["tempc"] = tempc;
  doc["hum"] = hum;
  doc["pres"] = pres;
  doc["up"] = up;

  size_t len = measureJson(doc) + 1;
  serializeJson(doc, jsonBuffer, len);

  Serial.println(jsonBuffer);

  LoRa.beginPacket();
  LoRa.print(jsonBuffer);
  LoRa.endPacket();

  delay(10000);
}

// EOF
