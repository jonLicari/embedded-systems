/* ------ GAME.h ------ */
#ifndef __bullet_h
#define __bullet_h

#include "GLCD.h"

#define __FI        1                      /* Font index 16x24               */
#define __USE_LCD   0

#define yPos 40 // find actual value
#define y0 199	// y coordinae of row 0 200
#define y1 176	// y coordinae of row 1
#define y2 150	// y coordinae of row 2
#define y3 124	// y coordinae of row 3
#define y4 100	// y coordinae of row 4
#define y5 80	// y coordinae of row 5
#define x0 47	// x coordinate of coloumn 0
#define x1 96	// x coordinate of coloumn 1
#define x2 147	// x coordinate of coloumn 2
#define x3 190	// x coordinate of coloumn 3
#define x4 235	// x coordinate of coloumn 4
#define x5 285	// x coordinate of coloumn 5

extern void delay(int d);
extern int menu (void);  //Uncomment for use with main Media Center project
int arr[6][6]= {{3,3,3,3,3,3},
							  {3,3,3,3,3,3},
							  {3,3,3,3,3,3},
							  {3,3,3,3,3,3},
							  {3,3,3,3,3,3},
							  {3,3,3,3,3,3}
};	// 3 is the "zero" value
static int xc,yc,yt,yd;
//extern int menu (void); 

void grid(void) {
	GLCD_Clear(Black);
	GLCD_SetBackColor(Black);
	GLCD_SetTextColor(Red);
	GLCD_DisplayString(0, 0, __FI, "     Connect  4     ");
	GLCD_DisplayString(3, 1, __FI, "|__|__|__|__|__|__| ");
	GLCD_DisplayString(4, 1, __FI, "|__|__|__|__|__|__| ");
	GLCD_DisplayString(5, 1, __FI, "|__|__|__|__|__|__| ");
	GLCD_DisplayString(6, 1, __FI, "|__|__|__|__|__|__| ");
	GLCD_DisplayString(7, 1, __FI, "|__|__|__|__|__|__| ");
	GLCD_DisplayString(8, 1, __FI, "|__|__|__|__|__|__| ");
	GLCD_DisplayString(9, 1, __FI, " 0  1  2  3  4  5   "); // correct spacing
}

void pointer (int x, int y) {
	GLCD_SetTextColor(Red);
	GLCD_PutPixel(x,y); // reference pixel
	
	GLCD_PutPixel(x-1,y-1);
	GLCD_PutPixel(x,y-1);
	GLCD_PutPixel(x+1,y-1);
	
	GLCD_PutPixel(x-2,y-2);
	GLCD_PutPixel(x-1,y-2);
	GLCD_PutPixel(x,y-2);
	GLCD_PutPixel(x+1,y-2);
	GLCD_PutPixel(x+2,y-2);
}

void cursor (int xc) {
	// #define at the top of the document file
	switch (xc) {
		case 0:
			pointer(x0, yPos);
			break;
		case 1:
			pointer(x1, yPos);
			break;
		case 2:
			pointer(x2, yPos);
			break;
		case 3:
			pointer(x3, yPos);
			break;
		case 4:
			pointer(x4, yPos);
			break;
		case 5:
			pointer(x5, yPos);
			break;
	}
}

void icon (int w, int h) { // 3x3 square icon
	GLCD_PutPixel(w,h); // reference pixel
	GLCD_PutPixel(w-1,h);GLCD_PutPixel(w+1,h);
	GLCD_PutPixel(w-1,h-1);GLCD_PutPixel(w, h-1); GLCD_PutPixel(w+1,h-1);
	GLCD_PutPixel(w-1,h+1);GLCD_PutPixel(w, h+1);GLCD_PutPixel(w+1, h+1);
}

