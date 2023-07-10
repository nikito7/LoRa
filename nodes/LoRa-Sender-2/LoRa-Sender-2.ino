// Credits: 
// https://github.com/sandeepmistry/arduino-LoRa

#include <LoRa.h>

#define LORA_SCK 14
#define LORA_MISO 12
#define LORA_MOSI 13
#define LORA_SS 15
#define LORA_RST 16
#define LORA_DI0 5

void setup() {

  Serial.begin(115200);

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
  float temperature = millis();
  float humidity = 50;
  float pressure = 9999;

  char tempStr[10];
  char humStr[10];
  char presStr[10];

  dtostrf(temperature, 4, 1, tempStr);
  dtostrf(humidity, 4, 1, humStr);
  dtostrf(pressure, 6, 1, presStr);

  String nodeData = "|id|";
  nodeData += String(tempStr);
  nodeData += "|";
  nodeData += String(humStr);
  nodeData += "|";
  nodeData += String(presStr);
  nodeData += "|";

  Serial.println(nodeData);

  LoRa.beginPacket();
  LoRa.print(nodeData);
  LoRa.endPacket();
  
  delay(10000);
}

// EOF
