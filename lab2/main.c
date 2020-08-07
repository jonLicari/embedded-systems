/*_______________________________________________
 * Project: Lab 2
 * Course: COE 718
 * Author: Jlicari
 * Due Date: 09/26/2018
 * Purpose: Main program for lab 2 allows for
 * joystick to switch between different methods
 * capable of lighting LEDs
 *_______________________________________________*/
 
// Definitions & Inclusions
 
#include <stdio.h>
#include "LPC17xx.H"                       
#include "GLCD.h"
#include "LED.h"
#include "ADC.h"
#include "KBD.h"
#include "bitband.h"

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
  GLCD_DisplayString(6, 0, __FI, "Method:            ");
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

    /* Print message every 10 ms                               */
    if (clock_ms) {
      clock_ms = 0;

    }
		/* ---- METHOD 1: FUNCTION ---- */
		if (get_button() == KBD_UP){ //If joystick was moved upwards
			LED_On(0); LED_On(1); //P1.28 & P1.29 turns on
			GLCD_DisplayString(6, 11, __FI, "Function"); // display the LED function method
			GLCD_DisplayString(7, 11, __FI, "UP     "); // display String direction to LCD
			LED_Off(0); LED_Off(1); // Turns off P1.28 & P1.29
		}
		/* ---- METHOD 2: MASKING ---- */
		else if (get_button() == KBD_DOWN){ //If joystick was moved down
			printf("\nMask mode\n");
			LPC_ADC->ADCR |=  ( 1 << 24);              // start conversion    
			printf("Addr: 0x%08X \n", LPC_ADC->ADCR);
			LPC_ADC->ADCR &= ~( 7 << 24);              // stop conversion     
			printf("Addr: 0x%08X \n", LPC_ADC->ADCR);
			/*
			LED_On(2); LED_On(3); //P1.31 & P2.2 turns on
			GLCD_DisplayString(6, 11, __FI, "MASKING"); // display the LED masking method
			GLCD_DisplayString(7, 11, __FI, "DOWN "); //display String direction to LCD
			LED_Off(2); LED_Off(3); // Turns off P1.31 & P2.2 */
		}
		/* ---- METHOD 3: BIT BANDING ---- */
		else if (get_button() == KBD_SELECT){ //If joystick was pressed 
			/*
			LED_On(4);LED_On(5); // P2.3 & P2.4 turn on 
			GLCD_DisplayString(7, 11, __FI, "PUSH "); //display String direction to LCD
			LED_Off(4);LED_Off(5); // P2.3 & P2.4 off */
		}				
  }
}
