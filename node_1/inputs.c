#include "inputs.h"


volatile bool inputs_updated = false;

void init_joystick()
{
	joystick_offset.x = read_ADC_value(X_JOYSTICK);
	joystick_offset.y = read_ADC_value(Y_JOYSTICK);
}

void update_global_inputs()
{
	prev_joystick_direction = joystick_direction;
	joystick_position = read_mapped_joystick_position();
	left_slider = read_ADC_value(LEFT_SLIDER);
	right_slider = read_ADC_value(RIGHT_SLIDER);
	joystick_direction = get_joystick_direction(joystick_position);
	inputs_updated = true;
}

int8_t map_input_value(uint8_t raw_value, InputDevice device)
{
	uint8_t adcMax1;
	uint8_t adcMin2;

	switch (device)
	{
	case X_JOYSTICK:
		adcMax1 = joystick_offset.x;
		adcMin2 = joystick_offset.x;
		break;
	case Y_JOYSTICK:
		adcMax1 = joystick_offset.y;
		adcMin2 = joystick_offset.y;
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

	int16_t result; // Temporary variable for intermediate calculations

	// Determine which segment the adcValue falls into and map accordingly
	if (raw_value <= adcMax1)
	{
		result = ((raw_value - adcMin1) * (desiredMax1 - desiredMin1)) / (adcMax1 - adcMin1) + desiredMin1;
	}
	else
	{
		result = ((raw_value - adcMin2) * (desiredMax2 - desiredMin2)) / (adcMax2 - adcMin2) + desiredMin2;
	}

	return (int8_t)result; // Cast back to int8_t for the final result
}

JoystickPosition read_mapped_joystick_position(void)
{
	JoystickPosition position;
	position.x = map_input_value(read_ADC_value(X_JOYSTICK), X_JOYSTICK);
	position.y = map_input_value(read_ADC_value(Y_JOYSTICK), Y_JOYSTICK);

	return position;
}

void print_pos(JoystickPosition pos)
{
	printf("xPos: %d, yPos: %d \n\r", pos.x, pos.y);
}

JoystickDirection get_joystick_direction(JoystickPosition pos)
{
	// printf("xPos: %d, yPos: %d \n\r", pos.xPos, pos.yPos);
	if (abs(pos.x) > abs(pos.y))
	{
		if (pos.x > 5)
		{
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
		if (pos.y > 5)
		{
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

ISR(INT0_vect)
{
	printf("BTN R\n\r");
}

ISR(INT1_vect)
{
	printf("BTN L\n\r");
}

ISR(TIMER3_COMPA_vect)
{
	update_global_inputs();
}