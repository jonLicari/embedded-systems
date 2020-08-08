/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher and Graphic Demo
 * Note(s): 
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2008-2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/
                  
#include <LPC17xx.H>                    /* NXP LPC17xx definitions            */
//#include "string.h"
#include "GLCD.h"
#include "LED.h"
#include "KBD.h"
//#include "churchs.c"
#include "thicc.c"
#include "rac.c"

#define __FI        1                      /* Font index 16x24               */
#define __USE_LCD   0
extern int menu (void);

int pic;
static uint32_t joy; // The current value of the joystick
static uint32_t oldJoy; // The previous value of the joystick

//extern unsigned char ClockLEDOn;
//extern unsigned char ClockLEDOff;
//extern unsigned char ClockANI;

/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int blinky_main (void) {                       /* Main Program                       */

  LED_Init ();
  GLCD_Init();
	KBD_Init();
	
	pic=0;	//Resets the pic counter everytime the main method is called

  GLCD_Clear  (Yellow);
	GLCD_SetBackColor(Blue);
  GLCD_SetTextColor(White);
	GLCD_DisplayString(0, 0, __FI, "       WELCOME      ");
  GLCD_DisplayString(1, 0, __FI, "       to  the      ");
  GLCD_DisplayString(2, 0, __FI, " SPICY MEDIA PLAYER ");
	GLCD_SetBackColor(Yellow);
	GLCD_SetTextColor(Blue);
	GLCD_DisplayString(5, 0, __FI, "   <-- Scroll -->   ");
	GLCD_DisplayString(6, 0, __FI, "    Push to Exit    ");
		
	while(1) {
	joy = get_button();
	if (oldJoy != joy){
		switch(pic) {
			case 0:
				LED_Off(1);LED_Off(2);
				LED_On(0);	// LED turns on to signify picture number
				//GLCD_Bitmap(0,0,320, 240, (unsigned char*)CHURCHS_pixel_data); // update picture
				GLCD_DisplayString(0, 0, __FI, "                    ");
				GLCD_DisplayString(1, 0, __FI, "!!!!!!!!!!!!!!!!!!!!");
				GLCD_DisplayString(2, 0, __FI, "                    ");
				GLCD_DisplayString(3, 0, __FI, " Well this sucks... ");
				GLCD_DisplayString(4, 0, __FI, "Only 2 pictures are "); 
				GLCD_DisplayString(5, 0, __FI, "here for demo due to");
				GLCD_DisplayString(6, 0, __FI, " memory limitations ");
				GLCD_DisplayString(7, 0, __FI, "                    ");
				GLCD_DisplayString(8, 0, __FI, "!!!!!!!!!!!!!!!!!!!!");
				GLCD_DisplayString(9, 0, __FI, "                    ");
			
				if (get_button() == KBD_RIGHT)   
					pic = 1;		
				else if (get_button() == KBD_LEFT) 
					pic = 2;
				else if (get_button() == KBD_SELECT){
					menu();
					LED_Off(0);LED_Off(1);LED_Off(2);
				}
				
				oldJoy = joy;					// assign the current KBD value to oldJoy to prompt new KBD input
				break;
			case 1:
				LED_Off(0);LED_Off(2);
				LED_On(1);
				GLCD_Bitmap(0,0,320, 240, (unsigned char*)DOGE_pixel_data); // update picture
			
				if (get_button() == KBD_RIGHT)   
					pic = 2;		
				else if (get_button() == KBD_LEFT) 
					pic = 0;
				else if (get_button() == KBD_SELECT){
					LED_Off(0);LED_Off(1);LED_Off(2);
					menu();
				}
									
				oldJoy = joy;
				break;					
			case 2:
				LED_Off(0);LED_Off(1);
				LED_On(2);
				GLCD_Bitmap(0,0,320, 240, (unsigned char*)RAC_pixel_data); // update picture			
			
				if (get_button() == KBD_RIGHT)   
					pic = 0;		
				else if (get_button() == KBD_LEFT) 
					pic = 1;
				else if (get_button() == KBD_SELECT) {
					menu();
					LED_Off(0);LED_Off(1);LED_Off(2);
				}
				
				oldJoy = joy;
				break;					
			
		}
	}
}

  
  //SysTick_Config(SystemCoreClock/100);  /* Generate interrupt every 10 ms     */

//	return(0);
}
