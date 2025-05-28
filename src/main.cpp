#include <Wire.h>
#include "config.h"
#include "os.h"

#if OS_MODE == CLIENT
  #include "client.h"
#else
  #include "broker_os.h"
#endif

#ifdef USE_DISPLAY
  HAS_DISPLAY display(OLED_RST, OLED_SCL, OLED_SDA);
#endif

#define SCK_LORA           5
#define MISO_LORA          19
#define MOSI_LORA          27
#define RESET_PIN_LORA     14
#define SS_PIN_LORA        18

/*
  TODO: TASKS
  - DEFAULT_TASK (Trasnmit byte sized data and listen for possible queries)
  - QUERY_TASK (Parses query request and schedules task for trasnmission)
    - Each task can then be individually  defined as an operation with generic
    and specific information on how to be parsed
  - TRANSMIT_SCHEDULED_QUERIES
*/

BrokerOS comms_os;

int counter = 0;
long _lastSendTime = 0;

void setup() {

  while(!initLoRa());

  comms_os.initialize();
}

void loop()
{
  comms_os.onLoop();
}