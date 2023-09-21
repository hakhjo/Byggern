#pragma once

#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

void init_USART( unsigned int ubrr );

int USART_transmit(char data, FILE* file);
int USART_receive(FILE* file );
