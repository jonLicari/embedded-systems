#include <stdio.h>
#include "LPC17xx.h"
#include <RTL.h>
#include "LED.h"
#include "GLCD.h"

#define __USE_LCD 0		
#define __FI 1 				//Font index 16x24
	
long g_c1 = 0, g_c2 = 0, g_c3 =0;
int i;

void delay(int ctr){
	int t;
	for(t = 0; t < ctr; t++)
	{
			t++;
	} 
}	
/* ----- TASK 1: INCREMENT ----- */
__task void task1(void){ //Finite run-time
	GLCD_DisplayString(6, 0, __FI, "Task 1: Increment  ");
	//os_dly_wait(100);
	for (i = 0; i<5; i++){ 
		LED_On(i);
		os_dly_wait(10);
	}
		//delay(10000000);
	os_dly_wait(5000);
}
/* ----- TASK 2: DECREMENT ----- */
__task void task2(void){ //Finite run-time
	GLCD_DisplayString(6, 0, __FI, "Task 2: Decrement  ");
	//os_dly_wait(100);
	for (i = 0; i<5; i++){
		LED_Off(4-i);
		os_dly_wait(10);
	}
	//delay(10000000);
	os_dly_wait(5000);	
}
/* ----- TASK 3: FLASH ----- */
__task void task3(void){ //Finite run-time
	GLCD_DisplayString(6, 0, __FI, "Task 3: Flash      ");
		for (i = 0; i<5; i++){
			LED_On(7);
			os_dly_wait(10);
			//delay(100000);
			LED_Off(7);
			os_dly_wait(10);
		}
		//delay(10000000);
		os_dly_wait(5000);
}

int main(void){
	LED_Init();

#ifdef __USE_LCD
  GLCD_Init();                               /* Initialize graphical LCD (if enabled */
  GLCD_Clear(White);                         /* Clear graphical LCD display   */
  GLCD_SetBackColor(Blue);
  GLCD_SetTextColor(Yellow);
  GLCD_DisplayString(0, 0, __FI, "     COE718 Demo    ");
  GLCD_SetTextColor(White);
  GLCD_DisplayString(1, 0, __FI, "      lab3a  Q1     ");
  GLCD_DisplayString(2, 0, __FI, "   Round Robin Demo "); /* Change */
	GLCD_DisplayString(3, 0, __FI, "      j.licari      "); // author signature
  GLCD_SetBackColor(White);
  GLCD_SetTextColor(Blue);
#endif
	SystemInit(); //used by the internal NXP microcontroller to setup the system and configure the clock.
	//The os_tsk_create() functions will create the tasks and set priority levels (low 1 - hi 255)
	/*Using the RTX timer, task1 and task2 will infinitely loop using round-robin arbitration with 10ms time
	slices, set according to the specifications in RTX_Conf_CM.c. */
	os_tsk_create(task1, 1);
	os_dly_wait(100);
	os_tsk_create(task2, 1);
	os_dly_wait(100);
	os_tsk_create(task3, 1);
	os_dly_wait(100);
	os_tsk_delete_self(); //deletes the main() so only the three tasks execute

	os_sys_init(task1); // Starts executing task1 
}
