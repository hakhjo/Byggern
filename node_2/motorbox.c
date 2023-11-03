#include "motorbox.h"
double error_sum = 0;

void init_motorbox(void) {
	PIOD->PIO_OER |= PIO_PD10; // motor enable
	PIOD->PIO_PER |= PIO_PD10;
	PIOD->PIO_OER |= PIO_PD9; // motor dir
	PIOD->PIO_PER |= PIO_PD9;
}

void motor_enable(void) {
	PIOD->PIO_SODR = PIO_PD9;  // Set the pin, turning the MOTOR on
}

void motor_disable(void) {
	PIOD->PIO_CODR = PIO_PD9;  // Clear the pin, turning the MOTOR off
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

uint16_t read_encoder(void){
	
	uint16_t encoder_value = 0;
	
	// Have to set MJ1 !OE low. This will sample and hold the encoder value.
	PIOD->PIO_OER |= (PIO_PD0 | PIO_PD2);
	PIOD->PIO_CODR = (PIO_PD0 | PIO_PD2);
	
	// Delay 20ms (ca.)
	volatile int i = 0;
	while(i < 2000) {i++;}
	
	// Encoder signals enters at PIN33-PIN40 on the SAM3X, and enabling input. DONE IN INIT FUNC.
	// Read MJ2 to get high byte
	
	uint32_t port_state = PIOC ->PIO_PDSR;
	
	
	//encoder_value |= (( PIO_PC8 | PIO_PC7 | PIO_PC6 | PIO_PC5 | PIO_PC4 | PIO_PC3 | PIO_PC2 | PIO_PC1 ) << 8)
	encoder_value |= (((port_state>>1)&0xFF) << 8);
	//printf("encoder value high: %d \n\r", encoder_value);
	// Set SEL high to get output low byte.
	PIOD->PIO_SODR = PIO_PD2;
	
	// Delay 20ms (ca.)
	i = 0;
	while(i < 2000) {i++;}
	
	//encoder_value |= (( PIO_PC8 | PIO_PC7 | PIO_PC6 | PIO_PC5 | PIO_PC4 | PIO_PC3 | PIO_PC2 | PIO_PC1 ));
	port_state = PIOC ->PIO_PDSR;
	encoder_value |= (((port_state>>1)&0xFF));
	
	PIOD->PIO_SODR = PIO_PD0;
	
	return encoder_value;
}

double PI_controller(uint16_t encoder_value, uint16_t joystick_value){
		double Kp = 1;
		double Ki = 0.01;
		double T = SysTick->VAL/840000;
		double error = (double)joystick_value - encoder_value;
		printf("error_sum: %d \n\r ", (int)error_sum);
		SysTick->VAL = 0;
		error_sum = error_sum + T*error;
		return Kp*error + Ki*error_sum;
		//denne funker ikke
}		