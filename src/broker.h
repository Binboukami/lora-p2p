#pragma once

#include <LoRa.h>
#include "common/esp_lora.h"

#ifdef HAS_DISPLAY
  #include "display.h"
#endif

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

LORA_PACKET lora_packet_in;
PacketQueue uartPacketQueue; // UART

void onDisplay()
{
  display.clear();
  display.setCursor(0, 0);

  display.clear();
  display.printf("Waiting...\n");
  display.printf("R: %s\n", uart_buff_in);
}

void _loop() {
  if(onInterval(1000))
  {
    // Parse incoming data from main device (Server application, in this case)
    // Server Application -> UART -> Process -> LoRa packet
    while(UART0.available())
    {
      // 3-Step operation
      // 1: Read everything into a binary buffer
      // pipe_into_buffer(...)
      UART0.readBytesUntil('\n', (char*)&uart_buff_in, sizeof(uart_buff_in));
      // 2: Parse header to get hints on how to parse the packet body
      // 3: Parse packet body
      //    - Read remaining buffer content up to specified size casted by packet
      // parse_packet((packet*)buffer);
    }

    // Forward call to remote ESP32 stub
    /*
      switch(command):
        case COMMAND_TYPE_A: call run_cmd_a();
          break;
        case COMMAND_TYPE_B: call run_cmd_b(command.params[0], command.params[...]);
          break;
      ...

      // TODO: Mount LoRa packet for transmission

      LoRa.send(call_header, call_payload)
    */

    // TODO: Process incoming RPC calls through LoRa interface and forward to server application
    // LoRa packet -> Process -> UART -> Server Application 
    {
      // build_packet(...); Prebuilt packets based on different rpc calls
      unsigned short new_packet_id = 0x01;

      char* packet_data = "Hello, world";
      uartPacketQueue.push(new_packet_id, 121, packet_data);

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

    #ifdef USE_DISPLAY
      onDisplay();
    #endif
  }
}

void onTxDone() {
  #ifdef DEBUG_SERIAL
    UART0.printf("Enviando opcode: %x\n", out_p.op_code);
  #endif
}

void onReceive(int packetSize) {
  LoRa.readBytes((uint8_t*)&lora_packet_in, sizeof(lora_packet_in));

  #ifdef DEBUG_SERIAL
    UART0.println("RECEBIDO: ");

    UART0.printf("OP_CODE: %u \n", pacote.op_code);
    UART0.printf("PAYLOAD: %s \n", pacote.payload);

    // print RSSI of packet
    UART0.print("' with RSSI ");
    UART0.println(LoRa.packetRssi());
  #endif
};
