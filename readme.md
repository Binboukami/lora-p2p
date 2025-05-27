# P2P Module for ESP32 w/ LoRa chip

## Device Specification
  The project was developed to work primaly on the sole developming platform but can be ported easily to different specs with minor adjustments.

  **Specs**:

  ESP32S3FH4R2 with LoRa Tranceiver SX1276

  Flash: `4MB`

  PSRAM: `2MB`

  Frequency Band: `868/916Mhz`
  

## Configuration

### Build Flags

  **REGION**
  
  Supported values:

  `NA: 915Hz`, `EU: 866Hz`, `AS: 433Hz`

  Default: `NA`
  ___


  **OS_MODE**

  Supported values:
  
  `CLIENT`, `BROKER`

  Default: `CLIENT`