
#include "menu.h"

volatile uint8_t menu_index = 0;

void display_menu(menu_item menu[], uint8_t menu_len, uint8_t menu_index)
{
    for (uint8_t i = 0; i < menu_len; i++)
    {
        OLED_write_string(menu[i].label, 2, i);
        if (i == menu_index)
        {
            OLED_write_arrow(0, i);
        }
        else
        {
            OLED_write_blank(0, i);
        }
    }
}

void navigate_menu(menu_item menu[])
{
    if (joystick_direction == DOWN && joystick_position.y <= -50 && prev_joystick_direction != DOWN)
    {
        menu_index++;
    }
    else if (joystick_direction == UP && joystick_position.y >= 50 && prev_joystick_direction != UP)
    {
        menu_index--;
    }
    else if (joystick_direction == RIGHT && joystick_position.x >= 50 && prev_joystick_direction != RIGHT)
    {
        menu[menu_index].action();
    }
}