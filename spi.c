#include "spi.h"


void init_SPI(void){
    /* Set MOSI and SCK and SS output, all others input */
    DDRB |= (1<<DDB5)|(1<<DDB7)|(1<<DDB4);
    // Enable SPI, Master, set clock rate fck/16(SPRO) and setting SPIMODE = 0 since CPOL and CPHA are set to 0
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);

}

uint8_t SPI_transmit(char cData)
{
	/* Start transmission */
	SPDR = cData;
	
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}

uint8_t SPI_receive(void) {
	// Send dummy byte to initiate reception
	SPI_transmit(0xFF);
	return SPDR;
}