#pragma once
#include <stdlib.h>
#include <avr/io.h>
#include <stdbool.h>
#include <avr/interrupt.h>

#include <stdio.h>

#define BASE_ADR_ADC ((uint8_t*)0x1400)
extern volatile bool adc_busy;

//global variables
uint8_t x_init_pos;
uint8_t y_init_pos;
typedef enum JoystickDirection {UP, DOWN, RIGHT, LEFT, NEUTRAL} JoystickDirection;
typedef enum Button {LEFT_SLIDER, Y_JOYSTICK, RIGHT_SLIDER, X_JOYSTICK} Button;

typedef struct Pos_t
{
    int8_t x; // Range -100 - 100
    int8_t y; // Range -100 - 100
    
} Pos_t;

void print_pos(Pos_t pos);
Pos_t get_joystick_analog_position(void);
uint8_t read_ADC_value(uint8_t pinChannelNum);
int8_t map_input_value(uint8_t adcValue, Button b);
void init_joystick(void);
JoystickDirection getjoystickdirection(Pos_t pos);