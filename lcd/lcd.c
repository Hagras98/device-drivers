/**
 * @file lcd.c
 * @author Khaled Hagras
 * @brief Function prototypes for interfacing character lcd.
 * 	
 * This file contains function definitions to control character lcds through sending data and different commands.
*/ 

#define F_CPU 8000000ul
#include <util/delay.h>
#include "lcd.h"

#if SINGLE_PORT_MODE == OFF
#if LCD_MODE == FOUR_PINS
u8 lcd_pins[LCD_MODE] = {LCD_P4, LCD_P5, LCD_P6, LCD_P7};
u8 lcd_ports[LCD_MODE] = {LCD_P4_PORT, LCD_P5_PORT, LCD_P6_PORT, LCD_P7_PORT};
#elif LCD_MODE == EIGHT_PINS
u8 lcd_pins[LCD_MODE] = {LCD_P0,LCD_P1, LCD_P2, LCD_P3, LCD_P4, LCD_P5, LCD_P6, LCD_P7};
u8 lcd_ports[LCD_MODE] = {LCD_P0_PORT, LCD_P1_PORT, LCD_P2_PORT, LCD_P3_PORT, LCD_P4_PORT, LCD_P5_PORT, LCD_P6_PORT, LCD_P7_PORT};
#endif
#endif

u8 DDRAM_address = 0;
u8 character_count[2];
u8 line;

void send_enable(){
	
	write_pin(ENABLE_PORT, ENABLE_PIN, HIGH);
	_delay_us(1);
	write_pin(ENABLE_PORT, ENABLE_PIN, LOW);
	_delay_ms(3);
}

u8 write_lcd_pins(u8 val){
	#if SINGLE_PORT_MODE == OFF
	for (u8 count = 0; count < LCD_MODE; count++)
	{
		switch ((val && (1 << count)) >> count){
			case 1: write_pin(lcd_ports[count], lcd_pins[count], HIGH);
				break;
			case 0: write_pin(lcd_ports[count], lcd_pins[count], LOW);
				break;				
		}
	}
	return EXIT_SUCCESS;
	#else
	return EXIT_FAILURE;
	#endif
}

void send_to_lcd(u8 val){
	#if SINGLE_PORT_MODE == ON
	#if LCD_MODE == EIGHT_PINS
	write_port(DATA_PORT,val);
	send_enable();
	#elif LCD_MODE == FOUR_PINS
	write_pins_in_port(DATA_PORT, LOW_NIBBLE, LOW);
	write_pins_in_port(DATA_PORT, (val & HIGH_NIBBLE)>>4, HIGH);
	send_enable();
	_delay_us(200);
	write_pins_in_port(DATA_PORT, LOW_NIBBLE, LOW);
	write_pins_in_port(DATA_PORT, (val & LOW_NIBBLE), HIGH);
	send_enable();
	#endif
	#elif SINGLE_PORT_MODE == OFF
	#if LCD_MODE == EIGHT_PINS
	write_lcd_pins(val);
	send_enable();	
	#elif LCD_MODE == FOUR_PINS
	write_lcd_pins(val >> 4);
	send_enable();
	_delay_us(200);
	write_lcd_pins(val);
	send_enable();
	#endif
	#endif
	_delay_ms(2);
}

void send_command(u8 command){
	_delay_ms(DELAY_VAL);
	write_pin(REGISTER_SELECT_PORT, REGISTER_SELECT_PIN, RS_COMMAND );
	send_to_lcd(command);	
	switch(command){
		case SHIFT_CURSOR_RIGHT: DDRAM_address++;
		break;
		case SHIFT_CURSOR_LEFT: DDRAM_address--;
		break;
		default: break;		
		}
}

u8 send_char(char character){
	_delay_ms(DELAY_VAL);
	if (DDRAM_address > LAST_DDRAM_ADDRESS){
		return EXIT_FAILURE;
	}
	if (character_count[line] > MAX_CHARACTERS_PER_LINE){
		return EXIT_FAILURE;
	}
	else{
		write_pin(REGISTER_SELECT_PORT, REGISTER_SELECT_PIN, RS_DATA);
		send_to_lcd(character);
		/*#if AUTO_SHIFT_DISPLAY == ON
			if ((DDRAM_address % 0x40) >= CHARACTERS_PER_DISPLAY_LINE)
			{
				send_command(SHIFT_DISPLAY_LEFT);
			}
		#endif*/
		character_count[line]++;
		return EXIT_SUCCESS	;
	}
}

void lcd_init(){
	direction_pin(ENABLE_PORT, ENABLE_PIN, OUTPUT);
	direction_pin(REGISTER_SELECT_PORT, REGISTER_SELECT_PIN, OUTPUT);
	#if SINGLE_PORT_MODE == ON
	for (u8 pin = 0; pin < LCD_MODE; ++pin){
		direction_pin(DATA_PORT, pin, OUTPUT);
	}	
	#elif SINGLE_PORT_MODE == OFF
	for (u8 count=0; count < LCD_MODE; ++count)	{
		direction_pin(lcd_ports[count], lcd_pins[count], OUTPUT);
	}
	#endif
	_delay_ms(15);
	send_command(CURSOR_HOME);
	#if LCD_MODE == FOUR_PINS
	send_command(_4BIT_MODE);
	#elif LCD_MODE == EIGHT_PINS
	send_command(_8BIT_MODE);
	#endif
	send_command(CLEAR_DISPLAY);
	send_command(ENTRY_MODE);
	send_command(CURSOR_BLINK_ON);		
}

u8 send_string(char *character){
	u8 i = 0;
	while(*(character + i))
	{
		send_char(*(character+i));
		++i;
		/*if (send_char(*(character+i)) == EXIT_SUCCESS)
		{
			++i;
		}
		else
		{
			return EXIT_FAILURE;
		}		*/
	}
	return EXIT_SUCCESS;
}

u8 go_to(u8 row, u8 column){
	--row;
	--column;
	line = row;
	DDRAM_address = ((row * 0x40) + column);
	if (DDRAM_address > 0x7F)	{
		return EXIT_FAILURE;
	}
	if (row > DISPLAY_LINES)	{
		return EXIT_FAILURE;
	}
	if (column > CHARACTERS_PER_DISPLAY_LINE)	{
		return EXIT_FAILURE;
	}
	else{
		send_command(0x80 + (row * 0x40) + column);
		return EXIT_SUCCESS	;
	}	
}

void create_character(custom_char _char, u8 address){
	send_command(address);
	send_char(_char.row1);
	send_char(_char.row2);
	send_char(_char.row3);
	send_char(_char.row4);
	send_char(_char.row5);
	send_char(_char.row6);
	send_char(_char.row7);
	send_char(_char.row8);
	go_to(1, 1);
}