// Credits 
// https://github.com/sandeepmistry/arduino-LoRa
// ESP8266
// No RST
// Sender to openmqttgateway
// Status: ok (2023-10-01)

#include <LoRa.h>
#include <ArduinoJson.h>

#define LORA_SCK 14
#define LORA_MISO 12
#define LORA_MOSI 13

#define LORA_SS 15
#define LORA_RST -1
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

  float tempc = 33;
  float hum = 77;
  float moi = 88;
  float pres = 1009;
  float up = millis() / 1000;

  StaticJsonDocument<200> doc;

  doc["id"] = "Node7_esp8266";
  doc["tempc"] = tempc;
  doc["hum"] = hum;
  doc["pres"] = pres;
  doc["moi"] = moi;
  doc["model"] = "esp8266dev";
  doc["up"] = up;

  size_t len = measureJson(doc) + 1;
  serializeJson(doc, jsonBuffer, len);

  Serial.println(jsonBuffer);

  LoRa.beginPacket();
  LoRa.print(jsonBuffer);
  LoRa.endPacket();

  delay(21000);
}

// EOF
