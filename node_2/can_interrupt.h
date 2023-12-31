/*
 * can_interrupt.h
 *
 * Author: Gustav O. Often and Eivind H. J�lsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */ 


#ifndef CAN_INTERRUPT_H_
#define CAN_INTERRUPT_H_


#include <stdio.h>
#include "sam.h"
#include "printf-stdarg.h"
#include "can_controller.h"


volatile uint8_t received;
volatile int8_t x_dir;
volatile int8_t y_dir;
volatile uint8_t right_slider;
volatile uint8_t r_btn;
void CAN0_Handler       ( void );






#endif /* CAN_INTERRUPT_H_ */