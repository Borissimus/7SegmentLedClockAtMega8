/*
 * gpio_pins.h
 *
 * Created: 11.10.2020 16:34:31
 *  Author: Borissimus
 */ 
#ifndef GPIO_PINS_H_
#define GPIO_PINS_H_
#endif /* GPIO_PINS_H_ */
#include <avr/io.h>
#include <stdint.h>

// pin configure type
#define PIN_AS_OUTPUT 1
#define PIN_AS_INPUT 2
#define PIN_AS_PULLED_IN 3

typedef struct PinProto {
	volatile uint8_t *port;
	volatile uint8_t *port_dir;
	volatile uint8_t *pin_reg;
	uint8_t pin_num;
} pin;

