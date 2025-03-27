#include <Wire.h>
#include <LoRa.h>
#include "config.h"
#include "display.h"
#include "Receiver.h"

#ifndef REGION
  #define REGION NA
#endif

#define BAND_HZ REGION

#ifdef HAS_DISPLAY
  HAS_DISPLAY display(OLED_RST, OLED_SCL, OLED_SDA);
#endif

#define SCK_LORA           5
#define MISO_LORA          19
#define MOSI_LORA          27
#define RESET_PIN_LORA     14
#define SS_PIN_LORA        18

/*
  TODO:
    - Transmissor LoRa
        - Transmitir as letras do alfabeto a cada 2 segundos
    - Receptor LoRa
        - Receber e mostrar no display o que recebeu
*/

#define MAX_BUFF_SIZE 255
char lora_buff[MAX_BUFF_SIZE];

int counter = 0;
int _sendInterval = 2000;
long _lastSendTime = 0;

void onTxDone();
void onReceive(int packetSize);
bool InitLora();

void setup() {

  Serial.begin(115200);

  while(!InitLora());

  #ifdef HAS_DISPLAY
    init_display();
  #endif

  LoRa.receive(); // Puts the radio in continuous receive mode.
  LoRa.onReceive(onReceive);

  // Callback function for when a packet transmission finish.
  LoRa.onTxDone(onTxDone); 
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
void loop() {

  if((millis() - _lastSendTime) > _sendInterval)
  {
    String message = "Hello, world\n";
    LoRa.beginPacket();
    LoRa.print(message);
    LoRa.print(counter);
    LoRa.endPacket();

    Serial.println("Enviando: " + message);
    _lastSendTime = millis();
    counter++;
  }
}

/* TODO: TASKS
  - DEFAULT_TASK (Trasnmit byte sized data and listen for possible queries)
  - QUERY_TASK (Parses query request and schedules task for trasnmission)
    - Each task can then be individually  defined as an operation with generic
    and specific information on how to be parsed
  - TRANSMIT_SCHEDULED_QUERIES
*/

void onReceive(int packetSize) { 
  // TODO: Callback for parsing incoming messages
}

void onTxDone() {
  Serial.println("TxDone");
}

/*
  @see https://www.makerhero.com/blog/comunicacao-lora-ponto-a-ponto-com-modulos-esp32-lora/?srsltid=AfmBOoo_C4YglJ10PAFk1F2ZvdahiF3xK2puvPehi5sTjggVRLYJVrqq
*/
bool InitLora() {

  bool hasInit = false;

  SPI.begin(SCK_LORA, MISO_LORA, MOSI_LORA, SS_PIN_LORA);
  LoRa.setPins(SS_PIN_LORA, RESET_PIN_LORA, LORA_DEFAULT_DIO0_PIN);

  if(!LoRa.begin(BAND_HZ)) {
    Serial.println("Failed to initialize LoRa module. Retrying...");
    delay(1000);
  }
  else
  {
    hasInit = true;
    Serial.println("Initialized LoRa module succesfully.");
  }

  return hasInit;
}