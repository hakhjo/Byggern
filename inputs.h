#pragma once
#include "adc.h"

volatile Pos_t joystick_position;
volatile uint8_t left_slider;
volatile uint8_t right_slider;
volatile uint8_t selected;
volatile int16_t lastJoystickY;
void update_global_inputs(void);
void update_selected(void);