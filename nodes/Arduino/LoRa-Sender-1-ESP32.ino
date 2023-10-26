// ESP32

#include <LoRa.h>
#include <ArduinoJson.h>


// esp32 ttgo
// select correct board in arduino

#define LORA_SCK 5
#define LORA_MISO 19
#define LORA_MOSI 27

#define LORA_SS 18
#define LORA_RST 23
#define LORA_DI0 26

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.print("MOSI: ");
  Serial.println(MOSI);
  Serial.print("MISO: ");
  Serial.println(MISO);
  Serial.print("SCK: ");
  Serial.println(SCK);
  Serial.print("SS: ");
  Serial.println(SS);   

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

  doc["id"] = "LoRa_Node32";
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
