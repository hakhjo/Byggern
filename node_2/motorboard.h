#pragma once
#include <sam.h>


#define ADC_MR_LOWRES_BITS_12 (0x0u << 5)


extern uint8_t score;


void init_PWM(void);
void init_ADC(void);
void read_ball_event(void);
void generate_pwm_cycle(uint8_t value);