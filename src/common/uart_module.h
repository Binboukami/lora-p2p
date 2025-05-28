#pragma once

#include "common/esp_lora.h"

#define UART_BUFF_SZ 32

enum UART_PROC_STEP {
  SYNC,
  HEADER,
  CONTENT,
  EOL
} uart_processing_step;

struct UART_PACKET_PROTO {
  unsigned short id;
  unsigned short data_len;
  char* data;
};

class UARTService {

  private:

  protected:
    size_t bytes_read = 0; 
    size_t current_packet_size = 0;

    // char *uart_buff_in[UART_BUFF_SZ];
    char *uart_buff_out[UART_BUFF_SZ];

    PacketQueue uartPacketQueue_; // UART

  public:
    char *uart_buff_in[UART_BUFF_SZ];

    /** 3-Step procedure
     *   - Read into temporary buffer until packet is complete
     *   - Copy complete packet to a packet queue for further processing
     *   - Request the queue for processing on other services
    */

    void readInputBuffer()
    {
      // TODO: Read one byte at a time instead to prevent blocking
      UART0.readBytesUntil('\n', (char*)&uart_buff_in, sizeof(uart_buff_in));
      processPacket();
    }

    void queuePacket(UART_PACKET_PROTO packet)
    {
      uartPacketQueue_.push(packet.id, packet.data_len, packet.data);
    }
    
    void writeOutputBuffer()
    {
      if(uartPacketQueue_.isWaiting())
      {
        uartPacketQueue_.mountNext();
        uartPacketQueue_.pipe((char*)&uart_buff_out);

        UART0.write((char*)&uart_buff_out, sizeof(uart_buff_out));

        // memset(uart_buff_out, 0, sizeof(uart_buff_out));
      }
    }

    /** Current implementation process only the contents of the input buffer */
    void processPacket()
    {
      byte data = 0;

      switch (uart_processing_step)
      {
        case UART_PROC_STEP::SYNC:

          data = UART0.readBytes((char*)&uart_buff_in, 0x02);

          // signature bytes
          if (data == 0xAAFF)
          {
            uart_processing_step = UART_PROC_STEP::HEADER;
          }

        break;

        case (UART_PROC_STEP::HEADER):
          
          /** Packet ID */
          data = UART0.readBytes((char*)&uart_buff_in, 0x02);
          // current_packet_id = data;

          /** Content Length, serialize as int */
          data = UART0.readBytes((char*)&uart_buff_in, 0x02);
          // current_packet_size = (int)data;

          uart_processing_step = UART_PROC_STEP::CONTENT;
        break;

        case CONTENT:
          /** Read 1 byte at a time, increament read counter till it reaches
           *  previously parsed 'Content-Length' */
          data = UART0.readBytes((char*)&uart_buff_in, 0x02);
          // current_packet_buff.append((char)data);

          if((bytes_read + 1) >= current_packet_size)
          {
            // expected to be content padding byte
            data = UART0.readBytes((char*)&uart_buff_in, 0x02);

            if (data == 'Z')
            {
              uart_processing_step = UART_PROC_STEP::EOL;
            }

            bytes_read += 1;
          }
        break;

        case EOL:
          data = UART0.readBytes((char*)&uart_buff_in, 0x02);

          if (data == '\n')
          {
            uart_processing_step = UART_PROC_STEP::SYNC;
          }

          // TODO: Commit complete packet to persitent buffer for processing loop

          break;
      }
    }

    char* getInputBuffer()
    {
      // return *this->uart_buff_in;
    }
};