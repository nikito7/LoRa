// Arduino Nano

#include <LoRa.h>
#include <ArduinoJson.h>
#include <ArduinoModbus.h>

#define LORA_SCK 13
#define LORA_MISO 12
#define LORA_MOSI 11

#define LORA_SS 10
#define LORA_RST 5
#define LORA_DI0 2

int rsdebug = -1;

void setup() {
  // modbus

  Serial.begin(9600);
  while (!Serial);

  if (!ModbusRTUClient.begin(9600)) {
    Serial.println("Failed to start Modbus RTU Client!");
    rsdebug = 1;
    while (1);
  } else {
    rsdebug = 0;
  }

  // lora

  SPI.begin();
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DI0);
  LoRa.enableCrc();
  LoRa.setSyncWord(0x12);

  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  // end setup
}

void loop() {
  // modbus
  
  int modbus = -1;

  if (!ModbusRTUClient.requestFrom(1, INPUT_REGISTERS, 0x006c, 1)) {
    modbus = 1;
  } else {
    short rawtemperature = ModbusRTUClient.read();
    modbus = 0;
  }

  // lora

  static char jsonBuffer[100];

  float tempc = 99;
  float hum = 50;
  float pres = 999;
  float up = millis() / 1000;
  char enc[50] = "djdjsjsjdjxjdn";

  StaticJsonDocument<100> doc;

  doc["id"] = "LoRa_Node7";
  doc["model"] = "easyhan.pt";
  doc["tempc"] = tempc;
  doc["enc"] = enc;
  doc["modbus"] = modbus;
  doc["rsdebug"] = rsdebug;
  doc["moi"] = up;

  size_t len = measureJson(doc) + 1;
  serializeJson(doc, jsonBuffer, len);

  LoRa.beginPacket();
  LoRa.print(jsonBuffer);
  LoRa.endPacket();

  // end loop
  delay(5000);
}

// EOF
