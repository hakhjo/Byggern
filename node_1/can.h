#pragma once
#include "spi.h"
#include "inputs.h"
#include "MCP2515.h"

#define CAN_STANDARD_MESSAGE_ID_BITS 29 // CAN2.0B


//typedef struct {
    //uint32_t id;        // CAN identifier (11-bit or 29-bit)
    //uint8_t  dlc;       // Data length code: number of bytes in data field (0-8)
    //uint8_t  data[8];   // Data field (up to 8 bytes)
    //uint8_t  extended;  // Extended identifier flag (1 for extended, 0 for standard)
    //uint8_t  rtr;       // Remote request flag (1 for RTR frame, 0 for data frame)
//} CAN_extended_frame;

typedef struct Message {
	uint16_t id;
	uint8_t length;
	char data[8];
} message_t, *message_ptr;


void init_CAN(void);
void can_send_joystick_position(void);
void can_send(message_ptr message);
message_t can_receive(void);