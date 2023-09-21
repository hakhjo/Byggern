#pragma once

#include <stdint.h>
#include <string.h>
#include "fonts.h"
#include <avr/pgmspace.h>
#include "adc.h"

#define PAGES 8
#define COLUMNS 128
#define BASE_ADR_OLED_CMD ((volatile uint8_t *)0x1000)
#define BASE_ADR_OLED_DATA ((volatile uint8_t *)0x1200)
#define BASE_ADR_SRAM_WRITE_BUFFER ((volatile uint8_t *)0x1800)
#define BASE_ADR_SRAM_DISPLAY_BUFFER ((volatile uint8_t *)0x1c00)

//typedef struct menu_item
//{
//    char label[COLUMNS / 4];
    //void (*function)();

//} menu_item;

// menu_item new_game = {"New game", *OLED_new_game};
// menu_item highscore = {"Highscore", *OLED_highscore};
// menu_item game_main_menu = {new_game, highscore, options, quit};
// menu_item options = {"Options", *OLED_options};
// menu_item quit = {"Quit", *OLED_quit};

void init_OLED(void);

void OLED_new_game(void);

void OLED_write_ctrl(volatile uint8_t command);
// void OLED_write_char(char c);

void OLED_clear(void);
void OLED_reset(void);
void OLED_home(void);
void OLED_goto_line(uint8_t line);
void OLED_goto_column(uint8_t col);
void OLED_clear_line(void);
void OLED_clear_column(void);

void OLED_display_user_interface(void);
void OLED_reset_game(void);
void OLED_game_won(void);

void OLED_display_buffer(void);
void OLED_stage_buffer(void);
void OLED_write_char(uint8_t x, uint8_t y, char c);
void OLED_clear_buffer(void);