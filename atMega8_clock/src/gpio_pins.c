/*
 * gpio_pins.c
 *
 * Created: 11.10.2020 16:36:07
 *  Author: Borissimus
 */ 
#include "../inc/gpio_pins.h"


// declare inputs
pin *SSBTN, *DirBTN, *SlowerBTN, *FasterBTN;
// declare outputs
pin *StatusLED, *FwdLED, *RvsLED, *StepDRV, *DirDRV, *EnDRV;

void pin_init(pin *pin, volatile uint8_t *port, volatile uint8_t *port_dir, volatile uint8_t *pin_reg, uint8_t pin_num) {
	pin->port = port;
	pin->port_dir = port_dir;
	pin->pin_reg = pin_reg;
	pin->pin_num = pin_num;
}

void pin_config(pin *pin, const uint8_t conf_type) {
	if (conf_type == PIN_AS_OUTPUT) {
		*pin->port_dir |= (1 << pin->pin_num);
		} else if (conf_type == PIN_AS_INPUT) {
		*pin->port_dir &= ~(1 << pin->pin_num);
		} else if (conf_type == PIN_AS_PULLED_IN) {
		*pin->port_dir &= ~(1 << pin->pin_num);
		*pin->port |= (1 << pin->pin_num);
	}
}

// set bit to "1"
void set_bit(pin *pin){
	*pin->port |= (1 << pin->pin_num);
}

// clear bit to "0"
void clear_bit(pin *pin){
	*pin->port &= ~(1 << pin->pin_num);
}

// get pin value
uint8_t get_pin_state (pin *pin) {
	return *pin->pin_reg & (1 << pin->pin_num);
}

void all_pins_init(void) {
	pin_init(SSBTN, &PORTC, &DDRC, &PINC, PINC3);
	pin_init(StatusLED, &PORTC, &DDRC, &PINC, PINC2);
	// add other pins inits ....
}

void all_pins_config(void) {
	pin_config(SSBTN, PIN_AS_INPUT);
	pin_config(StatusLED, PIN_AS_OUTPUT);
	// add other pins configs ...
}
