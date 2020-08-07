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
//global task counters
unsigned int counta = 0;
unsigned int countb = 1;
unsigned int fib = 0;
//char val[3];

__task void task1 (void const *arg) {  // __task is an RTX keyword
	for(;;) {
		fib = counta + countb;	
		//sprintf(val,"%d",fib);
		if (fib == 144)
			os_tsk_delete_self(); 
	}
}

__task void task2 (void const *arg) {
	for(;;) {
		counta=countb;
		if (fib == 144)
			os_tsk_delete_self();
	}
}

__task void task3 (void const *arg) {
	for(;;) {
		countb=fib;  	
		if (fib == 144)
			os_tsk_delete_self();
	}
}

osThreadDef (task1, osPriorityNormal, 1, 0); 
osThreadDef (task2, osPriorityNormal, 1, 0);
osThreadDef (task3, osPriorityNormal, 1, 0);

int main (void) {
	SystemInit(); // initialize the Coretx-M3 processor 
	osKernelInitialize ();   // setup kernel
	osThreadCreate (osThread(task1), NULL);   // create threads
 	osThreadCreate (osThread(task2), NULL); 
	osThreadCreate (osThread(task3), NULL);
 	osKernelStart ();       // start kernel
	
	osDelay(osWaitForever);
}
