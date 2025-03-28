#include <Wire.h>
#include "config.h"

#if OS_MODE == SENDER
  #include "sender.h"
#else
  #include "receiver.h"
#endif

#ifdef HAS_DISPLAY
  HAS_DISPLAY display(OLED_RST, OLED_SCL, OLED_SDA);
#endif

/*
  TODO:
    - Transmissor LoRa
        - Transmitir as letras do alfabeto a cada 2 segundos
    - Receptor LoRa
        - Receber e mostrar no display o que recebeu

  TODO: TASKS
  - DEFAULT_TASK (Trasnmit byte sized data and listen for possible queries)
  - QUERY_TASK (Parses query request and schedules task for trasnmission)
    - Each task can then be individually  defined as an operation with generic
    and specific information on how to be parsed
  - TRANSMIT_SCHEDULED_QUERIES
*/


int counter = 0;
int _sendInterval = 2000;
long _lastSendTime = 0;

void setup() {

  Serial.begin(115200);

  while(!initLoRa());

  #ifdef HAS_DISPLAY
    init_display();
  #endif

  _setup();
}

/*
  TODO:
    - Enable querying data from devices connected to the board so only the coordinates need
    to be streamed constantly
      - TODO: Use endPacket with non blocking mode for coordinates transmition
    - Constantly check if queries were received, and if so, proceed to decoding task and
    scheduling for data queried transmission
      - After the data queried has been transmitted, return to default scheduled task
*/
void loop()
{
  _loop();
}