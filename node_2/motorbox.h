#pragma once

#include <sam.h>

extern double error_sum;
void init_motorbox(void);
void motor_enable(void);
void motor_disable(void);
void motor_set_dir(uint8_t dir);
void motor_set_speed(uint16_t speed);
uint16_t read_encoder(void);

 double PI_controller(uint16_t encoder_value, uint16_t joystick_value);