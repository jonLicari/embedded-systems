/**********************************************************************/
/* MEDIA GALLERY USER INTERFACE																				*/
/**********************************************************************/

#include <stdio.h>
#include "LPC17xx.h"
#include "GLCD.h"
#include "KBD.h"
#include "bullet.h"

#define __FI        1                      /* Font index 16x24               */
#define __USE_LCD   0
#define row4 176 // Define the bullet row position for menu item 4

extern int audio_main(void);
extern int blinky_main(void);
extern void game_main(void);

uint32_t joy; // The current value of the joystick
uint32_t oldJoy; // The previous value of the joystick
int sel; // Menu selector variable
//int run = 0;

void delay(int d) {
	int ctr,val=0;
	for (ctr=0;ctr<d;ctr++)
		val++;
}

int menu(void) {
	KBD_Init();																 /* Joystick Initialization				*/

#ifdef __USE_LCD
  GLCD_Init();                               /* Initialize graphical LCD (if enabled */

  GLCD_Clear(Black);                         /* Clear graphical LCD display   */
  GLCD_SetBackColor(Magenta);
  GLCD_SetTextColor(Yellow);
  GLCD_DisplayString(0, 0, __FI, "     COE718 Demo    ");
	GLCD_SetTextColor(White);
  GLCD_DisplayString(1, 0, __FI, "       PROJECT      ");
  GLCD_DisplayString(2, 0, __FI, "  Select an Option  "); /* Change */
  GLCD_SetBackColor(Black);
  GLCD_SetTextColor(Red);
	GLCD_DisplayString(4, 0, __FI, "  mp3 Audio Player  ");
	GLCD_DisplayString(5, 0, __FI, "    Image Viewer    ");
	GLCD_DisplayString(6, 0, __FI, "     Game Center    ");
	GLCD_DisplayString(7, 0, __FI, "      Credits       ");

	
/* ----- MENU SELECTION FUNCTIONALITY ----- */
		sel=1;
bullet(5, 105, 1);	
while(1) {
	joy = get_button();
	if (oldJoy != joy){
		if (get_button() == KBD_DOWN) {  // On joystick move down
			switch(sel) {
				case 1:
					bullet(5, 105, 0);	// hide bullet
					sel = 2;							// Update select
					bullet(5, 127, 1);				// show next bullet
					oldJoy = joy;					// assign the current KBD value to oldJoy to prompt new KBD input
					break;
				case 2:
					bullet(5, 127, 0);	//hide bullet
					sel = 3;							// Update select
					bullet(5, 149, 1);				//show next bullet
					oldJoy = joy;
					break;					
				case 3:
					bullet(5, 149, 0);	//hide bullet
					sel = 4;							// Update select
					bullet(5, row4, 1);				//show next bullet
					oldJoy = joy;
					break;					
				//case 4:	i don't need a case 4 because there's no lower menu item
			}
		}
		else if (get_button() == KBD_UP) { //On joystick move up
			switch(sel) {
				//case 1: i don't need a case 1 because there is nothing higher than thisa menu item
				case 2:
					bullet(5, 127, 0);	// hide current bullet
					sel = 1;							// update select
					bullet(5, 105, 1);				// show next bullet
					oldJoy = joy;
					break;
				case 3:
					bullet(5, 149, 0);	// hide current bullet
					sel = 2;							// update select
					bullet(5, 127, 1);				// show next bullet
					oldJoy = joy;
					break;
				case 4:	
					bullet(5, row4, 0);	// hide current bullet
					sel = 3;							// update select
					bullet(5, 149, 1);				// show next bullet
					oldJoy = joy;
					break;
			}
		}
		else if (get_button() == KBD_SELECT) { // On joystick select
			switch(sel) {
				case 1:
					GLCD_DisplayString(9, 0, __FI, "  Loading  Player   ");
					oldJoy = joy;
					audio_main();
					break;
				case 2:
					GLCD_DisplayString(9, 0, __FI, "  Loading  Gallery  ");
					oldJoy = joy;
					blinky_main();
					
					break;
				case 3:
					GLCD_DisplayString(9, 0, __FI, "   Loading  Game    ");
					oldJoy = joy;
					game_main();
					break;
				case 4:	
					GLCD_DisplayString(9, 0, __FI, "      Credits       ");
					GLCD_Clear(Black);
					GLCD_DisplayString(0, 0, __FI, "      jlicari       ");
					GLCD_DisplayString(2, 0, __FI, "      500691548     ");
					GLCD_DisplayString(4, 0, __FI, "      COE 718       ");
					delay(1000);
					menu(); // return automatically to menu
					oldJoy = joy;
					break;
			}
		}	
	}
}
#endif
}

int main (void)
{
	int ld=0;				//Value of the Loading bargraph
	GLCD_Init();                               /* Initialize graphical LCD (if enabled */
  GLCD_Clear(Black);                         /* Clear graphical LCD display   */
  GLCD_SetBackColor(Magenta);
  GLCD_SetTextColor(White);
	GLCD_DisplayString(0, 0, __FI, "       WELCOME      ");
  GLCD_DisplayString(1, 0, __FI, "       to  the      ");
  GLCD_DisplayString(2, 0, __FI, " SPICY MEDIA PLAYER ");
	GLCD_SetBackColor(Black);
	GLCD_SetTextColor(Red);
	GLCD_DisplayString(5, 0, __FI, "     loading...     ");
	GLCD_SetBackColor(Yellow);
	for (ld=0;ld<176;ld++) {
		GLCD_Bargraph(144,149,376,20,ld);
		delay(100);
	}
	
	menu();
}
