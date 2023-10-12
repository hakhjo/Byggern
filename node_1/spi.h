#pragma once
#include <avr/io.h>
#include <avr/delay.h>

// Define the CS pin for MCP2515
#define SPI_READ_COMMAND 0x03
#define SPI_RESET_COMMAND 0xC0

void init_SPI(void);
uint8_t SPI_transmit(char cData);
uint8_t SPI_receive(void);