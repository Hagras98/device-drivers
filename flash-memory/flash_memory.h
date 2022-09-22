#ifndef MEMORY_H_
#define MEMORY_H_

#include "driver_lib.h"
/*-------------------------------------------*/

/*SPI CONFIGURATIONS*/
/*------------------------------------------------------------------------------------------------------------*/

//GPIO Pins
#define	MEMORY_CS_PORT							GPIO_PORT_P3
#define MEMORY_CS_PIN							GPIO_PIN0
#define MEMORY_SIMO_PORT						GPIO_PORT_P3
#define MEMORY_SIMO_PIN							GPIO_PIN1
#define MEMORY_SOMI_PORT						GPIO_PORT_P3
#define MEMORY_SOMI_PIN							GPIO_PIN2
#define MEMORY_CLK_PORT							GPIO_PORT_P3
#define MEMORY_CLK_PIN							GPIO_PIN3

//Init Configurations
#define MEMORY_SPI_CLOCK_SOURCE					USCI_B_SPI_CLOCKSOURCE_SMCLK
#define MEMORY_SPI_CLOCK_FREQUENCY				UCS_getSMCLK()
#define MEMORY_SPI_DESIRED_CLOCK				500000
#define MEMORY_SPI_SHIFT_DIRECTION				USCI_B_SPI_MSB_FIRST
#define MEMORY_SPI_CLOCK_MODE					3
#if MEMORY_SPI_CLOCK_MODE == 0
	#define MEMORY_SPI_CLOCK_PHASE				USCI_A_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT
	#define MEMORY_SPI_CLOCK_POLARITY			USCI_A_SPI_CLOCKPOLARITY_INACTIVITY_LOW
#elif MEMORY_SPI_CLOCK_MODE == 3
	#define MEMORY_SPI_CLOCK_PHASE				USCI_A_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT
	#define MEMORY_SPI_CLOCK_POLARITY			USCI_A_SPI_CLOCKPOLARITY_INACTIVITY_HIGH
#else
	#error Clock Mode Should be Set to 0 or 3
#endif
#define MEMORY_SPI_INIT_TRANSMIT_INTERRUPT()	MEMORY_SPI_DISABLE_INTERRUPT(MEMORY_TRANSMIT_INTERRUPT)
#define MEMORY_SPI_INIT_RECIEVE_INTERRUPT()		MEMORY_SPI_DISABLE_INTERRUPT(MEMORY_RECIEVE_INTERRUPT)
//Base address
#define	MEMORY_SPI_BASE							USCI_B0_BASE

//init struct
#define	MEMORY_INIT_STRUCT						USCI_B_SPI_initMasterParam

//Interrupt
#define MEMORY_TRANSMIT_INTERRUPT				USCI_B_SPI_TRANSMIT_INTERRUPT
#define MEMORY_RECIEVE_INTERRUPT				USCI_B_SPI_RECEIVE_INTERRUPT
#define MEMORY_VECTOR							USCI_B0_VECTOR
#define MEMORY_VECTOR_REGISTER					UCB0IV

//GPIO APIs
#define MEMORY_GPIO_SET_CS_OUTPUT();							GPIO_setAsOutputPin(MEMORY_CS_PORT, MEMORY_CS_PIN)
#define MEMORY_GPIO_SET_SIMO_OUTPUT_ALTERNATING_FUNCTION()		GPIO_setAsPeripheralModuleFunctionOutputPin(MEMORY_SIMO_PORT, MEMORY_SIMO_PIN)
#define MEMORY_GPIO_SET_CLK_OUTPUT_ALTERNATING_FUNCTION()		GPIO_setAsPeripheralModuleFunctionOutputPin(MEMORY_CLK_PORT, MEMORY_CLK_PORT)
#define MEMORY_GPIO_SET_SOMI_INPUT_ALTERNATING_FUNCTION()		GPIO_setAsPeripheralModuleFunctionInputPin(MEMORY_SOMI_PORT, MEMORY_SOMI_PIN)
#define MEMORY_GPIO_DRIVE_CS_HIGH()								GPIO_setOutputHighOnPin(MEMORY_CS_PORT, MEMORY_CS_PIN)
#define MEMORY_GPIO_DRIVE_CS_LOW()								GPIO_setOutputLowOnPin(MEMORY_CS_PORT, MEMORY_CS_PIN)

//SPI APIs
#define MEMORY_SPI_init(ptrParam)				USCI_B_SPI_initMaster(MEMORY_SPI_BASE, ptrParam)
#define MEMORY_SPI_ENABLE()						USCI_B_SPI_enable(MEMORY_SPI_BASE)
#define MEMORY_SPI_DISABLE()					USCI_B_SPI_disable(MEMORY_SPI_BASE)
#define MEMORY_SPI_TRANSMIT(data)				USCI_B_SPI_transmitData(MEMORY_SPI_BASE, data);\
												while(!MEMORY_SPI_INTERRUPT_STATUS(MEMORY_TRANSMIT_INTERRUPT))
