#pragma once
#include <sam.h>

#define PWM_PERIOD 20 //ms
#define FCPU 84 //MHz
#define PWM_PRESCALER 1024 
#define FCHANNEL_X_CLOCK  (FCPU*1000000 / PWM_PRESCALER)
#define ADC_MR_LOWRES_Pos 4
#define ADC_MR_LOWRES_BITS_12 (0x0u << 5)
#define ADC_MR_LOWRES_BITS_10 (0x1u << 5)

extern uint8_t score;


void init_PWM(void);
void init_ADC(void);
void read_adc(void);
void generate_pwm_cycle(int8_t value);