#ifndef CAN_MESSAGE_H
#define CAN_MESSAGE_H

typedef struct
{
  struct {
    uint8_t dlc:4;
    uint8_t RTR:1;
    uint8_t IDE:1;
  };
  uint32_t id;
  uint8_t data[8];
} can_message_t;

#endif // CAN_MESSAGE_H
