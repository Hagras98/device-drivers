/*
 * eeprom.h
 *
 * Created: 20-Oct-20 2:22:46 PM
 *  Author: khaled
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_
#include "reg.h"
#include "I2c.h"
void EEPROM_WriteWord(u16 adress, u8 word);
u8 EEPROM_ReadWord(u16 adress);
void EEPROM_sequentialRead(u16 adress,u8 n,u8*arr);
void EEPROM_WritePage(u16 adress,u8 n,u8*arr);

#endif /* EEPROM_H_ */