#include <stdio.h>
#include "LPC17xx.h"
#include <RTL.h>

long g_c1 = 0;
int ctr;
/* 
Task 1 increments the global variable
Task 2 decrements it proportionally to task 1
Task 3 increments the global value to a reference value, here 0xFFFF
*/
__task void task1(void){
	for(ctr = 0; ctr < 8; ctr++){
		g_c1 += 1;
	}
}

__task void task2(void){
	for(ctr = 0; ctr < 8; ctr++ ){
		g_c1 -= 1;
	}
}

__task void task3(void){
	for(ctr=0; ctr < 5; ctr++){
		g_c1 += 0x3333;
	}
}

int main(void){
	SystemInit(); //used by the internal NXP microcontroller to setup the system and configure the clock.
	//The os_tsk_create() functions will create the tasks and set priority levels (low 1 - hi 255)
	/*Using the RTX timer, task1 and task2 will infinitely loop using round-robin arbitration with 10ms time
	slices, set according to the specifications in RTX_Conf_CM.c. */
	os_tsk_create(task1, 1);
	os_tsk_create(task2, 1);
	os_tsk_create(task3, 1);
	os_tsk_delete_self(); //deletes the main() so only the three tasks execute

	os_sys_init(task1); // Starts executing task1 
}
