/*
 * GccApplication1.c
 *
 * Created: 31.08.2023 09:03:44
 * Author : haakohjo
 */
#define F_CPU 16000000
#define FOSC 4915200 // clock speed
#define BAUD 9600
#define UBRR FOSC / 16 / BAUD - 1

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>

#include "uart.h"
#include "external_memory.h"
#include "interrupt.h"
#include "adc.h"
#include "OLED.h"
#include "menu.h"
#include "spi.h"

void init_devices()
{
	init_spi();
	MCP2515_Init_Loopback();
	//MCP2515_Init();
	init_external_memory();
	init_interrupts();
	init_USART(UBRR);
	init_joystick();
	init_OLED();
	init_input_timer();
	init_OLED_timer();
}

void menu_function1(void)
{
	printf("FN1");
}

void menu_function2(void)
{
	printf("FN2");
}

int main(void)
{
	init_devices();
	uint8_t c;
	menu_item item1 = {"Item1", *menu_function1};
	menu_item item2 = {"Item2", *menu_function2};
	menu_item test_menu[] = {item1, item2};
	uint8_t menu_len = sizeof(test_menu) / sizeof(test_menu[0]);
	while (1)
	{
		//printf("joystick_position: %d,%d\n", joystick_position.x, joystick_position.y);
		//printf("joystick_direction: %d, \t", joystick_direction);
		//printf("menu index: %d\n\r", menu_index);
		//if (inputs_updated) {
			//inputs_updated = false;
			//navigate_menu(test_menu);
		//}
		//display_menu(test_menu, menu_len, menu_index);
		//
		//c = (readMCP2515Register(0x0E));
		//printf("%d\n\r", c);
		printf("original value %d\n\r", readMCP2515Register(0x36));
		MCP2515_WriteRegister(0x36, 5);
		printf("new value %d\n\r", readMCP2515Register(0x0E));
		//printf("%x\n\r",SPI_Receive());
		//
		//_delay_ms(1);
	}
	return 0;
}
