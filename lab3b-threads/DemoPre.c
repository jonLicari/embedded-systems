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
#include <math.h>
#include "GLCD.h"
#include "LED.h"

#define __USE_LCD 0		
#define __FI 1 				//Font index 16x24

double a=0, b=1, c=0, d=0, e=0; 	// Result storage
double ac=0,bc=1,cc=1,dc=0,ec=1; // Task counters
int bf=1, df=1; // Task factorial storage
int radius = 2;
char task = '0', val[8];

void delay(int ctr){
	int t;
	for(t = 0; t < ctr; t++)
	{
			t++;
	} 
}

__task void taskA (void const *arg) {  // Middle priority -
	for (;;) { 
		task='A';
		LED_On(0);
		GLCD_DisplayChar(4, 16, __FI, task);
		delay(10000);
		
		a = (a + (ac + (ac + 2))); 
		ac++; //increment counter
		
		if (ac > 256) {//Stop condition
			
			sprintf(val,"%.2lf",a);
			GLCD_DisplayString(5, 0, __FI, "   A:               ");
			GLCD_DisplayString(5, 6, __FI, (unsigned char*)val);
			LED_Off(0);
			os_tsk_delete_self();
		}
	}
}

__task void taskB (void const *arg) { // Lowest Priority -
	for (;;) {
		task='B';
		LED_On(1);
		GLCD_DisplayChar(4, 16, __FI, task);
		delay(10000);
		
		bf *= bc; // Factorial calculation
		b = (b + (pow(2,bc)/ bf));
	  bc++;
		
		if (bc>16) {//Stop condition
			sprintf(val,"%lf",b);
			GLCD_DisplayString(6, 0, __FI, "   B:               ");
			GLCD_DisplayString(6, 6, __FI, (unsigned char*)val);
			LED_Off(1);
			os_tsk_delete_self();
		}
	} 	
}
__task void taskC (void const *arg) { // Highest Priority -
	for (;;){
		task='C';
		LED_On(2);
		GLCD_DisplayChar(4, 16, __FI, task);
		delay(10000);
		
		c += ((cc + 1)/(cc));
		cc++;
		
		if (cc > 16) {//Stop condition
			sprintf(val,"%lf",c);
			GLCD_DisplayString(7, 0, __FI, "   C:               ");
			GLCD_DisplayString(7, 6, __FI, (unsigned char*)val);
			LED_Off(2);	
			os_tsk_delete_self();
		}
	}
}
__task void taskD (void const *arg) { // Middle priority -
	for (;;) {
		task='D';
		LED_On(3);
		GLCD_DisplayChar(4, 16, __FI, task);
		delay(10000);
		
		if (dc == 0) {
			d = 1;
		}
		else {
			df*=dc;
			d += (pow(5, dc)/df);
		}
		dc++;
		
		if (dc>6){
			sprintf(val,"%lf",d);
			GLCD_DisplayString(8, 0, __FI, "   D:               ");
			GLCD_DisplayString(8, 6, __FI, (unsigned char*)val);
			LED_Off(3);
			os_tsk_delete_self();
		}
	}
}
__task void taskE (void const *arg) { // Lowest Priority -
	for (;;) {
		task='E';
		LED_On(4);
		GLCD_DisplayChar(4, 16, __FI, task);
		delay(10000);
		
		e += ec*3.14*pow(radius,2);
		ec++;

		if (ec > 12) {//Stop condition
			sprintf(val,"%.2lf",e);
			GLCD_DisplayString(9, 0, __FI, "   E:               ");
			GLCD_DisplayString(9, 6, __FI, (unsigned char*)val);
			LED_Off(4);
			os_tsk_delete_self();
		}
	}
}
//create a thread for the function task1 and task2 with normal priority
osThreadDef (taskA, osPriorityNormal, 1, 0); 
osThreadDef (taskB, osPriorityBelowNormal, 1, 0);
osThreadDef (taskC, osPriorityAboveNormal, 1, 0);
osThreadDef (taskD, osPriorityNormal, 1, 0);
osThreadDef (taskE, osPriorityBelowNormal, 1, 0);

int main (void) {
	LED_Init();
#ifdef __USE_LCD
  GLCD_Init();                               /* Initialize graphical LCD (if enabled */
  GLCD_Clear(White);                         /* Clear graphical LCD display   */
  GLCD_SetBackColor(Blue);
  GLCD_SetTextColor(Yellow);
  GLCD_DisplayString(0, 0, __FI, "     COE718 Demo    ");
  GLCD_SetTextColor(White);
  GLCD_DisplayString(1, 0, __FI, "      lab3b  Q2     ");
  GLCD_DisplayString(2, 0, __FI, "   Pre-emptive Demo "); /* Change */
	GLCD_DisplayString(3, 0, __FI, "      j.licari      "); // author signature
	GLCD_SetBackColor(White);
  GLCD_SetTextColor(Blue);
	GLCD_DisplayString(4, 0, __FI, "Current Thread:     ");
#endif
	SystemInit(); // initialize the Coretx-M3 processor 
	osKernelInitialize ();   // setup kernel
	osThreadCreate (osThread(taskA), NULL);   // create threads
 	osThreadCreate (osThread(taskB), NULL);
	osThreadCreate (osThread(taskC), NULL);
	osThreadCreate (osThread(taskD), NULL);
	osThreadCreate (osThread(taskE), NULL);	
 	osKernelStart ();       // start kernel

	osDelay(osWaitForever);
}
