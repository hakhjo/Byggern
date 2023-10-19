#include "motorbox.h"


void init_motorbox(void) {
	PIOD->PIO_OER |= PIO_PD10; // motor enable
	PIOD->PIO_PER |= PIO_PD10;
	PIOD->PIO_OER |= PIO_PD9; // motor dir
	PIOD->PIO_PER |= PIO_PD9;
}

void motor_enable(void) {
	PIOD->PIO_SODR = PIO_PD9;  // Set the pin, turning the LED on
}

void motor_disable(void) {
	PIOD->PIO_CODR = PIO_PD9;  // Clear the pin, turning the LED off
}

void motor_set_dir(uint8_t dir) {
	if (dir) {
		PIOD->PIO_SODR = PIO_PD10;
	} else {
		PIOD->PIO_CODR = PIO_PD10;
	}
}

void motor_set_speed(uint16_t speed) {
	//while (!(DACC->DACC_ISR & DACC_ISR_TXRDY));
	DACC->DACC_CDR = speed;
}