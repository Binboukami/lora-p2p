#pragma once

#include <LoRa.h>
#include "common/esp_lora.h"

#ifdef HAS_DISPLAY
  #include "display.h"
#endif

Packet out_p;
Packet pacote;

void _setup() {

  // #ifdef DEBUG_SERIAL
  UART0.begin(115200);
  // #endif

  // LoRa.onTxDone(onTxDone);
  // LoRa.onReceive(onReceive);

  // LoRa.receive(); // Puts the radio in continuous receive mode.
}

#define UART_BUFF_SZ 32

char *uart_buff_in[UART_BUFF_SZ];
char *uart_buff_out[UART_BUFF_SZ];

PacketQueue packetQueue;

void _loop() {
  if(onInterval(1000))
  {
    // TODO: Process messages incoming from serial port
    // TODO: Show messages on display (for debugging)
    // Server Application -> UART -> Process -> LoRa packet
    while(UART0.available())
    {
      // 3-Step operation
      // 1: Read everything into a binary buffer
      UART0.readBytesUntil('\n', (char*)&uart_buff_in, sizeof(uart_buff_in));
      // 2: Parse header to get hints on how to parse the packet body
      //    - Read until heaader delimiter (define later) something like \h\h
      // 3: Parse packet body
      //    - Read remaining buffer content up to specified size casted by packet
      //    parse_packet((packer*)buffer);
    }

    // Forward call to remote ESP32 stub
    /*
      switch(command):
        case ???: call run_cmd_a();
      ...

      LoRa.send(call_header, call_payload)
    */

    // TODO: Process incoming RPC calls through LoRa interface and forward to server application
    // LoRa packet -> Process -> UART -> Server Application 
    // Simulate LoRa receive callback
    {
      unsigned short new_packet_id;

      char* packet_data = "Hello, world";
      packetQueue.push(0x01, 121, packet_data);

      if(UART0.availableForWrite())
      {
        if(packetQueue.isWaiting())
        {
          packetQueue.mountNext();
          packetQueue.pipe((char*)&uart_buff_out);

          UART0.write((char*)&uart_buff_out, sizeof(uart_buff_out));

          // memset(uart_buff_out, 0, sizeof(uart_buff_out));
        }
      }
    }

    #ifdef USE_DISPLAY
      display.clear();
      display.printf("Waiting...\n");
      display.printf("R: %s\n", uart_buff_in);
    #endif
  }
}

void onTxDone() {
  #ifdef DEBUG_SERIAL
    UART0.printf("Enviando opcode: %x\n", out_p.op_code);
  #endif
}

void onReceive(int packetSize) {
  LoRa.readBytes((uint8_t*)&pacote, sizeof(pacote));

  #ifdef DEBUG_SERIAL
    UART0.println("RECEBIDO: ");

    UART0.printf("OP_CODE: %u \n", pacote.op_code);
    UART0.printf("PAYLOAD: %s \n", pacote.payload);

    // print RSSI of packet
    UART0.print("' with RSSI ");
    UART0.println(LoRa.packetRssi());
  #endif
};