#define MEMORY_SPI_RECIEVE()					USCI_B_SPI_receiveData(MEMORY_SPI_BASE)
#define MEMORY_SPI_ENABLE_INTERRUPT(vector)		USCI_B_SPI_enableInterrupt(MEMORY_SPI_BASE, vector)
#define MEMORY_SPI_DISABLE_INTERRUPT(vector)	USCI_B_SPI_disableInterrupt(MEMORY_SPI_BASE, vector)
#define MEMORY_SPI_CLR_INTERRUPT(vector)		USCI_B_SPI_clearInterrupt(MEMORY_SPI_BASE, vector)
#define MEMORY_SPI_INTERRUPT_STATUS(vector)		USCI_B_SPI_getInterruptStatus(MEMORY_SPI_BASE, vector)
#define MEMORY_SPI_ISR							USCI_B0_ISR
/*------------------------------------------------------------------------------------------------------------*/

/****************************************************************************************************************************************************
 ***********************************************	DO NOT CHANGE ANY VALUES BELOW THIS POINT	*****************************************************
 ****************************************************************************************************************************************************/

/*Memory typedefs*/
/*---------------------------------*/
//Addresses

typedef struct{
	uint32_t page: 6;
	uint32_t plane: 1;
	uint32_t block: 10;
	uint32_t dummy: 7;
}pageBlockBitField_t;

typedef	union{
	pageBlockBitField_t addressField;
	uint8_t addressBytes[3];
}pageBlockAddress_t;

typedef struct {
	uint16_t column: 12;
	uint16_t plane: 1;
	uint16_t dummy: 3;
}columnBitField_t;

typedef	union
{
	columnBitField_t addressField;
	uint8_t addressBytes[2];
}columnAddress_t;

typedef struct {
	columnAddress_t* columnAddress;
	pageBlockAddress_t* pageBlockAddress;
}fullAddress_t;
/*---------------------------------*/

/*SPI NAND Command Definitions*/
/*-------------------------------------------------------------------------------------------------------------------------------------------*/
#define DUMMY_BYTE							0X00
#define RESET 								0xFF					//Reset the device
#define GET_FEATURES						0x0F					//Get features
#define SET_FEATURES						0x1F					//Set features
#define READ_ID								0x9F					//Read device ID
#define PAGE_READ							0x13					//Array read
#define READ_PAGE_CACHE_RANDOM				0x30					//Cache read
#define READ_PAGE_CACHE_LAST				0x3F					//Ending of cache read
#define READ_FROM_CACHE_x1					0x0B					//Output cache data at column address
#define READ_FROM_CACHE_x2					0x3B					//Output cache data on IO[1:0]
#define READ_FROM_CACHE_x4					0x6B					//Output cache data on IO[3:0]
#define READ_FROM_CACHE_Dual_IO				0xBB					//Input address/Output cace data on IO[1:0]
#define READ_FROM_CACHE_Quad_IO				0xEB					//Input address/Output cace data on IO[3:0]
#define WRITE_ENABLE						0x06					//Sets the WEL bit in the status register to 1
																	//required to enable operations that change the content of the memory array
#define WRITE_DISABLE						0x04					//Clears the WEL bit in the status register to 0
																	//required to disable operations that change the content of te memory array
#define BLOCK_ERASE							0xD8					//Block erase
#define PROGRAM_EXECUTE						0x10					//Array program
#define PROGRAM_LOAD_x1						0x02					//Load program data into cache register on SI
#define PROGRAM_LOAD_x4						0x32					//Load program data into cache register on SO[3:0]
#define PROGRAM_LOAD_RANDOM_DATA_x1			0x84					//Overwrite cache register with input data on SI
#define PROGRAM_LOAD RANDOM_DATA_x4			0x34					//Overwrite cache register with input data on SO[3:0]
#define PERMANENT_BLOCK_LOCK_PROTECTION		0x2C					//Permanently protect a specific group of blocks
/*-------------------------------------------------------------------------------------------------------------------------------------------*/


/*Feature Address Settings and Data Bits*/
/*-----------------------------------------------------*/
//Block lock register
#define BLOCK_LOCK				0xA0			//Address
//Data bits
#define BRWD					1<<7
#define BP3						1<<6
#define BP2						1<<5
#define BP1						1<<4
#define BP0						1<<3
#define TB						1<<2
#define WP_HOLD_Disable			1<<1

