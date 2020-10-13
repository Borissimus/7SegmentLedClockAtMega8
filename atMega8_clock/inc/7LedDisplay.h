/*
 * _7LedDisplay.h
 *
 * Created: 12.10.2020 10:20:54
 *  Author: Borissimus
 */ 
#ifndef _7LEDDISPLAY_H_
#define _7LEDDISPLAY_H_
#endif /* 7LEDDISPLAY_H_ */
#include <stdint.h>

#define NUM_OF_DIGITS 6

typedef struct DigitPointerPort {
	uint8_t CurrPtrMask;
	volatile uint8_t *port;
	uint8_t StatrPinNum;
} DigitPointerPort;

typedef struct Led7SegDispProto Display;

typedef struct Led7SegDispProto {
	uint8_t CurDispObj;                  // 1 - current time, 0 - time setup
	uint8_t CurDigit;                    // currently displayed digit
	uint8_t NumOfDigits;                 // must be assigned to NUM_OF_DIGITS  
	uint8_t Digit[NUM_OF_DIGITS];        // display of digits array 
	DigitPointerPort DigitPointer;       // 
	void(*setCurrDigit)(Display *disp);  // 
	pin double_point;
} Display;