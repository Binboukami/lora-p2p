#pragma once

#include "common/os.h"

class BrokerOS : public BaseOS {

  private:

  protected:

  public:

    static void LORAonTxDone() {
      #ifdef DEBUG_SERIAL
        UART0.printf("Enviando opcode: %x\n", out_p.op_code);
      #endif
    };

    static void LORAonReceive() {
      // LoRa.readBytes((uint8_t*)&lora_packet_in, sizeof(lora_packet_in));
    }

    void onSetup()
    {
      UART0.begin(115200);
      LoRa.onTxDone(BrokerOS::LORAonTxDone);
    }

    void onLoop()
    {
      if(onInterval(1000))
      {
        // Parse incoming data from main device (Server application, in this case)
        // Server Application -> UART -> Process -> LoRa packet
        while(UART0.available())
        {
          this->uart_service.readInputBuffer();
        }

        // Forward call to remote ESP32 stub
        /*
          switch(command):
            case COMMAND_TYPE_A: call run_cmd_a();
              break;
            case COMMAND_TYPE_B: call run_cmd_b(command.params[0], command.params[...]);
              break;
          ...

          // TODO: Mount LoRa packet for transmission

          LoRa.send(call_header, call_payload)
        */

        // TODO: Process incoming RPC calls through LoRa interface and forward to server application
        // LoRa packet -> Process -> UART -> Server Application 
        {
          // build_packet(...); Prebuilt packets based on different rpc calls
          unsigned short new_packet_id = 0x01;
          char* packet_data = "Hello, world\n";

          this->uart_service.queuePacket((UART_PACKET_PROTO){ new_packet_id, 121, packet_data });

          if(UART0.availableForWrite())
          {
            this->uart_service.writeOutputBuffer();
          }
        }

        onDisplay();
      }
    }

    void onDisplay()
    {
      if(!this->use_display_)
        return;

      display_->clear();
      display_->setCursor(0, 0);

      display_->clear();
      display_->printf("Waiting...\n");
      display_->printf("R: %s\n", this->uart_service.uart_buff_in);
    }

};