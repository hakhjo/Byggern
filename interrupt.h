#pragma once
#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "OLED.h"
#include "inputs.h"
#include "adc.h"

// flags

void init_interrupts(void);
void timer1_init(void);
void Timer3_init(void);