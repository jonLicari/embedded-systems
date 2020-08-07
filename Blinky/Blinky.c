/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher
 * Note(s): __USE_LCD   - enable Output on LCD, uncomment #define in code to use
 *  				for demo (NOT for analysis purposes)
 *----------------------------------------------------------------------------
 * Copyright (c) 2008-2011 Keil - An ARM Company.
 * Name: Jonathan Licari
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include "LPC17xx.H"                       
#include "GLCD.h"
#include "LED.h"
#include "ADC.h"
#include "KBD.h"

#define __FI        1                      /* Font index 16x24               */

#define __USE_LCD   0										/* Uncomment to use the LCD */

//ITM Stimulus Port definitions for printf //////////////////
#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))

#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000

struct __FILE { int handle;  };
FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f) {
  if (DEMCR & TRCENA) {
    while (ITM_Port32(0) == 0);
    ITM_Port8(0) = ch;
  }
  return(ch);
}
/////////////////////////////////////////////////////////

char text[10];

//Use to trace the pot values in Debug
uint16_t ADC_Dbg;
uint32_t KBD_Dbg; //Traces the joystick  values in debug; joystick is 32 bit peripheral

/* Import external variables from IRQ.c file                                  */
extern uint8_t  clock_ms;


/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
  uint32_t ad_avg = 0;
  uint16_t ad_val = 0, ad_val_ = 0xFFFF;

  LED_Init();                                /* LED Initialization            */
  ADC_Init();                                /* ADC Initialization            */
	KBD_Init();																 /* Joystick Initialization				*/

#ifdef __USE_LCD
  GLCD_Init();                               /* Initialize graphical LCD (if enabled */

  GLCD_Clear(White);                         /* Clear graphical LCD display   */
  GLCD_SetBackColor(Blue);
  GLCD_SetTextColor(Yellow);
  GLCD_DisplayString(0, 0, __FI, "     COE718 Demo    ");
	GLCD_SetTextColor(White);
  GLCD_DisplayString(1, 0, __FI, "       Blinky.c     ");
  GLCD_DisplayString(2, 0, __FI, "  Turn pot for LEDs "); /* Change */
  GLCD_SetBackColor(White);
  GLCD_SetTextColor(Blue);
  GLCD_DisplayString(6, 0, __FI, "AD value:            ");
	GLCD_DisplayString(7, 0, __FI, "Direction:         ");
#endif

  SysTick_Config(SystemCoreClock/100);       /* Generate interrupt each 10 ms */

  while (1) {                                /* Loop forever                  */
		
	

    /* AD converter input                                                     */
    if (AD_done) {                           /* If conversion has finished    */
      AD_done = 0;

      ad_avg += AD_last << 8;                /* Add AD value to averaging     */
      ad_avg ++;
      if ((ad_avg & 0xFF) == 0x10) {         /* average over 16 values        */
        ad_val = (ad_avg >> 8) >> 4;         /* average devided by 16         */
        ad_avg = 0;
      }
    }
    
		ADC_Dbg = ad_val;
		
    if (ad_val ^ ad_val_) {                  /* AD value changed              */
      ad_val_ = ad_val;
			
      sprintf(text, "0x%04X", ad_val);       /* format text for print out     */
			
#ifdef __USE_LCD
      GLCD_SetTextColor(Red);
      GLCD_DisplayString(6,  9, __FI,  (unsigned char *)text);
			GLCD_SetTextColor(Green);
      GLCD_Bargraph (144, 7*24, 176, 20, (ad_val >> 2)); /* max bargraph is 10 bit */
#endif
    }

    /* Print message with AD value every 10 ms                               */
    if (clock_ms) {
      clock_ms = 0;

      printf("AD value: %s\r\n", text);
    }
		if (get_button() == KBD_UP){ //If joystick was moved upwards
			LED_On(0); //P1.28 turns on
			GLCD_DisplayString(7, 11, __FI, "UP     "); //display String direction to LCD
			LED_Off(0); // Turns off P1.28 
		}
		else if (get_button() == KBD_RIGHT){ //If joystick was moved right
			LED_On(2); //P1.31 turns on
			GLCD_DisplayString(7, 11, __FI, "RIGHT"); //display String direction to LCD
			LED_Off(2); // Turns off P1.31
		}
		else if (get_button() == KBD_LEFT){ //If joystick was moved left
			LED_On(4); //P2.3 turns on
			GLCD_DisplayString(7, 11, __FI, "LEFT "); //display String direction to LCD
			LED_Off(4); // Turns off P2.3
		}
		else if (get_button() == KBD_DOWN){ //If joystick was moved down
			LED_On(7); //P1.31 turns on
			GLCD_DisplayString(7, 11, __FI, "DOWN "); //display String direction to LCD
			LED_Off(7); // Turns off P2.6
		}
		else if (get_button() == KBD_SELECT){ //If joystick was pressed 
			LED_On(0);LED_On(1);LED_On(2);LED_On(3);LED_On(4);LED_On(5);LED_On(6);LED_On(7);  //All LEDs on
			GLCD_DisplayString(7, 11, __FI, "PUSH "); //display String direction to LCD
			LED_Off(0);LED_Off(1);LED_Off(2);LED_Off(3);LED_Off(4);LED_Off(5);LED_Off(6);LED_Off(7);  //All LEDs off
		}				
  }
}
