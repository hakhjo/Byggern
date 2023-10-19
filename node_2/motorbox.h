#pragma once

#include <sam.h>

void init_motorbox(void);
void motor_enable(void);
void motor_disable(void);
void motor_set_dir(uint8_t dir);
void motor_set_speed(uint16_t speed);