#pragma once

#define MAX_PAYLOAD 127

struct Packet {
  char op_code;
  char payload[MAX_PAYLOAD];
};