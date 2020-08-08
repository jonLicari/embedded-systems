/* ------------- CONNECT FOUR GAME ------------- */
// Project designed and implemented by jlicari
// To be combined with the COE718 Media Center
#include <LPC17xx.H>                    /* NXP LPC17xx definitions            */

#include "GLCD.h"
#include "LED.h"
#include "KBD.h"
#include "game.h"

#define __FI        1                      /* Font index 16x24               */
#define __USE_LCD   0

extern int menu (void);  //Uncomment for use with main Media Center project
extern int arr[6][6];
static int ctr,x=0; // ctr - turn counter, x - x coordinate value
static uint32_t joy;
static uint32_t oldJoy;
static int fx,fy; // array fill counters

void game_main(void) {
	GLCD_Init(); KBD_Init(); LED_Init();
	GLCD_Clear(Black);
	grid();
	cursor(0);	// coloumn 0-5
	// At this point the ui is set up. Game mechanics follows
	//The array will empty or "reset" every time game main is called
	for (fy=0;fy<6;fy++) {
		for (fx=0;fx<5;fx++) {
			arr[fy][fx] = 3;
		}
	}		
	
	ctr = 1; // Player turn counter used with modulus operator to denote turn
	// ctr%2 --> 1 for Player 1 || 0 for Player 2
	
	/* --- SCROLL --- */
	while(1) {
		// LED Displays Current Player Turn
		if (ctr%2 == 0) { // if player 2
			LED_On(1); LED_Off(0);}
		else { // if player 1
			LED_On(0); LED_Off(1);}
			
		joy = get_button();

		if (oldJoy != joy) {
			oldJoy=0;
			// Joystick Controls
			if (joy == KBD_LEFT) {
				if (x>0) {
					x--; // Move x coordinate left
					GLCD_DisplayString(1, 0, __FI, "                    ");// Clear previous cursor
					cursor(x); // set new cursor
					oldJoy = joy;
				}
			}
			else if (joy == KBD_RIGHT) {
				if (x<5){
					x++; // move cursor right
					GLCD_DisplayString(1, 0, __FI, "                    ");
					cursor(x);
					oldJoy = joy;
				}
			}
			else if (joy == KBD_UP) {
				LED_Off(0);LED_Off(1);
				menu();  //Exits Connect 4 and returns to main menu
				
				//GLCD_DisplayString(0, 0, __FI, "You exited the game ");
			}/*
			else if (joy == KBD_SELECT) {
				oldJoy = joy;
			}*/
			else if (joy == KBD_DOWN) {
				drop(ctr, x, 0);		// drop(turn counter, x location, bottom row)
				if (ctr>6){
					check();
					ctr++; // increment the turn counter
					oldJoy = joy;
				} // Checks to see if any player has won the game
				else {
					ctr++; // increment the turn counter
					oldJoy = joy;
				}
			}		
		} // end while (joy != 0)
	} // end while(1)
} // end game_main()
/*
int main(void){
	game_main();
	return 0;
}
*/
