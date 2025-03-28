#pragma once

#include <LoRa.h>
#include "lora_common.h"

#ifdef HAS_DISPLAY
  #include "display.h"
#endif

/*
  Possibly split receiver code into a separate repo for integration with server UI
*/

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
    char character = (char)LoRa.read();

    Serial.print(character);

    #ifdef HAS_DISPLAY
      display.print(character);
    #endif
  }

  // print RSSI of packet
  Serial.print("' with RSSI ");
  Serial.println(LoRa.packetRssi());
};