#pragma once

#include <LoRa.h>
#include "common/esp_lora.h"
#include "display.h"

extern int counter;

Packet in_packet;
Packet out_packet;

void _setup() {

  UART0.begin(9600);

  // Callback function for when a packet transmission finish.
  LoRa.onTxDone(onTxDone);

  LoRa.receive();
  LoRa.onReceive(onReceive);
}


void _loop() {
  if(onInterval())
  {
    memset(out_packet.payload, 0, sizeof(out_packet.payload));

    // Parse incoming data from main device (Arduino Mega, in this case)
    while(UART0.available())
    {
      UART0.readBytes((uint8_t*)&out_packet, sizeof(out_packet));
    }

    // Streaming standard non-blocking packet (meant for device health status)
    LoRa.beginPacket();
    LoRa.write(out_packet.op_code);
    LoRa.printf("%s\0", out_packet.payload);
    LoRa.endPacket(true);

    // Display
    #ifdef HAS_DISPLAY
      display.clear();
      display.setCursor(0, 0);

      display.println("ENVIANDO: ");
      display.printf("OP CODE: %u\n", out_packet.op_code);
      display.printf("PAYLOAD: %s\n", out_packet.payload);

      display.println("RECEBENDO: ");
      display.printf("OP CODE: %u\n", in_packet.op_code);
    #endif

    // Pass incoming data from broker to main device
    while(UART0.availableForWrite())
    {
      UART0.write(in_packet.op_code);
    }
  }
}

void onTxDone() {}

void onReceive(int packetSize) {

  memset(&in_packet, 0, sizeof(in_packet));
  LoRa.readBytes((uint8_t*)&in_packet, sizeof(in_packet));

  // TODO: Valida pacote
};