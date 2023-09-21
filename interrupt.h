#pragma once
#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "OLED.h"

// flags
extern volatile bool adc_busy;

void init_interrupts(void);
void timer1_init(void);