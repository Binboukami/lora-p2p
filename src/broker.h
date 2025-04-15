#pragma once

#include <LoRa.h>
#include "common/esp_lora.h"

#ifdef HAS_DISPLAY
  #include "display.h"
#endif


Packet out_p;
Packet pacote;

void _setup() {

  Serial.begin(115200);

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

  Serial.printf("Enviando opcode: %x\n", out_p.op_code);

}

void onReceive(int packetSize) {
  // received a packet
  Serial.print("Received packet '");
  // memset(&pacote, 0x0, sizeof(pacote));

  // read packet
  LoRa.readBytes((uint8_t*)&pacote, sizeof(pacote));

  Serial.println("RECEBIDO: ");

  Serial.printf("OP_CODE: %u \n", pacote.op_code);
  Serial.printf("PAYLOAD: %s \n", pacote.payload);

  // print RSSI of packet
  Serial.print("' with RSSI ");
  Serial.println(LoRa.packetRssi());
};
