/*
 * keypad.c
 *
 * Created: 01-Sep-20 1:23:29 PM
 *  Author: khaled
 */ 
#include "keypad.h"
#include <util/delay.h>
void keypadInit()
{
	directionPin(KEYPADPORT,OUTPUT,ROW1);
	directionPin(KEYPADPORT,OUTPUT,ROW2);	
	directionPin(KEYPADPORT,OUTPUT,ROW3);	
	directionPin(KEYPADPORT,OUTPUT,ROW4);	
	writePin(KEYPADPORT,HIGH,ROW1);
	writePin(KEYPADPORT,HIGH,ROW2);
	writePin(KEYPADPORT,HIGH,ROW3);
	writePin(KEYPADPORT,HIGH,ROW4);
	writePin(KEYPADPORT,HIGH,COLUMN1);
	writePin(KEYPADPORT,HIGH,COLUMN2);
	writePin(KEYPADPORT,HIGH,COLUMN3);
	writePin(KEYPADPORT,HIGH,COLUMN4);
	directionPin(KEYPADPORT,INPUT,COLUMN1);
	directionPin(KEYPADPORT,INPUT,COLUMN2);
	directionPin(KEYPADPORT,INPUT,COLUMN3);
	directionPin(KEYPADPORT,INPUT,COLUMN4);
}
u8 toChar(u8 val)
{
	switch (val)
	{
		case 0:
		return '1';
		case 1:
		return '2';
		case 2:
		return '3';
		case 3:
		return 'A';
		case 10:
		return '4';
		case 11:
		return '5';
		case 12:
		return '6';
		case 13:
		return 'B';
		case 20:
		return '7';
		case 21:
		return '8';
		case 22:
		return '9';
		case 23:
		return 'C';
		case 30:
		return '*';
		case 31:
		return '0';
		case 32:
		return '#';
		case 33:
		return 'D';
		default: return 0;
	}
}
u8 readChar()
{
	u8 num;
	u8 columns[4]={COLUMN1,COLUMN2,COLUMN3,COLUMN4};
	u8 rows[4]={ROW1,ROW2,ROW3,ROW4};
	while(1){
	for (u8 i=0;i<4;i++)
	{
		writePin(KEYPADPORT,LOW,rows[i]);
		for (u8 j=0;j<4;j++)
		{
			if (readPin(KEYPADPORT,columns[j])==LOW)
			{
				while (readPin(KEYPADPORT,columns[j])==LOW);
				num = (j*10)+i;
				return (toChar(num));
			}
		}
		writePin(KEYPADPORT,HIGH,rows[i]);	
	}
	}	
	return 0;
}
