#include "motorboard.h"

uint8_t score = 0;

void init_PWM(){
	PMC->PMC_PCER1 |= PMC_PCER1_PID36; //PWM ligger på pin 4 evt | PMC_PCER1_PID36
	PIOC->PIO_ABSR |= (1<<19) | (1<<18);
	PWM->PWM_CH_NUM[5].PWM_CMR |= (PWM_CMR_CPRE_MCK_DIV_1024 | (1<<9)); //since CPOL er reg 9
	PWM->PWM_CH_NUM[5].PWM_CPRD = 1640;
	PWM->PWM_CH_NUM[5].PWM_CDTY = 74;
	PWM->PWM_ENA = PWM_ENA_CHID5;
	PIOC->PIO_PDR |= PIO_PDR_P19;  // enable peripheral control on PC26 (Pin 44)
	PIOC->PIO_PDR |= PIO_PDR_P18;
}

void generate_pwm_cycle(int8_t value){
	//må ligge mellom 74 og 172
	PWM->PWM_CH_NUM[5].PWM_CDTY = (value -(-100))*(172-74)/(100-(-100)) + 74;
}

void init_ADC(void) {
	
	PMC->PMC_PCER1 |= PMC_PCER1_PID37; // ADC peripheral id = 37;
	
	//// This register can only be written if the WPEN bit is cleared
	//ADC->ADC_WPMR &= 0b0;
	//
	// Reset ADC
	ADC->ADC_CR = ADC_CR_SWRST;
	
	// Configure ADC: 12-bit resolution, free run on, disable trigger selection.
	ADC->ADC_MR = ADC_MR_LOWRES_BITS_12 | ADC_MR_FREERUN_ON | ADC_MR_TRGEN_DIS;
	ADC->ADC_CHER = ADC_CHDR_CH0;
	PIOA->PIO_ABSR |= (1<<2);
	PIOA->PIO_PDR |= PIO_PDR_P2;  // enable peripheral control on PC26 (Pin 44)
	
	
	// How to set TRACKTIM, and TRANSFER on the ADC. See p.1333
	 
	
}

void read_ball_event(){
	// Wait for the conversion to complete for the channel
	while (!(ADC->ADC_ISR & (1 << 0)));
	static uint16_t previous_value;
	
	 uint16_t value = ADC->ADC_CDR[0];
	 if(value < 1000 && previous_value >= 500){
		 score++;
	 }
	 previous_value = value;
}