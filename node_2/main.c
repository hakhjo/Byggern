/*
 * node2.c
 *
 * Created: 12.10.2023 10:15:50
 * Author : haakohjo
 */ 


#include "sam.h"
#include <stdio.h>
#include <string.h>
#include "can_controller.h"
#include "can_interrupt.h"
#include "motorboard.h"
#include "motorbox.h"
#include <stdlib.h>
#define CPU_FREQUENCY 84000000
#define TIMER_FREQUENCY 1000  // 1 kHz or 1 ms intervals
#define TIMER_PRESCALER 128
#define SYSTICK_FREQ       2   // 100 Hz corresponds to 10 ms
#define SYSTICK_COUNT      (SystemCoreClock / SYSTICK_FREQ)

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
	//enable 5 v output
	
	// Disable PIO Control on PC13 and enable direct control by GPIO
	PIOB->PIO_PER = PIO_PER_P25;
	// Set PC13 as an output
	PIOB->PIO_OER = PIO_OER_P25;
	
	//set the push to start low
	PIOB->PIO_SODR = PIO_SODR_P25;
	
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
	PMC->PMC_PCER0 = 1 << ID_TC3;

	//disable and reset TC clock
	TC1->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKDIS |TC_CCR_SWTRG;
	

	// setting clk = clk/32 and trigger on RC compare
	TC1->TC_CHANNEL[0].TC_CMR = TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK3;
	
	// TC = 1 khz
	uint32_t rc = (CPU_FREQUENCY / 32) / TIMER_FREQUENCY;
	TC1->TC_CHANNEL[0].TC_RC = rc;
	
	// Enable interrupts 
	TC1->TC_CHANNEL[0].TC_IER = TC_IER_CPCS;
	TC1->TC_CHANNEL[0].TC_IDR = ~TC_IER_CPCS;
	
	// Start the Timer
	TC1->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
	
	// Enable the interrupt in NVIC
	NVIC_EnableIRQ(TC3_IRQn);
}



void SysTick_Handler(void) {

	// Disable systick interrupt
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	disableSysTick();
	PIOB->PIO_SODR = PIO_SODR_P25;
	//printf("det har gott 10 ms");
	
}

void enableSysTick(void) {
	// Load the SysTick counter value
	SysTick->LOAD  = (SYSTICK_COUNT - 1);

	// Set the priority of SysTick interrupt
	NVIC_SetPriority(SysTick_IRQn, (1 << __NVIC_PRIO_BITS) - 1);

	// Reset the SysTick counter value
	SysTick->VAL   = 0;

	// Enable SysTick IRQ and SysTick Timer
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |SysTick_CTRL_TICKINT_Msk   |SysTick_CTRL_ENABLE_Msk;
}

void disableSysTick(void) {
	// Disable SysTick IRQ and SysTick Timer
	SysTick->CTRL  = 0;
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
	init_TC3();
	
}
int main(void)
{
	init_atsam3x8e();
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
		PIOB->PIO_CODR = PIO_CODR_P25;
		//printf("right button pressed");
		enableSysTick();
	}
	generate_pwm_cycle(right_slider);
	//printf("joystick_position mapped AFTER: %d \n\r", joystick_pos);
	//printf("output : %d, x_dir %d \n\r", (int16_t)output, joystick_pos);
	if(output < 0){
		motor_set_dir(0);
		}else{
		motor_set_dir(1);
	}
	motor_set_speed(abs((int16_t)output));
	read_ball_event();
	//printf("Score: %d \n\r", score);
		char c[8];
		c[0] = (char) score;		
		CAN_MESSAGE message;
		message.id = 0;
		message.data_length = 1;
		memcpy(message.data, c, 8);
		//for(int i = 0;i<message.data_length;i++){
			//printf("%d ", message.data[i]);
		//}
		can_send(&message, 0);
	}

}
