/*************************************************************
	COE718 Lab 3 Demo.c file
	Understanding different scheduling algorithms with RTX
	Anita Tino
*************************************************************/
#include <stdio.h>                                  
#include <ctype.h>                    
#include <string.h>                   
#include <stdbool.h>
#include "cmsis_os.h"
#include "RTL.H"		// RTX header file
#include "LPC17xx.H"              // LPC17xx definitions 
#include "GLCD.h"
#include "LED.h"

#define __USE_LCD 0		
#define __FI 1 				//Font index 16x24

//global task counters
unsigned int counta = 0;
unsigned int countb = 1;
unsigned int fib = 0;
char val[3]={"000"};

void delay(int ctr){
	int t;
	for(t = 0; t < ctr; t++)
	{
			t++;
	} 
}

__task void task1 (void const *arg) {  // __task is an RTX keyword
	
	for(;;) {
		fib = counta + countb;	
		LED_On(0);
		sprintf(val,"%d",fib);
		if (fib < 10) {
			GLCD_DisplayChar(6, 15, __FI, val[0]);
		}
		else if (fib<100 && fib>9){
			GLCD_DisplayChar(6, 15, __FI, val[0]);
			GLCD_DisplayChar(6, 16, __FI, val[1]);
		}			
		else if (fib>99) {
			GLCD_DisplayChar(6, 15, __FI, val[0]);	
			GLCD_DisplayChar(6, 16, __FI, val[1]);
			GLCD_DisplayChar(6, 17, __FI, val[2]);		
		}
		delay(10);
		LED_Off(0);
		
		if (fib == 377)
			os_tsk_delete_self(); 
	}
}

__task void task2 (void const *arg) {
	
	for(;;) {
		counta=countb;
		LED_On(1);
		delay(10);
		LED_Off(1);
		
		if (fib == 377)
			os_tsk_delete_self();
	}
}

__task void task3 (void const *arg) {
	//os_dly_wait(1000);
	for(;;) {
		countb=fib;
		LED_On(2);
		delay(10);
		LED_Off(2);
				
		if (fib == 377)
			os_tsk_delete_self();
	}
}

osThreadDef (task1, osPriorityNormal, 1, 0); 
osThreadDef (task2, osPriorityNormal, 1, 0);
osThreadDef (task3, osPriorityNormal, 1, 0);

int main (void) {
	
  LED_Init();                                /* LED Initialization            */
	
	#ifdef __USE_LCD
  GLCD_Init();                               /* Initialize graphical LCD (if enabled */
  GLCD_Clear(White);                         /* Clear graphical LCD display   */
  GLCD_SetBackColor(Blue);
  GLCD_SetTextColor(Yellow);
  GLCD_DisplayString(0, 0, __FI, "     COE718 Demo    ");
  GLCD_SetTextColor(White);
  GLCD_DisplayString(1, 0, __FI, "      lab3b  Q1     ");
  GLCD_DisplayString(2, 0, __FI, "   Round Robin Demo "); /* Change */
	GLCD_DisplayString(3, 0, __FI, "      j.licari      "); // author signature
	GLCD_SetBackColor(White);
  GLCD_SetTextColor(Blue);
	GLCD_DisplayString(6, 0, __FI, "Fibonacci Val:      ");
#endif
	
	SystemInit(); // initialize the Coretx-M3 processor 
	osKernelInitialize ();   // setup kernel
	osThreadCreate (osThread(task1), NULL);   // create threads
	//os_dly_wait(1000);
 	osThreadCreate (osThread(task2), NULL);
	//os_dly_wait(1000);	
	osThreadCreate (osThread(task3), NULL);
	//os_dly_wait(1000);
 	osKernelStart ();       // start kernel
	
	osDelay(osWaitForever);
}
