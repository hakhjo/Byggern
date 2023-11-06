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
#define CPU_FREQUENCY 84000000
#define TIMER_FREQUENCY 1000  // 1 kHz or 1 ms intervals
#define TIMER_PRESCALER 128
#define TIMER_FREQUENCY_10MS 100  // 100 Hz for 10 ms intervals

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



void init_TC3(void) {
	// Enable the peripheral clock for TC3 (TC1 channel 0)
	PMC->PMC_PCER0 = 1 << ID_TC3;

	// Disable TC clock
	TC1->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKDIS;
	
	// Reset TC
	TC1->TC_CHANNEL[0].TC_CCR = TC_CCR_SWTRG;
	
	// Set Mode: Waveform Mode, Clock/32, UP mode with automatic trigger on RC Compare
	TC1->TC_CHANNEL[0].TC_CMR = TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK3;
	
	// Configure TC for a 1ms (1kHz) rate
	uint32_t rc = (CPU_FREQUENCY / 32) / TIMER_FREQUENCY;
	TC1->TC_CHANNEL[0].TC_RA = rc/2; // 50% duty cycle
	TC1->TC_CHANNEL[0].TC_RC = rc;
	
	// Enable interrupts on RC Compare
	TC1->TC_CHANNEL[0].TC_IER = TC_IER_CPCS;
	TC1->TC_CHANNEL[0].TC_IDR = ~TC_IER_CPCS;
	
	// Start the Timer
	TC1->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
	
	// Enable the interrupt in the Nested Vector Interrupt Controller (NVIC)
	NVIC_EnableIRQ(TC3_IRQn);
}

void configure_systick(void) {
	SysTick->LOAD = 0xFFFFFF; // Set reload register
	SysTick->VAL = 0; // Reset the SysTick counter value
	SysTick->CTRL |= 1 | (1<<2);
}

void TC7_Handler(void) {
	// Clear status bit to acknowledge interrupt for TC2 channel 0
	printf("hei");
	volatile uint32_t tmp = TC1->TC_CHANNEL[1].TC_SR;  // Read status register to clear interrupt status
	//TC2->TC_CHANNEL[2].TC_CCR = TC_CCR_CLKDIS;
	//PIOC->PIO_SODR = PIO_SODR_P12;
	// Your additional code for the interrupt goes here
}
void init_TC2_10ms(void) {
// Enable the peripheral clock for TC3 (TC1 channel 0)
PMC->PMC_PCER0 |= 1 << ID_TC7;

// Disable TC clock
TC1->TC_CHANNEL[1].TC_CCR = TC_CCR_CLKDIS;

// Reset TC
TC1->TC_CHANNEL[1].TC_CCR = TC_CCR_SWTRG;

// Set Mode: Waveform Mode, Clock/32, UP mode with automatic trigger on RC Compare
TC1->TC_CHANNEL[1].TC_CMR = TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK3;

// Configure TC for a 1ms (1kHz) rate
uint32_t rc = (CPU_FREQUENCY / 32) / TIMER_FREQUENCY;
TC1->TC_CHANNEL[1].TC_RA = rc/2; // 50% duty cycle
TC1->TC_CHANNEL[1].TC_RC = rc;

// Enable interrupts on RC Compare
TC1->TC_CHANNEL[1].TC_IER = TC_IER_CPCS;
TC1->TC_CHANNEL[1].TC_IDR = ~TC_IER_CPCS;

// Start the Timer
TC1->TC_CHANNEL[1].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;

// Enable the interrupt in the Nested Vector Interrupt Controller (NVIC)
NVIC_EnableIRQ(TC7_IRQn);

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
	configure_systick();
	init_TC3();
	init_TC2_10ms();
}
int main(void)
{
	init_atsam3x8e();
	__enable_irq();
	motor_enable();
	delay(10000);
	printf("enabled");
	while(1){
	//printf("joystick_position mapped: %d \n\r", map_joystick(x_dir));
	joystick_pos = map_joystick(-x_dir);
	encoder_pos = read_encoder();
	//printf("encoder_pos: %d \n\r",encoder_pos);
	if(r_btn){
		r_btn = 0;
		PIOC->PIO_CODR = PIO_CODR_P12;
		//TC2->TC_CHANNEL[2].TC_CCR = TC_CCR_CLKDIS;

	}
	
	//printf("joystick_position mapped AFTER: %d \n\r", joystick_pos);
	//printf("output : %d, x_dir %d \n\r", (int16_t)output, joystick_pos);
	if(output < 0){
		motor_set_dir(0);
		}else{
		motor_set_dir(1);
	}
	motor_set_speed(abs((int16_t)output));
	//printf("output: %d \n\r", output);
	
	//printf("Encoder value:  %d \n\r", read_encoder());	
			//read_ball_event();
			//printf("ADC output %d, current score: %d \n\r", ADC->ADC_CDR[0] & 0xFFFu, score);
	}

}
