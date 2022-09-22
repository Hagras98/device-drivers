#include <flash_memory.h>

apiStatus_t MEMORY_INIT(void)
{
	apiStatus_t returnValue = STATUS_FAIL;
	MEMORY_GPIO_SET_CS_OUTPUT();
	MEMORY_GPIO_SET_SIMO_OUTPUT_ALTERNATING_FUNCTION();
	MEMORY_GPIO_SET_CLK_OUTPUT_ALTERNATING_FUNCTION();
	MEMORY_GPIO_SET_SOMI_INPUT_ALTERNATING_FUNCTION();
	MEMORY_INIT_STRUCT param = {0};
    param.selectClockSource = MEMORY_SPI_CLOCK_SOURCE;
    param.clockSourceFrequency = MEMORY_SPI_CLOCK_FREQUENCY;
    param.desiredSpiClock = MEMORY_SPI_DESIRED_CLOCK;
    param.msbFirst = MEMORY_SPI_SHIFT_DIRECTION;
    param.clockPhase = MEMORY_SPI_CLOCK_PHASE;
    param.clockPolarity =  MEMORY_SPI_CLOCK_POLARITY;

	if ( STATUS_SUCCESS == MEMORY_SPI_init(&param) )
	{
		returnValue = STATUS_SUCCESS;
	}

	MEMORY_SPI_ENABLE();
	MEMORY_SPI_INIT_RECIEVE_INTERRUPT();
	MEMORY_SPI_INIT_TRANSMIT_INTERRUPT();
	MEMORY_SPI_CLR_INTERRUPT(MEMORY_TRANSMIT_INTERRUPT);
    MEMORY_SPI_CLR_INTERRUPT(MEMORY_RECIEVE_INTERRUPT);
	MEMORY_GPIO_DRIVE_CS_HIGH();
	return returnValue;
}

apiStatus_t MEMORY_SEND_COMMAND(uint8_t command)
{
	__delay_cycles(40);
	MEMORY_GPIO_DRIVE_CS_LOW();
	__delay_cycles(40);
	MEMORY_SPI_TRANSMIT(command);
	return STATUS_SUCCESS;
}

apiStatus_t MEMORY_VERIFY_ID()
{
	uint8_t id [2];
	MEMORY_SEND_COMMAND(READ_ID);
	MEMORY_SPI_TRANSMIT(DUMMY_BYTE);
	MEMORY_SPI_TRANSMIT(DUMMY_BYTE);
	id[0] = MEMORY_SPI_RECIEVE();
	MEMORY_SPI_TRANSMIT(DUMMY_BYTE);
	id[1] = MEMORY_SPI_RECIEVE();
	MEMORY_GPIO_DRIVE_CS_HIGH();

	if (id[0] == 0x2C && id[1] == 0x24)
	{
		return STATUS_SUCCESS;
	}

	else
	{
		return STATUS_FAIL;
	}
}


apiStatus_t MEMORY_RST(void)
{
	MEMORY_SEND_COMMAND(RESET);
	MEMORY_GPIO_DRIVE_CS_HIGH();
	return STATUS_SUCCESS;
}

apiStatus_t MEMORY_GET_FEATURE(uint8_t address, uint8_t* featureValue)
{
	MEMORY_SEND_COMMAND(GET_FEATURES);
	MEMORY_SPI_TRANSMIT(address);
	MEMORY_SPI_TRANSMIT(DUMMY_BYTE);
	*featureValue = MEMORY_SPI_RECIEVE();
	MEMORY_GPIO_DRIVE_CS_HIGH();
	return STATUS_SUCCESS;
}

apiStatus_t MEMORY_SET_FEATURE(uint8_t address, uint8_t featureValue)
{
	MEMORY_SEND_COMMAND(SET_FEATURES);
	MEMORY_SPI_TRANSMIT(address);
	MEMORY_SPI_TRANSMIT(featureValue);
	MEMORY_GPIO_DRIVE_CS_HIGH();
	return	STATUS_SUCCESS;
}

apiStatus_t MEMORY_PAGE_READ(pageBlockAddress_t* readAddress)
{
	volatile uint8_t i = 0;
	MEMORY_SEND_COMMAND(PAGE_READ);
	for (i = 0; i < 3; i++)
	{
		MEMORY_SPI_TRANSMIT(readAddress->addressBytes[2 - i]);
	}
	MEMORY_GPIO_DRIVE_CS_HIGH();
	return STATUS_SUCCESS;
}

