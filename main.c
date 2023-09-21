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
#include "sram.h"
#include "interrupt.h"
#include "adc.h"
#include "OLED.h"
#include "inputs.h"
// flags
void xmem_init(void);

void xmem_init(void)
{
	MCUCR |= 1 << SRE;
	SFIOR |= 1 << XMM0;
}


int main(void)
{
	init_external_memory();
	init_interrupts();
	init_USART(UBRR);
	init_joystick();
	_delay_ms(100);
	init_OLED();
	timer1_init();
	Timer3_init();
	OLED_clear_buffer();
	menu_item new_game = {"New game", *OLED_new_game};
	menu_item highscore = {"View highscore", *OLED_new_game};
	menu_item game_main_menu[] = {new_game, highscore};
	uint8_t menu_len = sizeof(game_main_menu)/sizeof(game_main_menu[0]);
	
	while(1){
		//printf("joystick_position: %d,%d\n\r", get_joystick_analog_position().x, get_joystick_analog_position().y);
		printf("joystick_position: %d,%d\n\r", joystick_position.x, joystick_position.y);
		printf("left_slider: %d\n\r", left_slider);
		printf("right_slider: %d\n\r", right_slider);
		printf("selected %d\n\r", selected);
		OLED_display_menu(game_main_menu, menu_len, selected);
		// delay to allow time for other operations
		// or to match the print rate with the ISR rate
		_delay_ms(100);
	}
	// int i = 0;
	// 	while(1) {
	// 		OLED_write_char(i++ % 10,0,'a');
	// 		OLED_write_char(0,1,'q');
	// 		_delay_ms(101);
	//   }
	return 0;
}
