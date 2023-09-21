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
#include "sram.h"
#include "interrupt.h"
#include "adc.h"
#include "OLED.h"

// flags
void xmem_init(void);

void xmem_init(void)
{
	MCUCR |= 1 << SRE;
	SFIOR |= 1 << XMM0;
}

int main(void)
{
  
  volatile uint8_t *dbuf = (uint8_t*)0x1C00;
  volatile uint8_t *wbuf = (uint8_t *)0x1800;
	init_external_memory();
	init_interrupts();
	init_USART(UBRR);
	init_joystick();
	init_OLED();
	//timer1_init();
	 //OLED_write_char('a');
	 OLED_clear_buffer();
	 printf("Cleared: ");
	 printf("%d, ", wbuf[0]);
	 printf("%d\n\r", dbuf[0]);
	 OLED_write_char(0, 0, 'Q');
	 printf("Written: ");
	 printf("%d, ", wbuf[0]);
	 printf("%d\n\r", dbuf[0]);
	 OLED_stage_buffer();
	 printf("Staged: ");
	 printf("%d, ", wbuf[0]);
	 printf("%d\n\r", dbuf[0]);
	 OLED_display_buffer();
	//SRAM_test();
	//while(1) {
  	//OLED_clear_buffer();
  	//OLED_write_char(0, 0, 'Q');
  	//OLED_stage_buffer();
  	//OLED_display_buffer();
  //}
	return 0;
}
