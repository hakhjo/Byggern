#include "OLED.h"

void init_OLED(void)
{
    OLED_write_ctrl(0xae); // display off
    OLED_write_ctrl(0xa1); // segment remap
    OLED_write_ctrl(0xda); // common pads hardware: alternative
    OLED_write_ctrl(0x12);
    OLED_write_ctrl(0xc8); // common output scan direction:com63~com0
    OLED_write_ctrl(0xa8); // multiplex ration mode:63
    OLED_write_ctrl(0x3f);
    OLED_write_ctrl(0xd5); // display divide ratio/osc. freq. mode
    OLED_write_ctrl(0x80);
    OLED_write_ctrl(0x81); // contrast control
    OLED_write_ctrl(0x50);
    OLED_write_ctrl(0xd9); // set pre-charge period
    OLED_write_ctrl(0x21);
    OLED_write_ctrl(0x20); // Set Memory Addressing Mode
    OLED_write_ctrl(0x00); // horizontal addressing mode
    // write_c(0x02);
    OLED_write_ctrl(0xdb); // VCOM deselect level mode
    OLED_write_ctrl(0x30);
    OLED_write_ctrl(0xad); // master configuration
    OLED_write_ctrl(0x00);
    OLED_write_ctrl(0xa4); // out follows RAM content
    OLED_write_ctrl(0xa6); // set normal display
    OLED_write_ctrl(0xaf); // display on

    OLED_reset();
}

void OLED_write_ctrl(volatile uint8_t command)
{
    *BASE_ADR_OLED_CMD = command;
}

void OLED_clear(void)
{
    for (int i = 0; i < 1024; i++)
    {
        *BASE_ADR_OLED_DATA = 0x00;
    }
}

void OLED_home(void)
{
    // reset the pointer
    // columns
    OLED_write_ctrl(0x21); // setting column adress
    OLED_write_ctrl(0x00); //
    OLED_write_ctrl(0x7F);

    // rows
    OLED_write_ctrl(0x22);
    OLED_write_ctrl(0x00);
    OLED_write_ctrl(0x07);
}

void OLED_reset(void)
{

    // clear screen
    OLED_clear();
    // moving the pointer
    OLED_home();
}

void OLED_goto_line(uint8_t line)
{
    OLED_write_ctrl(0x22);
    OLED_write_ctrl(line);
    OLED_write_ctrl(0x07);
}

void OLED_display_buffer(void)
{

    OLED_home();
    for (uint16_t i = 0; i < PAGES * COLUMNS; i++)
    {
        *BASE_ADR_OLED_DATA = BASE_ADR_SRAM_DISPLAY_BUFFER[i];
    }
}

void OLED_stage_buffer(void)
{

    for (uint16_t i = 0; i < PAGES * COLUMNS; i++)
    {
        BASE_ADR_SRAM_DISPLAY_BUFFER[i] = BASE_ADR_SRAM_WRITE_BUFFER[i];
    }
    // memcpy(BASE_ADR_SRAM_WRITE_BUFFER, BASE_ADR_SRAM_DISPLAY_BUFFER, PAGES * COLUMNS);
}

void OLED_write_char(uint8_t x, uint8_t y, char c)
{
    uint16_t index = ((x * 8) + COLUMNS * y);
    for (uint8_t i = 0; i < 8; i++)
    {
        BASE_ADR_SRAM_WRITE_BUFFER[index + i] = pgm_read_byte(&(font8[c - 32][i]));
    }
}

void OLED_clear_buffer(void)
{

    for (uint16_t i = 0; i < PAGES * COLUMNS; i++)
    {
	    BASE_ADR_SRAM_DISPLAY_BUFFER[i] = 0;
		BASE_ADR_SRAM_WRITE_BUFFER[i] = 0;
    }
}

//
 //void OLED_display_user_interface()
 //{
     //// Get the joystick position, and determine where to move in the menu.
     //// Change the page we are pointing to in the OLED display.
     //for (uint8_t i = 0; i < menu.size(); i++)
     //{
         //for (uint8_t j = 0; j < menu[i].label.size(); j++)
         //{
             //OLED_write_char(j, i, menu[i].label[j]);
         //}
         //// Have to increment the page we're on, in the memory mapping.
     //}
 //}
