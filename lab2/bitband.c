#include "LPC17xx.H"
#include <stdio.h>
#include <math.h> // Necessary for Barrel shifting computation
#include "LED.h"
#include "GLCD.h"
#include "KBD.h"

//------- ITM Stimulus Port definitions for printf ------------------- //
#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))

#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000

#define __USE_LCD 0		
#define __FI 1 				//Font index 16x24
/*
struct __FILE { int handle;  };
FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f) {
  if (DEMCR & TRCENA) {
    while (ITM_Port32(0) == 0);
    ITM_Port8(0) = ch;
  }
  return(ch);
}*/
//------------------------------------------------------------------- //

// Bit Band Macros used to calculate the alias address at run time
#define ADDRESS(x)    (*((volatile unsigned long *)(x)))
#define BitBand(x, y) 	ADDRESS(((unsigned long)(x) & 0xF0000000) | 0x02000000 |(((unsigned long)(x) & 0x000FFFFF) << 5) | ((y) << 2))
//			Bitband(SRAM Address, Bit #) 	

//Pointer declaration
volatile unsigned long * bit;
volatile unsigned long * bit2;

int i; // Bit Banding variable
int a = 0, b = 2, c = 0; // Barrel Shifting variables

#define ADCR_Bit24   (*((volatile unsigned long *)0x42680060))
// Define the LEDs to be used in the program
/*#define led28 	(*((volatile unsigned long *)0x425801F0))
#define led2		(*((volatile unsigned long *)0x42000008))
#define led3		(*((volatile unsigned long *)0x4200000C))
#define led4		(*((volatile unsigned long *)0x42000010))
#define led5		(*((volatile unsigned long *)0x42000019))*/
// Port 1
#define led28   (*((volatile unsigned long *)0x233806F0))
#define led29   (*((volatile unsigned long *)0x233806F4))
#define led31   (*((volatile unsigned long *)0x233806FC))
// Port 2	
#define led2   (*((volatile unsigned long *)0x23380A88))
#define led3   (*((volatile unsigned long *)0x23380A8C))
#define led6   (*((volatile unsigned long *)0x23380A98))	

void delay(int ctr){
	int t;
	for(t = 0; t < ctr; t++)
	{
			t++;
	} 
}	
int main(void){

  LED_Init();                                /* LED Initialization            */
	KBD_Init();																 /* Joystick Initialization  			*/
	
#ifdef __USE_LCD
  GLCD_Init();                               /* Initialize graphical LCD (if enabled */

  GLCD_Clear(White);                         /* Clear graphical LCD display   */
  GLCD_SetBackColor(Blue);
  GLCD_SetTextColor(Yellow);
  GLCD_DisplayString(0, 0, __FI, "     COE718 Demo    ");
  GLCD_SetTextColor(White);
  GLCD_DisplayString(1, 0, __FI, "       lab2.c       ");
  GLCD_DisplayString(2, 0, __FI, "Select Down Up Right"); /* Change */
	GLCD_DisplayString(3, 0, __FI, "      j.licari      "); // author signature
  GLCD_SetBackColor(White);
  GLCD_SetTextColor(Blue);
  GLCD_DisplayString(6, 0, __FI, "Method:            ");
	GLCD_DisplayString(7, 0, __FI, "Direction:         ");
#endif
/* --- ITE Conditional --- */ 
/* The conditional method was implemented using the joystick KBD get_button() states */
  while (1) {                               
			
 		/* ---- METHOD 1: FUNCTION ---- */
		if (get_button() == KBD_UP){ //If joystick was moved upwards
			// Display the method being used 
			GLCD_DisplayString(6, 9, __FI, "  FUNCTION"); // display the LED function method
			GLCD_DisplayString(7, 11, __FI, "UP     "); // display String direction to LCD
			
			// Returns address of the storage of the Bitband function where &Bitband(Returns value of Port 1 LED address, Bit #) 
			bit = &BitBand(&LPC_GPIO1->FIOPIN, 28);	
			// Returns address of the storage of the Bitband function where &Bitband(Returns value of Port 2 LED address, Bit #) 
			bit2 = &BitBand(&LPC_GPIO2->FIOPIN, 6);	
			
			//Set the chosen LEDs P1.28 and P2.6 ON by setting the bit of their address in the Status Register to 1, as the reset value is 0
			*bit = 1;
			*bit2 = 1;
			// Leave the LEDs on for some time
			delay(10000000);
			// Turn off the LEDs P1.28 and P2.6 by setting their bits in the Status Register to 0, as they are currently on
			*bit = 0;
			*bit2 = 0;
		}
		/* ---- METHOD 2: MASKING ---- */
		else if (get_button() == KBD_DOWN){ //If joystick was moved down
			GLCD_DisplayString(6, 9, __FI, "  MASKING  "); // display the LED function method
			GLCD_DisplayString(7, 11, __FI, "DOWN     "); // display String direction to LCD
			
				LPC_SC->PCONP     |= (1 << 15);            /* enable power to GPIO & IOCON  */

				LPC_GPIO1->FIODIR |= 0xB0000000;           /* LEDs on PORT1 are output      */
				LPC_GPIO2->FIODIR |= 0x0000007C;           /* LEDs on PORT2 are output      */
				// Turn on LEDs
				LPC_GPIO1->FIOPIN |=  (1<<28);
				LPC_GPIO2->FIOPIN |=  (1<<6);
				// Leave LEDs on for a bit
				delay(1000000);
				//Turn off the LEDs
				LPC_GPIO1->FIOPIN &=  ~(1<<28);
				LPC_GPIO2->FIOPIN &=  ~(1<<6);   
		}
		/* ---- METHOD 3: BIT BANDING ---- */
		else if (get_button() == KBD_SELECT){ //If joystick was pressed 
			GLCD_DisplayString(6, 9, __FI, "BIT BANDIN'"); // display the LED function method
			GLCD_DisplayString(7, 11, __FI, "PUSH     "); // display String direction to LCD
			for (i = 0; i < 5; i++) {
				led6 = 1;
				delay(1000000);	// Medium blink speed
				led6 = 0;
			}
		}
		/* ---- METHOD 4: BARREL SHIFTER ---- */
		else if (get_button() == KBD_RIGHT){ //If joystick was pressed 
			GLCD_DisplayString(6, 9, __FI, "B. Shifter "); // display the LED function method
			GLCD_DisplayString(7, 11, __FI, "RIGHT    "); // display String direction to LCD
			// Recall: int a = 0, b = 2, c = 0;
			while(c <= 7){
				a = pow(b,c);
				if(a == 1){
					LED_On(0);
					delay(10000000);
					LED_Off(0);
				}
				else if(a == 2){
					LED_On(1);
					delay(10000000);
					LED_Off(1);
				}
				else if(a == 4){
					LED_On(2);
					delay(10000000);
					LED_Off(2);
				}
				else if(a == 8){
					LED_On(3);
					delay(10000000);
					LED_Off(3);
				}
				else if(a == 16){
					LED_On(4);
					delay(10000000);
					LED_Off(4);
				}
				else if(a == 32){
					LED_On(5);
					delay(10000000);
					LED_Off(5);
				}
				else if(a == 64){
					LED_On(6);
					delay(10000000);
					LED_Off(6);
				}
				else if(a == 128){
					LED_On(7);
					delay(10000000);
					LED_Off(7);
				}
				c++; // increment the exponent factor
			}
			c=0; // once the while loop breaks, reset c to allow for another selection
		}				
  }
}
