
#include "menu.h"

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
            OLED_remove_arrow(0, i);
        }
    }
}

void navigate_menu(menu_item menu[])
{

    if (joystick_direction == DOWN && prev_joystick_direction != DOWN)
    {
        menu_index++;
    }
    else if (joystick_direction == UP && prev_joystick_direction != UP)
    {
        menu_index--;
    }
    else if (joystick_direction == RIGHT && prev_joystick_direction != RIGHT)
    {
        menu_item[menu_index].action();
    }
}