#include "adc.h"

volatile bool adc_busy = false;

ISR(INT2_vect)
{
    // Interrupt Service Routine for INT0
    // Read the ADC value from the MAX156 here

    adc_busy = true;
}

uint8_t read_ADC_value(uint8_t channel)
{
    cli();
    adc_busy = false;
    volatile uint8_t *adc_mem = BASE_ADR_ADC;
    volatile uint8_t config = 0;
    config |= (channel & 0x03) | (1 << 7);
    *adc_mem = config;

    sei();
    // DDRE &= ~(1 << PE2);

    while (!adc_busy)
        ;
    // Wait for busy rising edge

    volatile uint8_t adc_value;
    adc_value = *adc_mem;

    return adc_value;
}