apiStatus_t MEMORY_READ_FROM_CACHE_x1(columnAddress_t *cacheColumn, uint8_t* readArray, uint16_t arraySize)
{
	volatile uint16_t i = 0;
	MEMORY_SEND_COMMAND(READ_FROM_CACHE_x1);
	for (i = 0; i < 2; i++)
	{
		MEMORY_SPI_TRANSMIT(cacheColumn->addressBytes[1 - i]);
	}
	MEMORY_SPI_TRANSMIT(DUMMY_BYTE);
	for (i = 0; i < arraySize; i++)
	{
		MEMORY_SPI_TRANSMIT(DUMMY_BYTE);
		readArray[i] = MEMORY_SPI_RECIEVE();
	}
	MEMORY_GPIO_DRIVE_CS_HIGH();
	return STATUS_SUCCESS;
}

apiStatus_t MEMORY_WRITE_ENABLE()
{
	MEMORY_SEND_COMMAND(WRITE_ENABLE);
	MEMORY_GPIO_DRIVE_CS_HIGH();
	return STATUS_SUCCESS;
}

apiStatus_t MEMORY_PROGRAM_LOAD(columnAddress_t *cacheColumn, uint8_t* writeArray, uint16_t arraySize)
{
	uint16_t i = 0;
	MEMORY_SEND_COMMAND(PROGRAM_LOAD_x1);
	for (i = 0; i < 2; i++)
	{
		MEMORY_SPI_TRANSMIT(cacheColumn->addressBytes[1 - i]);
	}
	for (i = 0; i < arraySize; i++)
	{
		MEMORY_SPI_TRANSMIT(writeArray[i]);
	}
	MEMORY_GPIO_DRIVE_CS_HIGH();
	return STATUS_SUCCESS;
}

apiStatus_t MEMORY_PROGRAM_EXECUTE(pageBlockAddress_t *writeAddress)
{
	uint8_t i = 0;
	MEMORY_SEND_COMMAND(PROGRAM_EXECUTE);
	for (i = 0; i < 3; i++){
		MEMORY_SPI_TRANSMIT(writeAddress->addressBytes[2 - i]);
	}
	MEMORY_GPIO_DRIVE_CS_HIGH();
	return STATUS_SUCCESS;
}

apiStatus_t MEMORY_BLOCK_ERASE(pageBlockAddress_t *writeAddress)
{
	static apiStatus_t MEMORYERASapiStatus_t = STATUS_SUCCESS;
	uint8_t statValue;
	uint8_t i = 0;
	MEMORYERASapiStatus_t |= MEMORY_WRITE_ENABLE();
	MEMORYERASapiStatus_t |= MEMORY_SEND_COMMAND(BLOCK_ERASE);

	for (i = 0; i < 3; i++)
	{
		MEMORY_SPI_TRANSMIT(writeAddress->addressBytes[2 - i]);
	}

	do
	{
		MEMORY_GET_FEATURE(STATUS, &statValue);
	}
	while(statValue & OIP == OIP);

	if(statValue & E_FAIL == E_FAIL)
	{
		MEMORYERASapiStatus_t = STATUS_FAIL;
	}

	MEMORY_GPIO_DRIVE_CS_HIGH();
	return MEMORYERASapiStatus_t;
}

apiStatus_t READ(fullAddress_t *readAddress, uint8_t* readArray, uint16_t arraySize)
{
	static apiStatus_t MEMORYREAD_status = STATUS_SUCCESS;
	uint8_t statValue;
	pageBlockAddress_t *pageBlock = readAddress->pageBlockAddress;
	columnAddress_t	*readcolumn = readAddress->columnAddress;
	MEMORYREAD_status |= MEMORY_PAGE_READ(pageBlock);

	do
	{
		MEMORY_GET_FEATURE(STATUS, &statValue);
	}
	while(statValue & OIP == OIP);

	MEMORYREAD_status |= MEMORY_READ_FROM_CACHE_x1(readcolumn, readArray, arraySize);

	if(statValue & P_FAIL == P_FAIL)
	{
		MEMORYREAD_status = STATUS_FAIL;
	}

	return MEMORYREAD_status= STATUS_SUCCESS;;
}

apiStatus_t PROGRAM(fullAddress_t *writeAddress, uint8_t* writeArray, uint16_t arraySize)
{
	static apiStatus_t MEMORYPROGRAM_status = STATUS_SUCCESS;
	uint8_t statValue;
	pageBlockAddress_t *pageBlock = writeAddress->pageBlockAddress;
	columnAddress_t	*writecolumn = writeAddress->columnAddress;
	MEMORYPROGRAM_status |= MEMORY_WRITE_ENABLE();
	MEMORYPROGRAM_status |= MEMORY_PROGRAM_LOAD(writecolumn, writeArray, arraySize);
	MEMORYPROGRAM_status |= MEMORY_PROGRAM_EXECUTE(pageBlock);

	do
	{
		MEMORY_GET_FEATURE(STATUS, &statValue);
	}
	while(statValue & OIP == OIP);

	if(statValue & P_FAIL == P_FAIL)
	{
		MEMORYPROGRAM_status = STATUS_FAIL;
	}
	return MEMORYPROGRAM_status;
}
