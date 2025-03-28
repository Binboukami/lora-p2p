
/* Board Hardware Definitions */
/* Wrap around conditional if board is replaced during development */
#include "hal/ttgov2.h"

/* OS MODES 
  Uses build flags to keep all the source code in the same project
*/
#define SENDER 0x00
#define RECEIVER 0x01

/* REGION IDENTIFIERS */
#define AS 433E6
#define EU 866E6
#define NA 915E6

#ifndef REGION
  #define REGION NA
#endif

#define BAND_HZ REGION

/* SX1276 CUSTOM SPI PINS */
#define PIN_LORA_RST     14
