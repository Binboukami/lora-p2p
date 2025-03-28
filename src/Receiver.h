#pragma once

#include <LoRa.h>
#include "lora_common.h"
#include "display.h"

extern int counter;
extern int _sendInterval;
extern long _lastSendTime;

void _setup() {
  LoRa.receive(); // Puts the radio in continuous receive mode.
  LoRa.onReceive(onReceive);
}

void _loop() {}

void onReceive(int packetSize) {
  // received a packet
  Serial.print("Received packet '");

  // read packet
  for (int i = 0; i < packetSize; i++) {
    Serial.print((char)LoRa.read());
  }

  // print RSSI of packet
  Serial.print("' with RSSI ");
  Serial.println(LoRa.packetRssi());
};