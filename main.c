/*
 * GccApplication1.c
 *
 * Created: 31.08.2023 09:03:44
 * Author : haakohjo
 */
#define F_CPU 16000000
#define FOSC 4915200 // clock speed
#define BAUD 19200
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

void init_devices()
{
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
	printf("FN1")
}

void menu_function2(void)
{
	printf("FN2")
}

int main(void)
{
	menu_item item1 = {"Item1", *menu_function1};
	menu_item item2 = {"Item2", *menu_function2};
	menu_item test_menu[] = {item1, item2};
	uint8_t menu_len = sizeof(test_menu) / sizeof(game_main_menu[0]);

	while (1)
	{
		printf("joystick_position: %d,%d\n\r", joystick_position.x, joystick_position.y);
		printf("left_slider: %d\n\r", left_slider);
		printf("right_slider: %d\n\r", right_slider);

		navigate_menu(test_menu);
		display_menu(test_menu, menu_len, selected);
	}
	return 0;
}
