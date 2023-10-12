/*
 * node2.c
 *
 * Created: 12.10.2023 10:15:50
 * Author : haakohjo
 */ 


#include "sam.h"
#include "can_controller.h"
void delay(volatile uint32_t count) {
	while(count--) {}
}

int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	WDT->WDT_MR = WDT_MR_WDDIS;  
	configure_uart();
	can_init(can_br_value,1,1);
	
	/* Enable the peripheral clock for the LED's PIO */
	PMC->PMC_PCER0 = 1 << ID_PIOA;

	 /* Configure LED_PIN as output */
	 PIOA->PIO_OER |= PIO_PA19;
	 PIOA->PIO_PER |= PIO_PA19;
	  PIOA->PIO_OER |= PIO_PA20;
	  PIOA->PIO_PER |= PIO_PA20;
PIOA->PIO_SODR = PIO_PA19;  // Set the pin, turning the LED on
delay(1000000);

PIOA->PIO_CODR = PIO_PA19;  // Clear the pin, turning the LED off
delay(1000000);
     
}