void token(int play, int col, int row) {
	int cola, rowa; // coloumn and row arguments to be passed to icon()
	if ((play%2) == 1) // Player 1 token
		GLCD_SetTextColor(Yellow);
	else // Player 2 token
		GLCD_SetTextColor(Blue);
	// Determine x coordinate of token
	if (col == 0)
		cola = x0;
	else if (col == 1)
		cola = x1;
	else if (col == 2)
		cola = x2;
	else if (col == 3)
		cola = x3;
	else if (col == 4)
		cola = x4;
	else if (col == 5)
		cola = x5;
	// Determine y coordinate of token
	if (row == 0)
		rowa = y0;
	else if (row == 1)
		rowa = y1;
	else if (row == 2)
		rowa = y2;
	else if (row == 3)
		rowa = y3;
	else if (row == 4)
		rowa = y4;
	else if (row == 5)
		rowa = y5;
	// Create icon on the designated coordinate
	icon(cola, rowa);
}

void drop(int play, int x, int row) {
	if (arr[row][x] == 3) { // 3 is the empty state
		arr[row][x] = (play%2); // set occupy flag 0 -> player 2, 1 -> player 1
		token(play, x, row);
	}
	else {
		row++;
		drop(play, x, row); // try the higher row
	}
		
}

void victory (int player) {
	GLCD_Clear(Yellow); GLCD_SetTextColor(Red); GLCD_SetBackColor(Yellow);
	GLCD_DisplayString(1, 0, __FI, "  CONGRATULATIONS!  ");
	GLCD_DisplayString(4, 0, __FI, "  Player   Wins!!!  ");
	if (player == 2)
		GLCD_DisplayString(4, 9, __FI, "2");
	else
		GLCD_DisplayString(4, 9, __FI, "1");
	
	delay(1000);
	menu(); //Uncomment for use with MEDIA CENTER
} 

int check (void) { // Checks if game is won
	// VERTICAL CHECK
	for (xc=0; xc<6; xc++) {
		for (yc=0; yc<3; yc++) {
			if ((arr[yc][xc] == 1)&&(arr[yc+1][xc] == 1)&&(arr[yc+2][xc] == 1)&&(arr[yc+3][xc] == 1)){
				victory(1);
			}
			else if ((arr[yc][xc] == 0)&&(arr[yc+1][xc] == 0)&&(arr[yc+2][xc] == 0)&&(arr[yc+3][xc] == 0)){
				victory(2);
			}
		}
	}
	//HORIZONTAL CHECK
	for (yc=0; yc<6; yc--) {
		for (xc=0; xc<3; xc++) {
			if ((arr[yc][xc] == 1)&&(arr[yc][xc+1] == 1)&&(arr[yc][xc+2] == 1)&&(arr[yc][xc+3] == 1)){
				victory(1);
			}
			else if ((arr[yc][xc] == 0)&&(arr[yc][xc+1] == 0)&&(arr[yc][xc+2] == 0)&&(arr[yc][xc+3] == 0)){
				victory(2);
			}
		}
	}
	//DIAGONAL CHECK
	for (xc=0; xc<3; xc++) {
		// TOP DOWN CHECK
		for (yt=5; yt>2; yt--) {
			if ((arr[yt][xc] == 1)&&(arr[yt-1][xc+1] == 1)&&(arr[yt-2][xc+2] == 1)&&(arr[yt-3][xc+3] == 1)){
				victory(1);
			}
			else if ((arr[yt][xc] == 0)&&(arr[yt-1][xc+1] == 0)&&(arr[yt-2][xc+2] == 0)&&(arr[yt-3][xc+3] == 0)){
				victory(2);
			}
		} // end top down check
		// BOTTOM UP CHECK
		for (yd=0; yd<3; yd++) {
			if ((arr[yd][xc] == 1)&&(arr[yd+1][xc+1] == 1)&&(arr[yd+2][xc+2] == 1)&&(arr[yd+3][xc+3] == 1)){
				victory(1);
			}
			else if ((arr[yd][xc] == 0)&&(arr[yd+1][xc+1] == 0)&&(arr[yd+2][xc+2] == 0)&&(arr[yd+3][xc+3] == 0)){
				victory(2);
			}
		}// end bottom up check
	} // end diagonal check
	return 0;
} // end check();

#endif
