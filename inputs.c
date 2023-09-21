#include "inputs.h"



void update_global_inputs(){
    joystick_position = get_joystick_analog_position();
    left_slider = read_ADC_value(LEFT_SLIDER);
    right_slider = read_ADC_value(RIGHT_SLIDER);
	update_selected();
}

void update_selected() {
	
	if (joystick_position.y == -100 && lastJoystickY != -100) {
		selected++;
		printf("lastY %d\n\r", lastJoystickY);	
	}
	else if(joystick_position.y == 100 && lastJoystickY != 100){
		selected --;
	}
	lastJoystickY = joystick_position.y;
}