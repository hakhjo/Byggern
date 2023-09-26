#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>

#define BASE_ADR_SRAM_WRITE_BUFFER ((volatile uint8_t *)0x1800)
#define BASE_ADR_SRAM_DISPLAY_BUFFER ((volatile uint8_t *)0x1c00)

void SRAM_test(void);
void init_external_memory(void);