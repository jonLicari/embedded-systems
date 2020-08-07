#include <stdio.h>
#include "LPC17xx.h"
#include <RTL.h>
#include <string.h>
#include <math.h>
#include "GLCD.h"
#include "LED.h"

#define __USE_LCD 0		
#define __FI 1 				//Font index 16x24

// Bit Band Macros used to calculate the alias address at run time
#define ADDRESS(x)    (*((volatile unsigned long *)(x)))
#define BitBand(x, y) 	ADDRESS(((unsigned long)(x) & 0xF0000000) | 0x02000000 |(((unsigned long)(x) & 0x000FFFFF) << 5) | ((y) << 2))
//			Bitband(SRAM Address, Bit #)
//Pointer declaration
volatile unsigned long * bit;
int a=0, b=2, c=0; // Barrel Shifting variables

OS_TID id1, id2, id3, id4; // "Task Identifier" values to keep track of each task's ID
double  mem=0,dev=0, app=0, cpu=0, usr=0;
char var_logger[15];

__task void mem_mgmt (void);
__task void dev_mgmt (void);
__task void cpu_mgmt (void);
__task void usr_int (void);
__task void app_ui (void);

//MEMORY MANAGEMENT - Lowest Priority (1)
__task void mem_mgmt (void) {
	GLCD_DisplayString(6, 0, __FI, "Task: Memory Mgmt  ");
	LED_On(0); //Lowest priority will bring up lowest LED
	os_dly_wait(500); 			//delay os for 1 tick
	
	id1 = os_tsk_self(); //identify myself and create cpu_mgmt
	id2 = os_tsk_create(cpu_mgmt, 0x03); //create and return
	mem++; //increment the memory access counter
	
	// Returns address of the storage of the Bitband function where &Bitband(Returns value of Port 1 LED address, Bit #) 
	bit = &BitBand(&LPC_GPIO1->FIOPIN, 28);
	os_dly_wait(500);
	os_tsk_pass(); // Pass to the next task of equal or higher priority
	
	for(;;){
		if(os_evt_wait_and(0x0004, 0xFFFF)){ 
			GLCD_DisplayString(6, 0, __FI, "Task: Memory Mgmt  ");
			LED_On(0);
			os_dly_wait(1000); 			//delay os for 1 tick
			
			GLCD_DisplayString(6, 0, __FI, "Task: Idle Demon   ");
			os_dly_wait(50);
	
			LED_Off(0); //Lowest priority will bring up lowest LED
			LED_On(7);
			os_dly_wait(50);
			
			os_tsk_delete_self(); //delete itself
		}
	}
}//should go to idle_deamon after this

//CPU MANAGEMENT - Lowest Priority (1)
__task void cpu_mgmt (void) {
	GLCD_DisplayString(6, 0, __FI, "Task: CPU Mgmt     ");
	LED_On(0); //Lowest priority will bring up lowest LED
	os_dly_wait(500);
	
	id2 = os_tsk_self();
	cpu++;
	while(c <= 5){
		a = pow(b,c);
		c++;
	}
	c=0;
	
	os_dly_wait(500);
	LED_Off(0);
	os_evt_set(0x0004, id1);
	
	os_tsk_delete_self(); 
}

//APPLICATION INTERFACE - Priority (2)
__task void app_ui (void) { // make sure this executes before the device management //priority 3
	GLCD_DisplayString(6, 0, __FI, "Task: App. Int.    ");
	LED_On(1); // Second lowest priority for 2nd lowest LED
	os_dly_wait(500);
	
	id3 = os_tsk_self(); //obtain my identity
	id4 = os_tsk_create(dev_mgmt,0x02);
	strcpy(var_logger,"part 1"); //write a partial message to logger using a mutex
	os_tsk_pass(); 							 //switch to device management
	
	for(;;){
		if(os_evt_wait_and(0x0005, 0xFFFF)){ 
			GLCD_DisplayString(6, 0, __FI, "Task: App. Int.    ");
			LED_On(1);
			app++;
			os_dly_wait(500); 				//delay os for 1 tick
			LED_Off(1);
			os_dly_wait(50);
			os_tsk_delete_self(); 	//delete itself
		}
	}
}

//DEVICE MANAGEMENT - Priority (2)
__task void dev_mgmt (void){//make sure this runs after the app ui
	GLCD_DisplayString(6, 0, __FI, "Task: Device Mgmt  ");
	LED_On(1);
	os_dly_wait(500);
	
	id4 = os_tsk_self();
	
	var_logger[7] = 'p';		//finishes writing to the logger
	var_logger[8] = 'a';
	var_logger[9] = 'r';
	var_logger[10] = 't';
	var_logger[11] = ' ';
	var_logger[12] = '2';
	//os_tsk_pass();					//pass back to app_ui
	os_evt_set(0x0005, id3);

	dev++;									//increment global variable
	os_dly_wait(50);					//delays os for 1 tick to ensure the file is closed
	os_dly_wait(50);
	os_tsk_delete_self(); 	//deletes itself

}

//USER INTERFACE - Highest Priority (4)
__task void usr_int (void){
	GLCD_DisplayString(6, 0, __FI, "Task: User Int     ");
	LED_On(3);
	os_dly_wait(1000);
	
	usr++;
	os_dly_wait(50);//delay os for 1 tick from RTL.h we want os delay not a real time delay
	LED_Off(3);
	os_tsk_delete_self(); //delete the function
}

int main (void) {
	
  LED_Init();                                /* LED Initialization            */
	
	#ifdef __USE_LCD
  GLCD_Init();                               /* Initialize graphical LCD (if enabled */
  GLCD_Clear(White);                         /* Clear graphical LCD display   */
  GLCD_SetBackColor(Blue);
  GLCD_SetTextColor(Yellow);
  GLCD_DisplayString(0, 0, __FI, "     COE718 Demo    ");
  GLCD_SetTextColor(White);
  GLCD_DisplayString(1, 0, __FI, "      lab3a  Q2     ");
  GLCD_DisplayString(2, 0, __FI, "   Pre-emptive Demo "); /* Change */
	GLCD_DisplayString(3, 0, __FI, "      j.licari      "); // author signature
  GLCD_SetBackColor(White);
  GLCD_SetTextColor(Blue);
#endif
	SystemInit();
	os_tsk_create(usr_int, 0x03);
	os_dly_wait(1000);
	os_tsk_create(app_ui, 0x02);
	os_dly_wait(3000);
	os_tsk_create(mem_mgmt, 0x01); //create job1 and initialize system
		
	os_tsk_delete_self();
}
