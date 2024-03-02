#include "stm32f446xx.h"
#include "LCD.h"

int main(void)
{
	LCD_Initialise();
	
	LCD_ClearScreen();
	LCD_Print("LCDHW5",0,'M');

	while (1)
	{
	}
}
