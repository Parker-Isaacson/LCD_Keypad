/*
MKP.h is rellies on LCD.h so it is included here and does not need to be included in main.c
MKP.h includes LCD.h
*/

#include "stm32f446xx.h"
#include "LCD.h"
#include <stdlib.h>

// First Itteration of this function
// this function will run one and should be put inside a loop to be constantly called
// it will return the value on the keypad, and will return 'Z'
char MKP_CheckPressed(void)
{
	// Power a row and then check if any other collum is powered, only one row can be powered at once
	//Turns on first row
	
	GPIOA->ODR &= ~0x0010;
	GPIOB->ODR &= ~0x0001;
	GPIOC->ODR &= ~0x0003;
	GPIOC->ODR |= 0x0001;
	
	// Checks collumns for being powered
	if ((GPIOA->IDR & 0x0002) == 0x0002) { return '1'; }
	else if ((GPIOC->IDR & 0x0020) == 0x0020) { return '2'; }
	else if ((GPIOC->IDR & 0x0040) == 0x0040) { return '3'; }
	else if ((GPIOC->IDR & 0x0100) == 0x0100) { return 'A'; }
	
	//Turns on second row
	GPIOA->ODR &= ~0x0010;
	GPIOB->ODR &= ~0x0001;
	GPIOC->ODR &= ~0x0003;
	GPIOC->ODR |= 0x0002;
	
	// Checks collumns for being powered
	if ((GPIOA->IDR & 0x0002) == 0x0002) { return '4'; }
	else if ((GPIOC->IDR & 0x0020) == 0x0020) { return '5'; }
	else if ((GPIOC->IDR & 0x0040) == 0x0040) { return '6'; }
	else if ((GPIOC->IDR & 0x0100) == 0x0100) { return 'B'; }
	
	//Turns on third row
	GPIOA->ODR &= ~0x0010;
	GPIOB->ODR &= ~0x0001;
	GPIOC->ODR &= ~0x0003;
	GPIOB->ODR |= 0x0001;
	
	// Checks collumns for being powered
	if ((GPIOA->IDR & 0x0002) == 0x0002) { return '7'; }
	else if ((GPIOC->IDR & 0x0020) == 0x0020) { return '8'; }
	else if ((GPIOC->IDR & 0x0040) == 0x0040) { return '9'; }
	else if ((GPIOC->IDR & 0x0100) == 0x0100) { return 'C'; }
	
	//Turns on fourth row
	GPIOA->ODR &= ~0x0010;
	GPIOB->ODR &= ~0x0001;
	GPIOC->ODR &= ~0x0003;
	GPIOA->ODR |= 0x0010;
	
	// Checks collumns for being powered
	if ((GPIOA->IDR & 0x0002) == 0x0002) { return '*'; }
	else if ((GPIOC->IDR & 0x0020) == 0x0020) { return '0'; }
	else if ((GPIOC->IDR & 0x0040) == 0x0040) { return '#'; }
	else if ((GPIOC->IDR & 0x0100) == 0x0100) { return 'D'; }
	
	if ((GPIOC->IDR & 0x2000) == 0) // Checks if PC13 is low
			LCD_ClearScreen();
	
	return 'Z';
}

void MKP_PrintToFirstLine(void)
{
	char* str;
	str = (char*) malloc(sizeof(char));
	*str = MKP_CheckPressed();
	if (*str != 'Z')
	{
	//LCD_ClearScreen();
	LCD_PrintToFirstLine(str);
	}
	free(str);
}

void MKP_Initialise(void)
{
	/* Connections for the Membrane Thing
	r1 -> PC0, r2 -> PC1, r3 -> PBO, r4 -> PA4
	c1 -> PA1, c2 -> PC5, c3 -> PC6, c5 -> PC8
	puol all collumns down on the breadboard
	Check which intersection is low */
	// Turns on GPIOA, GPIOB, GPIOC
	RCC->AHB1ENR |= 0x00000007;
	
	// Clears the Pins, and sets collumn pins to input
	GPIOA->MODER &= ~0x0000030C;
	GPIOB->MODER &= ~0x00000003;
	GPIOC->MODER &= ~0x00033c0f;
	
	// Sets ROW pins to be output
	GPIOA->MODER |= 0x00000100;
	GPIOB->MODER |= 0x00000001;
	GPIOC->MODER |= 0x00000005;
	
	// Turns needed input ports off
	GPIOA->ODR &= ~0x0010;
	GPIOB->ODR &= ~0x0001;
	GPIOC->ODR &= ~0x0003;
	/* Could not get it to work so we just pull down physcially
	// Makes output ports pull down
	GPIOA->PUPDR &= ~0x0000000c;
	GPIOC->PUPDR &= ~0x00033c00;
	
	GPIOA->PUPDR |= 0x00000002;
	GPIOC->PUPDR |= 0x00022800;
		*/
	GPIOC->MODER &= ~0x0C000000; //clears PC13 to input	

	LCD_ClearScreen();
	LCD_PrintToFirstLine("Startup...");
}
