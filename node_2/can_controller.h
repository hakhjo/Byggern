/*
 * can_controller.h
 *
 * Author: Gustav O. Often and Eivind H. J�lsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */ 


#ifndef CAN_CONTROLLER_H_
#define CAN_CONTROLLER_H_

#include "sam.h"
#include "printf-stdarg.h"
#include <stdint.h>

#define  sjw 0	   // sync jump width length 1 x T_q
#define  btlmode 1 // PS2 bit time length
#define  Fq 250*16	   // baud rate prescaler, 500kHz
#define  prseg 2   // propagation segment length
#define  phseg1 6  // PS1 length
#define  phseg2 7  // PS2 length
#define brp (84000/(Fq)-1)

uint32_t can_br_value;
typedef struct can_message_t
{
	uint16_t id;
	char data_length;
	char data[8];
} CAN_MESSAGE;

uint8_t can_init_def_tx_rx_mb(uint32_t can_br);
uint8_t can_init(uint32_t can_br, uint8_t num_tx_mb, uint8_t num_rx_mb);

uint8_t can_send(CAN_MESSAGE* can_msg, uint8_t mailbox_id);
uint8_t can_receive(CAN_MESSAGE* can_msg, uint8_t mailbox_id);

#endif /* CAN_CONTROLLER_H_ */