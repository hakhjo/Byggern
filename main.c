/*
 * GccApplication1.c
 *
 * Created: 31.08.2023 09:03:44
 * Author : haakohjo
 */
#define F_CPU 4915200
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
#include "MCP2515.h"
#include "can.h"

void init_devices()
{
	init_SPI();
	MCP2515_init_loopback();
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
	// uint8_t c;
	// menu_item item1 = {"Item1", *menu_function1};
	// menu_item item2 = {"Item2", *menu_function2};
	// menu_item test_menu[] = {item1, item2};
	// uint8_t menu_len = sizeof(test_menu) / sizeof(test_menu[0]);
	int i = 0;
	printf("Canstat register 0x%x\n\r" , MCP2515_read_register(0x0E));
	
	// Sender melding
	message_t message = {
		1, // Id
		6, //Data length
		"heiiii" //data
	};
	can_send(&message); 

	
	//printf("RXB0CTRL register 0x%x \n\r", MCP2515_read_register(0x60));
	_delay_us(100);
	// Mottar melding
	message_t receive = can_receive();
	printf("melding motatt.\r\n");
	printf("Id: %u \r\n", receive.id);
	printf("Lengde: %d \r\n", receive.length);
	printf("Melding: %s \r\n\r\n", receive.data);

	//while (1)
	//{
		////printf("joystick_position: %d,%d\n", joystick_position.x, joystick_position.y);
		////printf("joystick_direction: %d, \t", joystick_direction);
		////printf("menu index: %d\n\r", menu_index);
		////if (inputs_updated) {
			////inputs_updated = false;
			////navigate_menu(test_menu);
		////}
		////display_menu(test_menu, menu_len, menu_index);
		////
		////c = (readMCP2515Register(0x0E));
		////printf("%d\n\r", c);
		//printf("original value %d, ", MCP2515_read_register(MCP_RXB0D0));
		//MCP2515_write_reg(MCP_TXB0D0, i++);
		//
		//printf("tx value %d, ", MCP2515_read_register(MCP_TXB0D0));
		//MCP2515_request_send(0);
		//printf("new value %d\n\r", MCP2515_read_register(MCP_RXB0D0)); // 0x0E??
		////printf("%x\n\r",SPI_Receive());
		////
		//_delay_ms(500);
	//}
	return 0;
}
