#pragma once
#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>

// flags

void init_interrupts(void);
void init_OLED_timer(void);
void init_input_timer(void);