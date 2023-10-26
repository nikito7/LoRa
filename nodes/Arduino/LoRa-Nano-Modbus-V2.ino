// Arduino Nano, Modbus, LoRa, edpbox, rs485.
// Status: uint32 not working. volt ok.

#include <LoRa.h>
#include <ArduinoJson.h>
#include <ModbusMaster.h>

// instantiate ModbusMaster object
ModbusMaster node;

#define LORA_SCK 13
#define LORA_MISO 12
#define LORA_MOSI 11

#define LORA_SS 10
#define LORA_RST 5
#define LORA_DI0 2

void setup() {
  // modbus

  Serial.begin(9600);
  while (!Serial);

  // communicate with Modbus slave ID 1 over Serial (port 0)
  node.begin(1, Serial);

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
  uint16_t volt = 0;

  uint8_t result;

  // slave: read (1) 16-bit register
  result = node.readInputRegisters(0x006c, 1);
  if (result == node.ku8MBSuccess)
  {
    modbus = 0;
    volt = node.getResponseBuffer(0);
  }

  // slave: read (1) 32-bit register
  uint32_t power;
  result = node.readInputRegisters(0x0016, 1);
  if (result == node.ku8MBSuccess)
  {
    modbus = 0;
    // not working
    power = node.getResponseBuffer(0) + node.getResponseBuffer(1) << 16;
  }

  // lora

  static char jsonBuffer[100];

  float hum = 50;
  float pres = 999;
  float up = millis() / 1000;

  StaticJsonDocument<100> doc;

  doc["id"] = "LoRa_Node7";
  doc["volt"] = volt;
  doc["power"] = power;
  doc["model"] = "easyhan.pt";
  doc["modbus"] = modbus;
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
