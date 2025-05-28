#pragma once

#include <U8x8lib.h>
#include <config.h>

#include "common/uart_module.h"

class BaseOS {

  private:
  
  protected:
    bool use_display_ = false;

    /* Peripherals */
    HAS_DISPLAY* display_ = nullptr;

    /* Service modules*/
    UARTService uart_service;

  public:

    void initialize()
    {
      #ifdef USE_DISPLAY
        use_display_ = true;
      #endif

      if(use_display_)
      {
        display_ = new HAS_DISPLAY(OLED_RST, OLED_SCL, OLED_SDA);

        display_->begin();
        display_->setFont(u8x8_font_chroma48medium8_r);
        display_->clear();
      }

      onSetup();
    }

    /* LoRa callbacks */
    // TODO: These methods can be probably be static members of this base class
    // since the behaviour should be mostly the same for both devices
    // static void onTxDone();
    // static void onReceive();

    virtual void onSetup() = 0;
    virtual void onLoop() = 0;
    virtual void onDisplay() = 0;

    void hookTriggerDisplay()
    {
      // if (onDisplayInterval())
      // {
      //   onDisplay();
      // }
    }
};