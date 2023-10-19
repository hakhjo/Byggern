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



void delay(volatile uint32_t count) {
	while(count--) {}
}

void init_PIO(void) {
	PMC->PMC_PCER0 |= 1 << ID_PIOA;
	PMC->PMC_PCER0 |= 1 << ID_PIOB;
	PMC->PMC_PCER0 |= 1 << ID_PIOC;
	PMC->PMC_PCER0 |= 1 << ID_PIOD;
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
	motor_set_dir(1);
		delay(5000000);
		motor_set_speed(2000);
	motor_set_dir(0);
		delay(5000000);
		motor_set_speed(2000);
		printf("cycle");
		
	}
		//read_ball_event();
	//printf("ADC output %d, current score: %d \n\r", ADC->ADC_CDR[0] & 0xFFFu, score);
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
