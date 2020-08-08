#include <LPC17xx.H>                    /* NXP LPC17xx definitions            */
#include "GLCD.h"

#define __FI        1                      /* Font index 16x24               */
#define __USE_LCD   0

int main(void) {
	GLCD_Init();
	GLCD_Clear(White);
	GLCD_SetTextColor(Red);
	GLCD_DisplayString(0, 0, __FI, "Player 1 Turn");
	GLCD_DisplayString(3, 1, __FI, "A|__|__|__|__|__|__|  ");
	GLCD_DisplayString(4, 1, __FI, "B|__|__|__|__|__|__|  ");
	GLCD_DisplayString(5, 1, __FI, "C|__|__|__|__|__|__|  ");
	GLCD_DisplayString(6, 1, __FI, "D|__|__|__|__|__|__|  ");
	GLCD_DisplayString(7, 1, __FI, "E|__|__|__|__|__|__|  ");
	GLCD_DisplayString(8, 1, __FI, "F|__|__|__|__|__|__|  ");
	GLCD_DisplayString(9, 1, __FI, "  0  1  2  3  4  5    "); // correct spacing


}
