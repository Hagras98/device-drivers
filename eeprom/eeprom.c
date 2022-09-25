/*
 * eeprom.c
 *
 * Created: 20-Oct-20 2:22:22 PM
 *  Author: khaled
 */ 
#include "eeprom.h"

void EEPROM_WriteWord(u16 adress, u8 word)
{
	u8 deviceAdress=(u8)(0xA0|(u8)((adress&0x0700)>>7));
	u8 byteAdress=(u8)(adress);
	MasterT_start();
	MasterT_send(deviceAdress);
	MasterT_send(byteAdress);
	MasterT_send(word);
	MasterT_stop();
}

u8 EEPROM_ReadWord(u16 adress)
{
	u8 word;
	u8 deviceAdress=(u8)(0xA0|(u8)((adress&0x0700)>>7));
	u8 byteAdress=(u8)(adress);
	MasterT_start();
	MasterT_send(deviceAdress);
	MasterT_send(byteAdress);
	MasterT_start();
	deviceAdress|=1;
	MasterT_send(deviceAdress);
	word = MasterNak_recieve();
	MasterT_stop();
	return word;
}

void EEPROM_sequentialRead(u16 adress,u8 n,u8*arr)
{
	u8 i=0;
	u8 deviceAdress=(u8)(0xA0|(u8)((adress&0x0700)>>7));
	u8 byteAdress=(u8)(adress);
	MasterT_start();
	MasterT_send(deviceAdress);
	MasterT_send(byteAdress);
	MasterT_start();
	deviceAdress|=1;
	MasterT_send(deviceAdress);
	for (i=0;i<n-1;i++)
		{
			*(arr+i) = MasterAck_recieve();
		}
	*(arr+i) = MasterNak_recieve();
	MasterT_stop();	
}

void EEPROM_WritePage(u16 adress,u8 n,u8*arr)
{
	u8 i=0;
	u8 deviceAdress=(u8)(0xA0|(u8)((adress&0x0700)>>7));
	u8 byteAdress=(u8)(adress);
	MasterT_start();
	MasterT_send(deviceAdress);
	MasterT_send(byteAdress);
	for (i=0;i<n;i++)
	{
		MasterT_send(*(arr+i));
	}
	MasterT_stop();
}