#pragma once
#include <avr/io.h>
#include <avr/delay.h>

void init_spi(void);
void SPI_Transmit(char cData);
uint8_t SPI_Receive(void);
void MCP2515_Select(void);
void MCP2515_Deselect(void);
uint8_t readMCP2515Register(uint8_t address);
void MCP2515_Init_Loopback();
void MCP2515_WriteRegister(uint8_t address, uint8_t data);