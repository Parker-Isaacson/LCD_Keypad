/* How to set up pins for this header file to work
Port on LCD -> Pin on STM32F446RE
en -> PA6, rw -> PA7, rs -> PB6

d0 -> PA10, d1 -> PB3, d2 -> PB5, d3 -> PB4, d4 -> Pb10, d5 -> PA8, d6 -> PA9, d7 -> PC7

3v3 to VDD and E
GND to VSS, V0, and k
*/
#include "stm32f446xx.h"

// Delay functionr required for allowing the LCD to process data
// Works for 16Mhz and input is milliseconds of delay
void delay(int n)
{
	int i;
	for (; n > 0; n--)
		for (i = 0; i < 3195; i++) ;
}

// Function that sends 8BIT data to the LCD screen
// Input is the char of the data you want to send, REQUIRES proper pin set up, see top
void LCD_Printdata(unsigned char data) // data = 8 bit hexidecimal data
{
	// Bit 0 => d0 = PA10
	if( ( data & 0x01 ) == 0x01 ) { GPIOA->ODR |= ( 1 << 10 ); }
	else { GPIOA->ODR &= ~( 1 << 10 ); }
	// Bit 1 => d1 = PB3
	if( ( data & 0x02 ) == 0x02 ) { GPIOB->ODR |= ( 1 << 3 ); }
	else { GPIOB->ODR &= ~( 1 << 3 ); }
	// Bit 2 => d2 = PB5
	if( ( data & 0x04 ) == 0x04 ) { GPIOB->ODR |= ( 1 << 5 ); }
	else { GPIOB->ODR &= ~( 1 << 5 ); }
	// Bit 3 => d3 = PB4
	if( ( data & 0x08 ) == 0x08 ) { GPIOB->ODR |= ( 1 << 4 ); }
	else { GPIOB->ODR &= ~( 1 << 4 ); }
	// Bit 4 => d4 = PB10
	if( ( data & 0x10 ) == 0x10 ) { GPIOB->ODR |= ( 1 << 10 ); }
	else { GPIOB->ODR &= ~( 1 << 10 ); }
	// Bit 5 => d5 = PA8
	if( ( data & 0x20 ) == 0x20 ) { GPIOA->ODR |= ( 1 << 8 ); }
	else { GPIOA->ODR &= ~( 1 << 8 ); }
	// Bit 6 => d6 = PA9
	if( ( data & 0x40 ) == 0x40 ) { GPIOA->ODR |= ( 1 << 9 ); }
	else { GPIOA->ODR &= ~( 1 << 9 ); }
	// Bit 7 => d7 = PC7
	if( ( data & 0x80 ) == 0x80 ) { GPIOC->ODR |= ( 1 << 7 ); }
	else { GPIOC->ODR &= ~( 1 << 7 ); }
}

// Passing data to data register of LCD
// Printing data to LCD
void LCD_Data(unsigned char data)
{
	LCD_Printdata(data); // Pass the 8bit data to data line's of LCD
	GPIOA->ODR = GPIOA->ODR & ~( 1 << 7 ); // Drive R/W line of LCD to be LOW
	GPIOB->ODR = GPIOB->ODR | ( 1 << 6 ); // Drive RS line of LCD to be HIGH
	GPIOA->ODR = GPIOA->ODR | ( 1 << 6 ); // Drive EN line of LCD to be HIGH
	delay(10); // Delay
	GPIOA->ODR = GPIOA->ODR & ~( 1 << 6); // Drive EN line of LCD to be LOW
}

// Passing data to Instruction register of LCD
// Pass commands to LCD
void LCD_Command(unsigned char command)
{
	LCD_Printdata(command); // Pass the 8bit data to data line's of LCD
	GPIOA->ODR = GPIOA->ODR & ~( 1 << 7 ); // Drive R/W line of LCD to be LOW
	GPIOB->ODR = GPIOB->ODR & ~( 1 << 6 ); // Drive RS line of LCD to be LOW
	GPIOA->ODR = GPIOA->ODR | ( 1 << 6 ); // Drive EN line of LCD to be HIGH
	delay(10); // Delay
	GPIOA->ODR = GPIOA->ODR & ~( 1 << 6); // Drive EN line of LCD to be LOW
}

