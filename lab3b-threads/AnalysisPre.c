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

double a=0, b=1, c=0, d=0, e=0; 	// Result storage
double ac=0,bc=1,cc=1,dc=0,ec=1; // Task counters
int bf=1, df=1; // Task factorial storage
int radius = 2;
//int x=0;

//char rad[3]; 									// For printing stored values

__task void taskA (void const *arg) {  // Middle priority -
	for (;;) { 
		a = (a + (ac + (ac + 2))); 
		ac++; //increment counter
		
		if (ac > 256) //Stop condition
			os_tsk_delete_self();
	}
}

__task void taskB (void const *arg) { // Lowest Priority -
	for (;;) {
		bf *= bc; // Factorial calculation
		b = (b + (pow(2,bc)/ bf));
	  bc++;
		
		if (bc>16) //Stop condition
			os_tsk_delete_self();
	} 	
}
__task void taskC (void const *arg) { // Highest Priority -
	for (;;){
		c += ((cc + 1)/(cc));
		cc++;
		
		if (cc > 16) //Stop condition
				os_tsk_delete_self();
	}
	
}
__task void taskD (void const *arg) { // Middle priority -
	for (;;) {
		if (dc == 0) {
			d = 1;
		}
		else {
			df*=dc;
			d += (pow(5, dc)/df);
		}
		dc++;
		
		if (dc>6)
			os_tsk_delete_self();
	}
}
__task void taskE (void const *arg) { // Lowest Priority -
	for (;;) {
		e += ec*3.14*pow(radius,2);
		ec++;

		if (ec > 12) //Stop condition
			os_tsk_delete_self();
	}
}
//create a thread for the function task1 and task2 with normal priority
osThreadDef (taskA, osPriorityNormal, 1, 0); 
osThreadDef (taskB, osPriorityBelowNormal, 1, 0);
osThreadDef (taskC, osPriorityAboveNormal, 1, 0);
osThreadDef (taskD, osPriorityNormal, 1, 0);
osThreadDef (taskE, osPriorityBelowNormal, 1, 0);

int main (void) {
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
		/* Test code 
		for (x =0;x<1000;x++)
			radius++;	*/
