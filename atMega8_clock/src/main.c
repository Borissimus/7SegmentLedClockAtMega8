/*
 * atMega8_clock.c
 *
 * Created: 29.03.2018 21:14:56
 * Author : Borissimus
 */
#define F_CPU 8000000L
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include "../inc/main.h"
#include "../inc/gpio_pins.h"

//define 7 LED variables
#define CHAR_0 32 // mask 0b00100000
#define CHAR_1 60 // mask 0b00111100
#define CHAR_2 18 // mask 0b00010010
#define CHAR_3 24 // mask 0b00011000
#define CHAR_4 12 // mask 0b00001100
#define CHAR_5 9 // mask 0b00001001
#define CHAR_6 1 // mask 0b00000001
#define CHAR_7 60+1 // mask 0b00111101
#define CHAR_8 0 // mask 0b00000000
#define CHAR_9 8 // mask 0b00001000
#define LED_POINT() PORTD ^= (1<<PD6) //индикация секундных импульсов "двоеточие"

//Записать значение аргумента в 7 сегментный элемент согласно аргументу
#define LED_SET(chDigit) if(chDigit==CHAR_7)PORTB=chDigit-1; else PORTB=chDigit; if(chDigit==CHAR_1 || chDigit==CHAR_4)PORTD|=(1<<PD7); else PORTD&=~(1<<PD7)
//fucking global variables )))
int seconds = 0, minutes = 0, hours = 0, ledNum = 0;
int secondsForSet = 0, minutesForSet = 9, hoursForSet = 1;
char settingTimeFlag = 0;
//int *pSeconds = &seconds, *pMinutes = &minutes, *pHours = &hours, *pLedNum = &ledNum;
//peripheral configure functions
void extInteruptSettings(){ // enable external interrupts for time setting function
	DDRD &= ~(1<<PD2); // set INT0 INT1 of PORTD as interrupt inputs
	DDRD &= ~(1<<PD3);
	PORTD |= (1<<PD2)|(1<<PD3); // Pull-up configure
	MCUCR |= 0b1010; // ext interrupts on pins INT0 and INT1 generates by falling edge
}
void extIntINT0Enable(){ // External Interrupt Request 0 Enable
	GICR |= (1<<INT0);
}
void extIntINT1Enable(){ // External Interrupt Request 1 Enable
	GICR |= (1<<INT1);		
}
void extIntINT0Disable(){ //External Interrupt Request 0 Disable
	GICR &= ~(1<<INT0);
}
void extIntINT1Disable(){ //External Interrupt Request 1 Disable
	GICR &= ~(1<<INT1);
}
void counterSettings(void){
	// 16-bit Timer/Counter1
	TCCR1B = TCCR1B | 0xE; //mask: 1110 16bit counter to external source is set (T1)PD5
	TCCR1A = TCCR1A | 0b100;  // 00 Waveform Gen Mode
	OCR1A = 32767; //Compare Reg Value for timer  In dec. it is 32767
	// counter 8-bit Timer/Counter0
	TCCR0 = 0b11; //cl/O/64 prescaler Timer/Counter0 Control Register
	TIMSK |= (1<<TOIE0); //Timer/Counter0 Overflow Interrupt Enable (TOIE0 set to 1)
}
void timer1IntEnable(){
	TIMSK |= 0x10; //Timer/Counter1, Output Compare A Match Interrupt Enable
}
void timer1IntDisable(){
	TIMSK &= ~(1<<OCIE1A); //Timer/Counter1, Output Compare A Match Interrupt Disable
}
void ledPortSEtting(){
	DDRD |= 0b11000000;
	PORTD |= 0b11000000;
	DDRB = 0xFF;
	PORTB = 0xFF;
	DDRC = 0xFF;
	PORTC = 0xFF;
}
// Set time function
void setNewTime(void){
	int count;
	for(count = 0; count<30; count++){ // проверка времени удержания кнопки настройки времени
		_delay_ms(50);
		if(!(PIND & (1<<PD2)))continue;
		else break;
	}
	if(!(PIND & (1<<PD2))){ // если кнопка нажата - начать програму установки
		secondsForSet = seconds;
		minutesForSet = minutes;
		hoursForSet = hours;
		settingTimeFlag = 1;
		} 
}
// main program user functions
void uptClockVars(int *seconds, int *minutes, int *hours){
	LED_POINT(); // toggle led double point
	*seconds = *seconds + 1;
	if(*seconds == 60){*seconds = 0; *minutes = *minutes + 1;}
	if(*minutes == 60){*minutes = 0; *hours = *hours + 1;}
	if(*hours == 24)*hours = 0; 		
}
void setDigit(int digit){ //записать значение аргумента в 7 сег. индикатор
	switch(digit){
		case 0: LED_SET(CHAR_0); break;
		case 1: LED_SET(CHAR_1); break;
		case 2: LED_SET(CHAR_2); break;
		case 3: LED_SET(CHAR_3); break;
		case 4: LED_SET(CHAR_4); break;
		case 5: LED_SET(CHAR_5); break;
		case 6: LED_SET(CHAR_6); break;
		case 7: LED_SET(CHAR_7); break;  
		case 8: LED_SET(CHAR_8); break;
		case 9: LED_SET(CHAR_9); break;
	}
}
int getLedValue(int uniVal, char settingTimeFlag){
	if(!settingTimeFlag){
		switch(uniVal){
			case 0: uniVal = seconds%10; break;
			case 1: uniVal = seconds/10; break;
			case 2: uniVal = minutes%10; break;
			case 3: uniVal = minutes/10; break;
			case 4: uniVal = hours%10; break;
			case 5: uniVal = hours/10; break;
		}
	}
	else{
		switch(uniVal){
			case 0: uniVal = secondsForSet%10; break;
			case 1: uniVal = secondsForSet/10; break;
			case 2: uniVal = minutesForSet%10; break;
			case 3: uniVal = minutesForSet/10; break;
			case 4: uniVal = hoursForSet%10; break;
			case 5: uniVal = hoursForSet/10; break;
		}	
	}
	return uniVal;
}
void switchDigit(int *ledNum, int *ledValue){ //установка значений на дисплее
	PORTC = 0xFF; // отключить отображение всех разрядов 7 сег. индикаторов
	setDigit(*ledValue); // установить значение порта для вывода на индикаторе 
	switch(*ledNum){ // выбрать регистр индикатора, в котором будет отображаться значение до следующего прерывания
		case PC0: PORTC&=~(1<<PC0); break; 
		case PC1: PORTC&=~(1<<PC1); break;
		case PC2: PORTC&=~(1<<PC2); break;
		case PC3: PORTC&=~(1<<PC3); break;
		case PC4: PORTC&=~(1<<PC4); break;
		case PC5: PORTC&=~(1<<PC5); break;
	}
}
//global interrupt functions
ISR(TIMER1_COMPA_vect){
	uptClockVars(&seconds, &minutes, &hours); //перевод стрелок
}
ISR(TIMER0_OVF_vect){ // прерывание по смене отображаемого регистра и установка значения в него
	int ledValue = getLedValue(ledNum, settingTimeFlag);
	switchDigit(&ledNum, &ledValue);//смена отображаемого регистра дисплея и запись значения в него
	ledNum++;
	if(ledNum == 6) ledNum = 0;	
}

ISR(INT0_vect){

}				
ISR(INT1_vect){
	timer1IntDisable();
}			
int main(void){
	//int test = 0;
	sei(); //interrupts are enable
	counterSettings();
	ledPortSEtting();
	timer1IntEnable();
	extInteruptSettings();
	//extIntINT0Enable();
	extIntINT1Enable();
    while (1){
		if(!(PIND&(1<<PD2)))setNewTime();
    }
}

