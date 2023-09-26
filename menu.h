#pragma once
#include "inputs.h"
#include "OLED.h"

typedef struct menu_item
{
    char label[COLUMNS / 4];
    void (*action)(void);

} menu_item;

void display_menu(menu_item menu[], uint8_t menu_len, uint8_t menu_index);
void navigate_menu(menu_item menu[]);
void volatile uint8_t menu_index;