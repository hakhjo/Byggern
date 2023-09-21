#include "adc.h"

void init_joystick()
{
    x_init_pos = read_ADC_value(X_JOYSTICK);
    y_init_pos = read_ADC_value(Y_JOYSTICK);
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
	
    while (!adc_busy);
    // Wait for busy rising edge

    volatile uint8_t adc_value;
    adc_value = *adc_mem;
	
    return adc_value;
}

int8_t map_input_value(uint8_t adcValue, Button b)
{
    uint8_t adcMax1;
    uint8_t adcMin2;

    switch (b)
    {
    case X_JOYSTICK:
        adcMax1 = x_init_pos;
        adcMin2 = x_init_pos;
        break;
    case Y_JOYSTICK:
        adcMax1 = y_init_pos;
        adcMin2 = y_init_pos;
		break;

    default:
        adcMax1 = 162;
        adcMin2 = 162;
        break;
    }

    // Define the ADC range for the two segments
    const uint8_t adcMin1 = 0;
    const uint8_t adcMax2 = 255;

    // Define the desired range for the two segments
    const int8_t desiredMin1 = -100;
    const int8_t desiredMax1 = 0;
    const int8_t desiredMin2 = 0;
    const int8_t desiredMax2 = 100;

    int16_t tempValue; // Temporary variable for intermediate calculations

    // Determine which segment the adcValue falls into and map accordingly
    if (adcValue <= adcMax1)
    {
        tempValue = ((adcValue - adcMin1) * (desiredMax1 - desiredMin1)) / (adcMax1 - adcMin1) + desiredMin1;
    }
    else
    {
        tempValue = ((adcValue - adcMin2) * (desiredMax2 - desiredMin2)) / (adcMax2 - adcMin2) + desiredMin2;
    }

    return (int8_t)tempValue; // Cast back to int8_t for the final result
}

Pos_t get_joystick_analog_position(void)
{
    Pos_t joystick_pos = {map_input_value(read_ADC_value(X_JOYSTICK), X_JOYSTICK), map_input_value(read_ADC_value(Y_JOYSTICK), Y_JOYSTICK)};
    // printf("xPos: %d, yPos: %d \n\r", joystick_pos.xPos, joystick_pos.yPos);
    return joystick_pos;
}

void print_pos(Pos_t pos) {
	printf("xPos: %d, yPos: %d \n\r", pos.x, pos.y);
}

JoystickDirection getjoystickdirection(Pos_t pos){
    //printf("xPos: %d, yPos: %d \n\r", pos.xPos, pos.yPos);
    if (abs(pos.x) > abs(pos.y)){
        if (pos.x >5){
            return RIGHT;
        }
        else if (pos.x < -5)
        {
            return LEFT;
        }
		else
		{
            return NEUTRAL;
        }
    }
	else
	{
        if (pos.y > 5){
            return UP;
        }
        else if (pos.y < -5)
        {
            return DOWN;
        }
		else
		{
            return NEUTRAL;
        }
    }
}