#include "interrupt.h"

void init_interrupts(void)
{
    // Configure INT0, INT1 and INT2 for rising edge detection
    MCUCR |= (1 << ISC01) | (1 << ISC00);
    MCUCR |= (1 << ISC11) | (1 << ISC10);
    EMCUCR |= (1 << ISC2);
    // Enable external interrupt INT0, INT1 and INT2
    GICR |= (1 << INT0);
    GICR |= (1 << INT1);
    GICR |= (1 << INT2);

    sei(); // enable interrupts
}

// Initialize the Timer1 to interrupt at 60 Hz
void init_OLED_timer(void)
{
    // Set initial timer value
    TCNT1 = 0;
    // Set compare value
    OCR1A = 320 * 2;
    // Configure CTC mode and with a prescaler of 256
    TCCR1A = 0;
    TCCR1B = (1 << WGM12) | (1 << CS12);
    // Enable
    TIMSK = (1 << OCIE1A);
}

// Initialize Timer3 for a 60 Hz interrupt
void init_input_timer()
{
    // Set initial timer value
    TCNT3 = 0;
    // Set compare value
    OCR3A = 520;
    // Configure CTC mode and with a prescaler of 256
    TCCR3A = 0;
    TCCR3B = (1 << WGM32) | (1 << CS32) | (1 << CS30);
    // Enable
    ETIMSK = (1 << OCIE3A);
}
