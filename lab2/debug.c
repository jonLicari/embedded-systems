#include "LPC17xx.H"
#include <stdio.h>
#include "LED.h"
#include "KBD.h"

//------- ITM Stimulus Port definitions for printf ------------------- //
#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))

#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000

#define __FI 1 				//Font index 16x24

//------------------------------------------------------------------- //

// Bit Band Macros used to calculate the alias address at run time
#define ADDRESS(x)    (*((volatile unsigned long *)(x)))
#define BitBand(x, y) 	ADDRESS(((unsigned long)(x) & 0xF0000000) | 0x02000000 |(((unsigned long)(x) & 0x000FFFFF) << 5) | ((y) << 2))
//			Bitband(SRAM Address, Bit #) 	

//Pointer declaration
volatile unsigned long * bit;
volatile unsigned long * bit2;

int i; // Bit Banding variable

#define ADCR_Bit24   (*((volatile unsigned long *)0x42680060))
// Define the LEDs to be used in the program
// Port 1
#define led28   (*((volatile unsigned long *)0x233806F0))
#define led29   (*((volatile unsigned long *)0x233806F4))
#define led31   (*((volatile unsigned long *)0x233806FC))
// Port 2	
#define led2   (*((volatile unsigned long *)0x23380A88))
#define led3   (*((volatile unsigned long *)0x23380A8C))
#define led6   (*((volatile unsigned long *)0x23380A98))	


int main(void){

  LED_Init();                                /* LED Initialization            */
	
/* --- ITE Conditional --- */ 
/* The conditional method was implemented using the joystick KBD get_button() states */                   
			
 		/* ---- METHOD 1: FUNCTION ---- */
		//if (get_button() == KBD_UP){ //If joystick was moved upwards
			// Returns address of the storage of the Bitband function where &Bitband(Returns value of Port 1 LED address, Bit #) 
			bit = &BitBand(&LPC_GPIO1->FIOPIN, 28);	
			// Returns address of the storage of the Bitband function where &Bitband(Returns value of Port 2 LED address, Bit #) 
			bit2 = &BitBand(&LPC_GPIO2->FIOPIN, 6);	
			
			//Set the chosen LEDs P1.28 and P2.6 ON by setting the bit of their address in the Status Register to 1, as the reset value is 0
			*bit = 1;
			*bit2 = 1;
						
			// Turn off the LEDs P1.28 and P2.6 by setting their bits in the Status Register to 0, as they are currently on
			*bit = 0;
			*bit2 = 0;
	//	}
		/* ---- METHOD 2: MASKING ---- */
	//	else if (get_button() == KBD_DOWN){ //If joystick was moved down
						
				LPC_SC->PCONP     |= (1 << 15);            /* enable power to GPIO & IOCON  */

				LPC_GPIO1->FIODIR |= 0xB0000000;           /* LEDs on PORT1 are output      */
				LPC_GPIO2->FIODIR |= 0x0000007C;           /* LEDs on PORT2 are output      */
				// Turn on LEDs
				LPC_GPIO1->FIOPIN |=  (1<<28);
				LPC_GPIO2->FIOPIN |=  (1<<6);
				
				//Turn off the LEDs
				LPC_GPIO1->FIOPIN &=  ~(1<<28);
				LPC_GPIO2->FIOPIN &=  ~(1<<6);   
	//	}
		/* ---- METHOD 3: BIT BANDING ---- */
	//	else if (get_button() == KBD_SELECT){ //If joystick was pressed 
				led6 = 1;
				led6 = 0;
			}
			
  

