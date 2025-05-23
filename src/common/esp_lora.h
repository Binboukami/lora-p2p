#pragma once

#include <LoRa.h>
#include "config.h"
#include "common/uart_packet.h"
#include "common/lora_packet.h"

extern long _lastSendTime;

void _setup();
void _loop();

void onTxDone();
void onReceive(int packetSize);

bool initLoRa()
{
  bool _initialized = false;

  SPI.begin(PIN_SPI_SCK, PIN_SPI_MISO, PIN_SPI_MOSI, PIN_SPI_SS);
  LoRa.setPins(PIN_SPI_SS, PIN_LORA_RST, DIO0);

  if(!LoRa.begin(BAND_HZ)) {
    #ifdef USE_SERIAL
      Serial.println("Failed to initialize LoRa module. Retrying...");
    #endif
    delay(1000);
  }
  else
  {
    _initialized = true;
    #ifdef USE_SERIAL
      Serial.println("Initialized LoRa module succesfully.");
    #endif
  }

  return _initialized; 
}

bool onInterval(long default_interval = 10000)
{
  bool pass = (millis() - _lastSendTime) > default_interval;

  if(pass)
    _lastSendTime = millis();

  return pass;
}