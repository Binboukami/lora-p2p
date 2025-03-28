#pragma once

#include <LoRa.h>
#include "lora_common.h"
#include "display.h"

extern int counter;
extern int _sendInterval;
extern long _lastSendTime;

void _setup() {
  // Callback function for when a packet transmission finish.
  LoRa.onTxDone(onTxDone); 
}

void _loop() {
  if((millis() - _lastSendTime) > _sendInterval)
  {
    String message = "Hello, world\n";
    LoRa.beginPacket();
    LoRa.print(message);
    LoRa.print(counter);
    LoRa.endPacket(true); // Non-blocking

    Serial.println("Enviando: " + message);

    display.drawString(0, 0, "Enviando: ");
    display.drawString(0, 1, message.c_str());

    display.setCursor(0, 2);
    display.print("Counter:  ");
    display.print(counter);

    _lastSendTime = millis();

    counter++;
  }
}

void onTxDone() {
  Serial.println("TxDone");
}
void onReceive(int packetSize) {};