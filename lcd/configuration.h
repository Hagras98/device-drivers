/**
 * @file configuration.h
 * @author Khaled Hagras
 * @brief configure the lcd before using it.
 * 	
 * This header file is used to configure the LCD driver to adapt with the user specifications.
*/ 


#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#define DISPLAY_LINES 2
#define CHARACTERS_PER_DISPLAY_LINE 16

#define LCD_MODE EIGHT_PINS //choose mode FOUR_PINS or EIGHT_PINS
#define SINGLE_PORT_MODE ON //choose ON or OFF

#define AUTO_SHIFT_DISPLAY OFF

//choose the enable pin and port
#define ENABLE_PORT 'C'
#define ENABLE_PIN 7

//choose the register select pin and port
#define  REGISTER_SELECT_PORT 'C'
#define  REGISTER_SELECT_PIN 6

//choose port a single port to connect your lcd to (use with single port mode only)
#define DATA_PORT A

//choose a port and pin number for each lcd pin (use when single port mode is off)
#define	LCD_PIN0_PORT A
#define LCD_PIN0 0

#define	LCD_PIN1_PORT A
#define LCD_PIN1 1

#define	LCD_PIN2_PORT A
#define LCD_PIN2 2

#define	LCD_PIN3_PORT A
#define LCD_PIN3 3

#define	LCD_PIN4_PORT A
#define LCD_PIN4 4

#define	LCD_PIN5_PORT A
#define LCD_PIN5 5

#define	LCD_PIN6_PORT A
#define LCD_PIN6 6

#define	LCD_PIN7_PORT A
#define LCD_PIN7 7

/*user defined characters replace CHAR_#N_NAME with a name of your choice*/

#define CHAR_1_NAME CHAR_1
#define CHAR_2_NAME CHAR_2
#define CHAR_3_NAME CHAR_3
#define CHAR_4_NAME CHAR_4
#define CHAR_5_NAME CHAR_5
#define CHAR_6_NAME CHAR_6
#define CHAR_7_NAME CHAR_7
#define CHAR_8_NAME CHAR_8
#endif /* CONFIGURATION_H_ */