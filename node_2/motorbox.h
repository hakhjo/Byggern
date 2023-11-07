#pragma once

#include <sam.h>
#include <stdlib.h>

void init_motorbox(void);
void motor_enable(void);
void motor_disable(void);
void motor_set_dir(uint8_t dir);
void motor_set_speed(uint16_t speed);
uint16_t read_encoder(void);
volatile uint16_t joystick_pos;
volatile uint16_t encoder_pos;
volatile int32_t output;

 int32_t PI_controller(uint16_t encoder_value, uint16_t joystick_value);
 
 uint16_t map_joystick(int8_t joystick_position);