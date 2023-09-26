#pragma once
#include <stdlib.h>
#include <avr/io.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define BASE_ADR_ADC ((uint8_t *)0x1400)

volatile bool adc_busy;

uint8_t read_ADC_value(uint8_t pinChannelNum);