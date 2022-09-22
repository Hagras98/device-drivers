/**
 * @file lcd.h
 * @author Khaled Hagras
 * @brief Controlling a character LCD device.
 * 	
 * This header file contains macros and function definitions to display characters, numbers and symbols on a character lcd device.
*/ 

#ifndef LCD_H_
#define LCD_H_



#include "../GPIO/GPIO.h"
#include "configuration.h"

#define DELAY_VAL 50
#define ENABLE_VAL 2

#define ON 1
#define OFF 0

#define EIGHT_PINS 8
#define FOUR_PINS 4

#define MAX_CHARACTERS_PER_LINE 40
#define LAST_DDRAM_ADDRESS 0x7F

#define RS_COMMAND LOW
#define RS_DATA HIGH


/********LCD device********/

//LCD commands
#define CLEAR_DISPLAY (u8)0x01
#define CURSOR_HOME (u8)0x02
#define ENTRY_MODE (u8)0x06
#define DISPLAY_OFF (u8)0x08
#define CURSROR_HIDE (u8)0x0C
#define CURSOR_BLINK_ON (u8)0x0F
#define CURSOR_BLINK_OFF (u8)0x0E
#define SHIFT_CURSOR_LEFT (u8)0x10
#define SHIFT_CURSOR_RIGHT (u8)0x14
#define SHIFT_DISPLAY_LEFT (u8)0x18
#define SHIFT_DISPLAY_RIGHT (u8)0x1C
#define _4BIT_MODE (u8)0x28
#define _8BIT_MODE (u8)0x38

#if LCD_MODE == FOUR_PINS
#define DATA_LENGTH_BIT 0x00
#elif LCD_MODE == EIGHT_PINS
#define DATA_LENGTH_BIT 0x10
#endif

#define SINGLE_LINE_5x7_FONT (u8)(DATA_LENGTH_BIT | 0x20)
#define SINGLE_LINE_5x10_FONT (u8)(DATA_LENGTH_BIT | 0x24)
#define DOUBLE_LINE_5x7_FONT (u8)(DATA_LENGTH_BIT | 0x28)


/*user defined characters*/

#define CHAR_1 0X00
#define CHAR_2 0X01
#define CHAR_3 0X02
#define CHAR_4 0X03
#define CHAR_5 0X04
#define CHAR_6 0X05
#define CHAR_7 0X06
#define CHAR_8 0X07


/*CGRAM addresses*/

#define CHAR_1_ADDRESS 0X40
#define CHAR_2_ADDRESS 0X48
#define CHAR_3_ADDRESS 0X50
#define CHAR_4_ADDRESS 0X58
#define CHAR_5_ADDRESS 0X60
#define CHAR_6_ADDRESS 0X68
#define CHAR_7_ADDRESS 0X70
#define CHAR_8_ADDRESS 0X78


/**********pin definitions**********/

#define EN_PORT (u8)ENABLE_PORT
#define EN_PIN (u8)ENABLE_PIN

#define RS_PORT (u8)REGISTER_SELECT_PORT
#define RS_PIN (u8)REGISTER_SELECT_PIN

#define D_PORT DATA_PORT //use if SINGLE_PORT_MODE is ON in configuration.h


//use if SINGLE_PORT_MODE is OFF in configuration.h
#define	LCD_P0	(u8)LCD_PIN0
#define LCD_P1	(u8)LCD_PIN1
#define	LCD_P2	(u8)LCD_PIN2
#define	LCD_P3	(u8)LCD_PIN3
#define	LCD_P4	(u8)LCD_PIN4
#define	LCD_P5	(u8)LCD_PIN5
#define	LCD_P6	(u8)LCD_PIN6
#define	LCD_P7	(u8)LCD_PIN7

#define	LCD_P0_PORT (u8)LCD_PIN0_PORT
#define	LCD_P1_PORT (u8)LCD_PIN1_PORT
#define	LCD_P2_PORT (u8)LCD_PIN2_PORT
#define	LCD_P3_PORT (u8)LCD_PIN3_PORT
#define	LCD_P4_PORT (u8)LCD_PIN4_PORT
#define	LCD_P5_PORT (u8)LCD_PIN5_PORT
#define	LCD_P6_PORT (u8)LCD_PIN6_PORT
#define	LCD_P7_PORT (u8)LCD_PIN7_PORT

typedef struct Custom_char
{
	u8 row1;
	u8 row2;
	u8 row3;
	u8 row4;
	u8 row5;
	u8 row6;
	u8 row7;
	u8 row8;
}custom_char;
 

/****Function definitions****/

/**
 *  @brief Sending an enable signal to lcd.
 *	  
 *	This enable signal is used to initiate the actual transfer of commands and characters to lcd module. Data is only transferred on the high to low transition of enable signal. 
 *
 *  @return void
 */
void send_enable();

/**
 *  @brief Sending data to lcd.
 *	  
 *	Function is used to send data to lcd whether command or character.
 *
 *	@param val value of data to be send to lcd.
 *  @return void
 */
void send_to_lcd(u8 val);

/**
 *  @brief send data to lcd pins when SINGLE_PORT_MODE is switched off.
 *	  
 *	This enable signal is used to initiate the actual transfer of commands and characters to lcd module. Data is only transferred on the high to low transition of enable signal. 
 *
 *	@param val value to be written on lcd pins
 *  @return EXIT_FAILURE if SINGLE_PORT_MODE is switched on, EXIT_SUCCESS if SINGLE_PORT_MODE is switched off.
 */
u8 write_lcd_pins(u8 val);

/**
 *  @brief send command.
 *	  
 *	This function sends commands to control the lcd. The list of possible commands is available in lcd.h
 *
 *	@param command command to be sent to lcd
 *  @return void.
 */
void send_command(u8 command);

/**
 *  @brief initiate the lcd device.
 *	  
 *	initiate the lcd by setting the lcd pins to output and sending necessary commands to use the device according to specifications set in configuration.h. Essential to call the function before using the lcd.
 *
 *  @return void.
 */
void lcd_init();

/**
 *  @brief display a character on lcd
 *	  
 *	Display a single character on the lcd at the current cursor position (No more than 40 characters can be typed per line)
 *
 *	@param character value to be displayed
 *  @return EXIT
 */
u8 send_char(char character);
u8 send_string(char *character);

/**
 *  @brief display a character on lcd
 *	  
 *	Display a single character on the lcd at the current cursor position (No more than 40 characters can be typed per line)
 *
 *	@param character value to be displayed
 *  @return 
 */
u8 go_to(u8 row, u8 column);
void create_character(custom_char _char, u8 address);

#endif /* LCD_H_ */