#include <SPI.h>
#include <Wire.h>
#include <LoRa.h>
#include "config.h"
#include "display.h"

#ifndef REGION
  #define REGION NA
#endif

#define BAND_HZ REGION

#ifdef HAS_DISPLAY
  HAS_DISPLAY display(OLED_RST, OLED_SCL, OLED_SDA);
#endif

/*
  TODO:
    - Transmissor LoRa
        - Transmitir as letras do alfabeto a cada 2 segundos
    - Receptor LoRa
        - Receber e mostrar no display o que recebeu
*/

#define MAX_BUFF_SIZE 255
char lora_buff[MAX_BUFF_SIZE];

void setup() {

  #ifdef HAS_DISPLAY
    init_display();
  #endif

  // Init LoRa
  // LoRa.setPins()
  if(!LoRa.begin(BAND_HZ)) {
    Serial.println("Failed to initialize LoRa module \n");
    while(1);
  }

}

void loop() {

  if(!LoRa.beginPacket()) {
    // TODO: Check if busy or failed to begin packet
  }
  
  // LoRA.write(lora_buff, len);

  LoRa.endPacket();

  /*
    TODO:
      - Enable querying data from devices connected to the board so only the coordinates need
      to be streamed constantly
        - TODO: Use endPacket with non blocking mode for coordinates transmition
      - Constantly check if queries were received, and if so, proceed to decoding task and
      scheduling for data queried transmission
        - After the data queried has been transmitted, return to default scheduled task
  */
}

/* TODO: TASKS
  - DEFAULT_TASK (Trasnmit byte sized data and listen for possible queries)
  - QUERY_TASK (Parses query request and schedules task for trasnmission)
    - Each task can then be individually  defined as an operation with generic
    and specific information on how to be parsed
  - TRANSMIT_SCHEDULED_QUERIES
*/