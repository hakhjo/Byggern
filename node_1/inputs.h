#pragma once
#include "adc.h"
#include <avr/interrupt.h>
#include <stdbool.h>


typedef enum JoystickDirection
{
    UP,
    DOWN,
    RIGHT,
    LEFT,
    NEUTRAL
} JoystickDirection;

typedef enum InputDevice
{
    LEFT_SLIDER,
    Y_JOYSTICK,
    RIGHT_SLIDER,
    X_JOYSTICK
} InputDevice;

typedef struct JoystickPosition
{
    int8_t x; // Range -100 - 100
    int8_t y; // Range -100 - 100
} JoystickPosition;

JoystickPosition joystick_offset;
volatile JoystickDirection prev_joystick_direction;
volatile JoystickPosition joystick_position;
volatile JoystickDirection joystick_direction;
volatile uint8_t left_slider;
volatile uint8_t right_slider;
volatile uint8_t r_btn;

volatile bool inputs_updated;

void init_joystick(void);

void update_global_inputs(void);
void print_pos(JoystickPosition pos);
JoystickPosition read_mapped_joystick_position(void);
int8_t map_input_value(uint8_t adcValue, InputDevice b);
JoystickDirection get_joystick_direction(JoystickPosition pos);
