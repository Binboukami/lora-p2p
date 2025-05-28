// @deprecated

#include <U8x8lib.h>
#include "config.h"

extern HAS_DISPLAY display;

void init_display() {
  display.begin();
  display.setFont(u8x8_font_chroma48medium8_r);
  display.clear();
}