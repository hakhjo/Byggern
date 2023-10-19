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



void delay(volatile uint32_t count) {
	while(count--) {}
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
}
int main(void)
{
	init_atsam3x8e();
	while(1){
		read_adc();
	printf("ADC output %d, current score: %d \n\r", ADC->ADC_CDR[0] & 0xFFFu, score);
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
