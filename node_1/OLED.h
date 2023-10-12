#pragma once

#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>
#include "fonts.h"
#include "external_memory.h"

#define PAGES 8
#define COLUMNS 128
#define BASE_ADR_OLED_CMD ((volatile uint8_t *)0x1000)
#define BASE_ADR_OLED_DATA ((volatile uint8_t *)0x1200)

void init_OLED(void);

void OLED_write_ctrl(volatile uint8_t command);

void OLED_clear(void);
void OLED_home(void);

void OLED_display_buffer(void);
void OLED_stage_buffer(void);
void OLED_clear_buffers(void);

void OLED_write_char(char c, uint8_t x, uint8_t y);
void OLED_write_string(char char_arr[], uint8_t x, uint8_t y);
void OLED_write_arrow(uint8_t x, uint8_t y);
void OLED_write_blank(uint8_t x, uint8_t y);