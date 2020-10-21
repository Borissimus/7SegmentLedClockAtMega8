/*
 * _7LedDisplay.h
 *
 * Created: 12.10.2020 10:20:54
 *  Author: Borissimus
 */ 
#ifndef _7LEDDISPLAY_H_
#define _7LEDDISPLAY_H_
#endif /* 7LEDDISPLAY_H_ */
// User includes
#include <stdint.h>

// User defines
#define DIGIT_DATA_PORT &PORTC
#define DIGIT_DATA_PORT_DIR &DDRC

#define DIGIT_PTR_PORT &PORTD
#define DIGIT_PTR_PORT_DIR &DDRD
#define START_DIGIT_PIN PIND0	  // digit pointer port start
#define END_DIGIT_PIN PIND5       // digit pointer port end
#define NUM_OF_DIGITS (END_DIGIT_PIN - START_DIGIT_PIN + 1) // 7Led display digits quantity
#define PORT_SIZE 8				  // GPIO port size depend on MCU architecture 
// 7Seg display type: Common Anode 1 or Common Cathode 0
#define COMMON_ANODE   1
#define COMMON_CATHODE 0
// Display data source defines
#define TIME_MODE  1   // GlobalTimeData is selected as CuurrSourceData
#define SETUP_MODE 0   // SettingTimeData is selected as CuurrSourceData

typedef union Led7SegDigitProto {
	uint8_t dec_val;
	struct bit_val {
		uint8_t A  : 1;  // PIN0 of DIGIT_DATA_PORT
		uint8_t B  : 1;	 //	PIN1 of DIGIT_DATA_PORT
		uint8_t C  : 1;	 //	PIN2 of DIGIT_DATA_PORT
		uint8_t D  : 1;	 //	PIN3 of DIGIT_DATA_PORT
		uint8_t E  : 1;	 //	PIN4 of DIGIT_DATA_PORT
		uint8_t F  : 1;	 //	PIN5 of DIGIT_DATA_PORT
		uint8_t G  : 1;	 //	PIN6 of DIGIT_DATA_PORT
		uint8_t DP : 1;	 //	PIN7 of DIGIT_DATA_PORT
	};
} Led7SegDigitProto;
// current LED (Digit) selector port definition
// display selector pins must be continuously set from any port (PINB1 - PINB6 for example) 
typedef struct DisplayDigitPointer {
	volatile uint8_t active_digit_mask[NUM_OF_DIGITS];
	volatile uint8_t mask;
	volatile uint8_t inv_mask;
	volatile uint8_t *port;
	volatile uint8_t *port_dir;
	uint8_t start_pin_num;
	uint8_t end_pin_num;
} DisplayDigitPointer;

typedef struct DisplayDigitsData {
	Led7SegDigitProto Data[NUM_OF_DIGITS];  // display of digits array (display current data)
	volatile uint8_t mask;
	volatile uint8_t inv_mask;
	volatile uint8_t *port;
	volatile uint8_t *port_dir;
} DisplayDigitsData;

typedef struct Led7SegDispConfig {
   
} Led7SegDispConfig;

// Pre define Display prototype 
typedef struct Led7SegDispProto Display;

typedef struct Led7SegDispProto {
	uint8_t curr_digit_num;                // currently displaying digit number
	DisplayDigitPointer DigitPointer;       // struct used for setup cur. digit displaying and definition
	uint8_t disp_type;
	uint8_t curr_src_data;              // disp. src selector GLOBAL_TIME or SETTING_TIME
	uint8_t num_of_digits;                 // must be assigned to NUM_OF_DIGITS   
	
	DisplayDigitsData Digits;
	void(*SwitchDigit)(Display *disp);   // Update displayed digit by modifying *CurrDigitPointer 
	
	pin double_point;                    // seconds marker - double point (pin object)
	void (*init)(Display *disp);
} Display;