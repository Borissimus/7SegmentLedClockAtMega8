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
#define NUM_OF_DIGITS 6 // 7Led display digits quantity
// 7Seg display type: Common Anode 1 or Common Cathode 0
#define COMMON_ANODE 1
#define COMMON_CATHODE 0
// Display data source defines
#define GLOBAL_TIME 1   // GlobalTimeData is selected as CuurrSourceData
#define SETTING_TIME 0  // SettingTimeData is selected as CuurrSourceData 

// current LED (Digit) selector port definition
// display selector pins must be continuously set from any port (PINB1 - PINB6 for example) 
typedef struct DigitPointerPort {
	volatile uint8_t *port;
	uint8_t StatrPinNum;
} DigitPointerPort;

typedef struct Led7SegDispConfig {
	
} Led7SegDispConfig;

// Pre define Display prototype 
typedef struct Led7SegDispProto Display;

typedef struct Led7SegDispProto {
	uint8_t CurrSoucreData;              // disp. src selector GLOBAL_TIME or SETTING_TIME
	uint8_t CurrDigitNum;                // currently displaying digit number
	uint8_t NumOfDigits;                 // must be assigned to NUM_OF_DIGITS  
	uint8_t Digit[NUM_OF_DIGITS];        // display of digits array (display current data) 
	DigitPointerPort *CurrDigitPointer;  // struct used for setup cur. digit displaying and definition
	void(*SwitchDigit)(Display *disp);   // Update displayed digit by modifying *CurrDigitPointer 
	pin double_point;                    // seconds marker - double point (pin object)
	Led7SegDispConfig config;
} Display;