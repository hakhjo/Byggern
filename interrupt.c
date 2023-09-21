#include "interrupt.h"

volatile bool adc_busy = false;
void init_interrupts(void)
{
    // Configure INT0 for rising edge detection
    MCUCR |= (1 << ISC01) | (1 << ISC00);
    MCUCR |= (1 << ISC11) | (1 << ISC10);
    EMCUCR |= (1 << ISC2);
    // Enable external interrupt INT0
    GICR |= (1 << INT0);
    GICR |= (1 << INT1);
    GICR |= (1 << INT2);

    sei(); // enable global interrupt
}

ISR(INT0_vect)
{
     printf("BTN R\n\r");
     //OLED_write_char('R');
}

ISR(INT1_vect)
{
     printf("BTN L\n\r");
     //OLED_write_char('L');
}

ISR(INT2_vect)
{
      //Interrupt Service Routine for INT0
      //Read the ADC value from the MAX156 here

    adc_busy = true;
}
// Initialize the Timer1 to interrupt at 60 Hz
void timer1_init(void)
{
    // Set initial timer value
    TCNT1 = 0;

    // Calculate the value for the Output Compare Register
    // For example, with a prescaler of 256 and 8 MHz system clock:
    // (8,000,000 / 256) / 60 = 520.83 (approx.)
    // OCR value = 520
    OCR1A = 320 / 60;

    // Configure Timer1 in CTC mode (Clear Timer on Compare match)
    // and with a prescaler of 256.
    TCCR1A = 0;
    TCCR1B = (1 << WGM12) | (1 << CS12);

    // Enable the Output Compare A interrupt
    TIMSK = (1 << OCIE1A);
}

// Timer1 ISR
ISR(TIMER1_COMPA_vect)
{
    //printf("Hello\r\n");
}