// Printing word of data to LCD
void LCD_String(char *str, unsigned char length)
{
	// LCD Data function in loop for printing string or word of data to LCD
	unsigned char i = 0;
	for ( i = 0; i < length; i++)
	{
		LCD_Data(str[i]);
	}
}

void LCD_GPIO_Initialise(void)
{
	// Configuration of all necicary pins
	RCC->AHB1ENR = RCC->AHB1ENR | ( 1 << 0 ) | ( 1 << 1 ) | ( 1 << 2 ); // Clock of GPIO's Enabled GPIOA, GPIOB, GPIOC
	// Output is '01', so following sets the first placement to 1, and second placement to 0
	GPIOA->MODER = GPIOA->MODER | ( 1 << 16 ) | ( 1 << 18 ) | ( 1 << 20 );
	GPIOA->MODER = GPIOA->MODER & ~( 1 << 17 ) & ~( 1 << 19 ) & ~( 1 << 21); // PA8, PA9, PA10 as output
	
	GPIOB->MODER = GPIOB->MODER | ( 1 << 6 ) | ( 1 << 8 ) | ( 1 << 10 ) | ( 1 << 20 );
	GPIOB->MODER = GPIOB->MODER & ~( 1 << 7 ) & ~( 1 << 9 ) & ~( 1 << 11 ) & ~( 1 << 21 ); // PB3, PB4, PB5, PB10 as output
	
	GPIOC->MODER = GPIOC->MODER | ( 1 << 14 );
	GPIOC->MODER = GPIOC->MODER & ~( 1 << 15); // PC7 as output
	
	GPIOA->MODER = GPIOA->MODER | ( 1 << 12 ) | ( 1 << 14 );
	GPIOA->MODER = GPIOA->MODER & ~( 1 << 13 ) & ~(  1 << 15 ); // PA6, PA7 as output
	
	GPIOB->MODER = GPIOB->MODER | ( 1 << 12 );
	GPIOB->MODER = GPIOB->MODER & ~( 1 << 13 ); // PB6 as output
}

// Initializing the LCD
void LCD_Initialise(void)
{
	LCD_GPIO_Initialise();
	LCD_Command(0x38); // configuring lcd in 16 column and 2 rows format
	LCD_Command(0x06); // display on, cursor off
	LCD_Command(0x0C); // autoincrement cursor
	LCD_Command(0x01); // clearscreen
}

void LCD_PrintToFirstLine(char *str)
{
	LCD_Command(0x80); // Moves cursor to first row first collumn
	int str_length, i;
	for ( i = 0; str[i] != '\0'; i++) { str_length++; }
	LCD_String(str, str_length);
}

void LCD_PrintToSecondLine(char *str)
{
	LCD_Command(0xc0); //Moves cursor to second row first collum
	int str_length, i;
	for ( i = 0; str[i] != '\0'; i++) { str_length++; }
	LCD_String(str, str_length);
}

void LCD_ClearScreen(void) { LCD_Command(0x01); delay(5); }

// first input is the string to be printed, second is on which line (First : 0, Second : 1, auto is first), third input is justfity (Right : R, Left : L, Middle : M, auto is right)
void LCD_Print( char *str, int line, char justify)
{
	(line == 1) ? LCD_Command(0xc0) : LCD_Command(0x80);
	int str_length, i, cursor_move;
	for ( i = 0; str[i] != '\0'; i++) { str_length++; }
	if (justify == 'R')
	{
		cursor_move = 16 - str_length;
		for ( i = 0; i < cursor_move; i ++)
			LCD_Command(0x14); // Moves cursor to the right by one
	}
	else if (justify == 'M')
	{
		cursor_move = (16 - str_length) / 2;
		for ( i = 0; i < cursor_move; i ++)
			LCD_Command(0x14); // Moves cursor to the right by one
	}
	else
	{
		cursor_move = 0;
		for ( i = 0; i < cursor_move; i ++)
			LCD_Command(0x14); // Moves cursor to the right by one
	}
	LCD_String(str, str_length);
}
