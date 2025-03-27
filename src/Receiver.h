#pragma once

#include <LoRa.h>

void AsyncPacket(String message)
{
  LoRa.beginPacket();
  LoRa.print(message);
  LoRa.endPacket();
}
