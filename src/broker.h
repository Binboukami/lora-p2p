#pragma once

#include <LoRa.h>
#include "common/esp_lora.h"

#ifdef HAS_DISPLAY
  #include "display.h"
#endif

Packet out_p;
Packet pacote;

void _setup() {

  #ifdef DEBUG_SERIAL
    UART0.begin(115200);
  #endif

  LoRa.onTxDone(onTxDone);
  LoRa.onReceive(onReceive);

  LoRa.receive(); // Puts the radio in continuous receive mode.
}

void _loop() {
  if(onInterval())
  {
    // Sends messages to single/multiple clients
    out_p.op_code = 0x03;
    strcpy(out_p.payload, "Liga motor\0");

    // enviando 0x03
    LoRa.beginPacket();
    LoRa.write(out_p.op_code);
    LoRa.printf("%s\0", out_p.payload);
    LoRa.endPacket(true);

    #ifdef HAS_DISPLAY
      display.setCursor(0, 0);
      display.println("RECEBIDO: ");
      display.printf("OP_CODE: %u \n", pacote.op_code);
      display.printf("PAYLOAD: %s \n", pacote.payload);

      display.println("ENVIANDO: ");
      display.printf("OP_CODE: %u \n", out_p.op_code);
      display.printf("PAYLOAD: %s \n", out_p.payload);
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
