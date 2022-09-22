/*
 * keypad.h
 *
 * Created: 01-Sep-20 12:56:55 PM
 *  Author: Khaled Hagras
 */ 


#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "GPIO.h"
#define KEYPADPORT 'A'

#define ROW1 (u8)0
#define ROW2 (u8)1
#define ROW3 (u8)2
#define ROW4 (u8)3

#define COLUMN1 (u8)4
#define COLUMN2 (u8)5
#define COLUMN3 (u8)6
#define COLUMN4 (u8)7

void keypadInit();
u8 readChar();
u8 toChar(u8 val);
#endif /* KEYPAD_H_ */