//Configuration register
#define CONFIGURATION			0xB0			//Address
//Data bits
#define CFG2					1<<7
#define CFG1					1<<6
#define LOT_EN					1<<5
#define ECC_EN					1<<4
#define CFG0					1<<1

//Status register
#define STATUS					0xC0			//Address
//Data bits
#define CRBSY					1<<7
#define ECCS2					1<<6
#define ECCS1					1<<5
#define ECCS0					1<<4
#define P_FAIL					1<<3
#define E_FAIL					1<<2
#define WEL						1<<1
#define OIP						1<<0

//Die select register
#define DIE_SELECT				0xD0			//Address
//Data bits
#define DS0						1<<6
/*-----------------------------------------------------*/


/*Block Protection Bits*/
/*----------------------------------------------------------------------*/
#define ALL_UNLOCKED			(0x00)
#define UPPER_1_1024_LOCKED		(BP0)
#define UPPER_1_512_LOCKED		(BP1)
#define UPPER_1_256_LOCKED		(BP1 | BP0)
#define UPPER_1_128_LOCKED		(BP2)
#define UPPER_1_64_LOCKED		(BP2 | BP0)
#define UPPER_1_32_LOCKED		(BP2 | BP1)
#define UPPER_1_16_LOCKED		(BP2 | BP1 | BP0)
#define UPPER_1_8_LOCKED		(BP3)
#define UPPER_1_4_LOCKED		(BP3 | BP0)
#define UPPER_1_2_LOCKED		(BP3 | BP1)
#define LOWER_1_1024_LOCKED		(TB | BP0)
#define LOWER_1_512_LOCKED		(TB | BP1)
#define LOWER_1_256_LOCKED		(TB | BP1 | BP0)
#define LOWER_1_128_LOCKED		(TB | BP2)
#define LOWER_1_64_LOCKED		(TB | BP2 | BP0)
#define LOWER_1_32_LOCKED		(TB | BP2 | BP1)
#define LOWER_1_16_LOCKED		(TB | BP2 | BP1 | BP0)
#define LOWER_1_8_LOCKED		(TB | BP3)
#define LOWER_1_4_LOCKED		(TB | BP3 | BP0)
#define LOWER_1_2_LOCKED		(TB | BP3 | BP1)
#define ALL_LOCKED				(TB | BP3 | BP2 | BP1 | BP0)	//deafult
/*----------------------------------------------------------------------*/


/*Configuration Registers for Security*/
/*--------------------------------------------------------------------------------*/
#define NORMAL_OPERATION										(0X00)	//deafult
#define ACCESS_OTP_AREA__PARAMETER__UNIQUE_ID					(CFG1)
#define ACCESS_OTP_DATA_PROTECTION_BIT_TO_LOCK_OTP_AREA			(CFG1 |CFG2)
#define ACCESS_SPI_NOR_READ_PROTOCOL_ENABLE_MODE				(CFG0 | CFG2)
#define ACCESS_PERMANENT_BLOCK_LOCK_PROTECTION_DISABLE_MODE		(CFG0 | CFG1 |CFG2)
/*--------------------------------------------------------------------------------*/


/*APIs Declaration*/
/*-------------------------------------------------------------------------------------------------------------*/
extern apiStatus_t MEMORY_INIT(void);
extern apiStatus_t MEMORY_SEND_COMMAND(uint8_t command);
extern apiStatus_t MEMORY_GET_FEATURE(uint8_t address, uint8_t *featureValue);
extern apiStatus_t MEMORY_VERIFY_ID(void);
extern apiStatus_t MEMORY_RST(void);
extern apiStatus_t MEMORY_PAGE_READ(pageBlockAddress_t *readAddress);
extern apiStatus_t MEMORY_READ_FROM_CACHE_x1(columnAddress_t *cacheColumn, uint8_t* readArray, uint16_t arraySize);
extern apiStatus_t MEMORY_WRITE_ENABLE();
extern apiStatus_t MEMORY_PROGRAM_LOAD(columnAddress_t *cacheColumn, uint8_t* writeArray, uint16_t arraySize);
extern apiStatus_t MEMORY_PROGRAM_EXECUTE(pageBlockAddress_t *writeAddress);
extern apiStatus_t MEMORY_BLOCK_ERASE(pageBlockAddress_t *writeAddress);
extern apiStatus_t MEMORY_SET_FEATURE(uint8_t address, uint8_t featureValue);
extern apiStatus_t READ(fullAddress_t *readAddress, uint8_t* readArray, uint16_t arraySize);
extern apiStatus_t PROGRAM(fullAddress_t *writeAddress, uint8_t* writeArray, uint16_t arraySize);
/*-------------------------------------------------------------------------------------------------------------*/

#endif
