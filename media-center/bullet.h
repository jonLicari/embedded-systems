/* *************************************** */
/* Definitions for the user interface      */
/* *************************************** */

#ifndef __BULLET_H
#define __BULLET_H

#include "GLCD.h"

void bullet (int x, int y, int colour) {
	if (colour == 1) 
		GLCD_SetTextColor(Red);
	else
		GLCD_SetTextColor(Black);
	
	//Bottom Square
	GLCD_PutPixel(x,y);
	GLCD_PutPixel(x+1,y);
	GLCD_PutPixel(x,y+1);
	GLCD_PutPixel(x+1,y+1);
	//Left square
	GLCD_PutPixel(x-2,y+2);
	GLCD_PutPixel(x-1,y+2);
	GLCD_PutPixel(x-2,y+3);
	GLCD_PutPixel(x-1,y+3);
	//Top Square
	GLCD_PutPixel(x,y+4);
	GLCD_PutPixel(x,y+5);
	GLCD_PutPixel(x+1,y+4);
	GLCD_PutPixel(x+1,y+5);
	//Right square
	GLCD_PutPixel(x+2,y+2);
	GLCD_PutPixel(x+2,y+3);
	GLCD_PutPixel(x+3,y+2);
	GLCD_PutPixel(x+3,y+3);
}

/*
void hide_bullet(int x, int y) {
	GLCD_SetTextColor(0x0000);
	bullet(x, y);
} */
#endif
