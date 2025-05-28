#pragma once
/** @deprecated */

#include <LoRa.h>
#include "common/esp_lora.h"
#include "display.h"

extern int counter;

void _setup() {

  UART0.begin(9600);

  // Callback function for when a packet transmission finish.
  LoRa.onTxDone(onTxDone);

  LoRa.receive();
  LoRa.onReceive(onReceive);
}

#define UART_BUFF_SZ 32

char *uart_buff_in[UART_BUFF_SZ];
char *uart_buff_out[UART_BUFF_SZ];

LORA_PACKET lora_packet_in;
LORA_PACKET lora_packet_out;

PacketQueue uartPacketQueue; // UART

void onDisplay()
{
  display.clear();
  display.setCursor(0, 0);

  //
}

void _loop() {
  if(onInterval())
  {
    // Parse incoming data from main device (Arduino Mega, in this case)
    while(UART0.available())
    {
      UART0.readBytesUntil('\n', (char*)&uart_buff_in, sizeof(uart_buff_in));
    }

    // TODO: Mount LoRa packet for transmission

    // Streaming standard non-blocking packet (meant for device health status)
    // LoRa.beginPacket();
    // LoRa.write(out_packet.op_code);
    // LoRa.printf("%s\0", out_packet.payload);
    // LoRa.endPacket(true);

    // Display
    #ifdef USE_DISPLAY
      onDisplay();
    #endif

    unsigned short new_packet_id = 0x01;

    char* packet_data = "Hello, world";
    uartPacketQueue.push(new_packet_id, 121, packet_data);

    // Pass incoming data from broker to main device
    if(UART0.availableForWrite())
    {
      if(uartPacketQueue.isWaiting())
      {
        uartPacketQueue.mountNext();
        uartPacketQueue.pipe((char*)&uart_buff_out);

        UART0.write((char*)&uart_buff_out, sizeof(uart_buff_out));

        // memset(uart_buff_out, 0, sizeof(uart_buff_out));
      }
    }
  }
}

void onTxDone() {}

void onReceive(int packetSize) {
  LoRa.readBytes((uint8_t*)&lora_packet_in, sizeof(lora_packet_in));

  {
    // TODO: Valida pacote
  }
};