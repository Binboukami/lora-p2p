#pragma once

#define PACKET_PAYLOAD_MAX_SZ 127
#define PACKET_QUEUE_MAX_SZ 2
#include <queue>

/** TODO: Debug assert the packet size, should be ~127 bytes */
struct UART_PACKET {
  struct UART_PACKET_HEADER {
    char _SYNCTX0_;                         // 1 byte
    char _SYNCTX1_;                         // 1 byte
    unsigned short _ID_;                    // 2 bytes
    unsigned short _LEN_;                   // 2 bytes
  } _HEAD_;
  char _DATA_[PACKET_PAYLOAD_MAX_SZ];       // 121 bytes
  char _C_PAD_;                             // 1 byte
  char _ETX_;                               // 1 byte
};

struct UART_PACKET_MODIFIABLE_CONTENT {
  unsigned short _ID_;
  unsigned short _LEN_;
  char* _DATA_;
};

struct PacketQueue {
  UART_PACKET _proto_uart_packet;

  size_t _packets_queue_max_sz = PACKET_QUEUE_MAX_SZ;
  std::queue<UART_PACKET_MODIFIABLE_CONTENT> _packets;

  /** Returns true if there are packets in the queue */
  inline bool isWaiting() { return (!_packets.empty()); };

  bool push(unsigned short id, unsigned short data_len, char* data)
  {
    size_t current_size = _packets.size();

    if(current_size == _packets_queue_max_sz)
      return 1;

    _packets.push(
      (UART_PACKET_MODIFIABLE_CONTENT){ ._ID_ = id, ._LEN_ = data_len, ._DATA_ = data });

    // Pushed sucessfully if 0
    return (_packets.size() > current_size);
  }

  void mountNext()
  {
    UART_PACKET_MODIFIABLE_CONTENT next = _packets.back();

    _proto_uart_packet._HEAD_._ID_ = next._ID_;
    _proto_uart_packet._HEAD_._LEN_ = next._LEN_;

    strcpy(_proto_uart_packet._DATA_, next._DATA_);

    _packets.pop();
  }

  /** Consumes the internal proto_packet */
  inline void pipe(char* output_buffer)
  {
    if (sizeof(output_buffer) < sizeof(_proto_uart_packet))
    {
      // TODO: Handle buffer overflow
    }

    memcpy((char*)&output_buffer, &_proto_uart_packet, sizeof(_proto_uart_packet));

    // clean-up
    _proto_uart_packet._HEAD_._ID_ = 0;
    _proto_uart_packet._HEAD_._LEN_ = 0;
    memset(_proto_uart_packet._DATA_, 0, sizeof(char)*PACKET_PAYLOAD_MAX_SZ);
  }
};