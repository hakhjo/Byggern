/*
 * node2.c
 *
 * Created: 12.10.2023 10:15:50
 * Author : haakohjo
 */ 


#include "sam.h"
#include "can_controller.h"
#include "can_interrupt.h"
#include "motorboard.h"
#include "motorbox.h"
#include <stdlib.h>



void delay(volatile uint32_t count) {
	while(count--) {}
}

void init_PIO(void) {
	PMC->PMC_PCER0 |= 1 << ID_PIOA;
	PMC->PMC_PCER0 |= 1 << ID_PIOB;
	PMC->PMC_PCER0 |= 1 << ID_PIOC;
	PMC->PMC_PCER0 |= 1 << ID_PIOD;
	
	// Encoder signals enters at PIN33-PIN40 on the SAM3X, and enabling input.
	PIOC->PIO_ODR |= (PIO_PC8 | PIO_PC7 | PIO_PC6 | PIO_PC5 | PIO_PC4 | PIO_PC3 | PIO_PC2 | PIO_PC1 );
	//PIOC ->PIO_OER |= PIO_PC12;
	
	// Disable PIO Control on PC13 and enable direct control by GPIO
	PIOC->PIO_PER = PIO_PER_P12;
	// Set PC13 as an output
	PIOC->PIO_OER = PIO_OER_P12;
	
	//set the push to start low
	PIOC->PIO_SODR = PIO_SODR_P12;
	
}

uint16_t map_joystick(int8_t joystick_position){
	return (joystick_position - (-100.0))/(100-(-100)) * (1404 -0) + 0;
}

void init_DAC(void) {
	PMC->PMC_PCER1 |= PMC_PCER1_PID38;
	DACC->DACC_MR = DACC_MR_USER_SEL_CHANNEL1 | DACC_MR_WORD_HALF;
	DACC->DACC_CHER = DACC_CHER_CH1;
	
	PIOB->PIO_ABSR |= PIO_ABSR_P16;
	PIOB->PIO_PDR |= PIO_PDR_P16;
}

void disable_watchdog(void){
	WDT->WDT_MR = WDT_MR_WDDIS;  
}

void configure_systick(void) {
	SysTick->LOAD = 0xFFFFFF; // Set reload register
	SysTick->VAL = 0; // Reset the SysTick counter value
	SysTick->CTRL |= 1 | (1<<2);
}

void init_atsam3x8e(void){
	SystemInit();
	disable_watchdog();
	configure_uart();
	init_PWM();
	can_init(can_br_value,1,1);
	init_ADC();
	init_PIO();
	init_DAC();
	init_motorbox();
}
int main(void)
{
	init_atsam3x8e();
	
	motor_enable();
	delay(10000);
	printf("enabled");
	while(1){
	//printf("joystick_position mapped: %d \n\r", map_joystick(x_dir));
	uint16_t joystick_pos = map_joystick(x_dir);
	//printf("joystick_position mapped: %d \n\r", joystick_pos);
	double output = PI_controller(read_encoder(), joystick_pos);
	//printf("joystick_position mapped AFTER: %d \n\r", joystick_pos);
	//printf("output : %d, x_dir %d \n\r", (int16_t)output, joystick_pos);
	if(output < 0){
		motor_set_dir(0);
		}else{
		motor_set_dir(1);
	}
	motor_set_speed(abs((int16_t)output));
	
	//printf("Encoder value:  %d \n\r", read_encoder());	
			//read_ball_event();
			//printf("ADC output %d, current score: %d \n\r", ADC->ADC_CDR[0] & 0xFFFu, score);
	}

    /* Initialize the SAM system */
	
	//printf("initializing");
	//
	///* Enable the peripheral clock for the LED's PIO */
	//PMC->PMC_PCER0 = 1 << ID_PIOA;
//
	 ///* Configure LED_PIN as output */
	 //PIOA->PIO_OER |= PIO_PA19;
	 //PIOA->PIO_PER |= PIO_PA19;
	  //PIOA->PIO_OER |= PIO_PA20;
	  //PIOA->PIO_PER |= PIO_PA20;
//PIOA->PIO_SODR = PIO_PA19;  // Set the pin, turning the LED on
//delay(1000000);
//
//PIOA->PIO_CODR = PIO_PA19;  // Clear the pin, turning the LED off
//delay(1000000);
     
}
