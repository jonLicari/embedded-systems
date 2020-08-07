#include <stdio.h>
#include "LPC17xx.h"
#include <RTL.h>
#include <string.h>
#include <math.h>

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
	id1 = os_tsk_self(); //identify myself and create cpu_mgmt
	id2 = os_tsk_create(cpu_mgmt, 0x03); //create and return
	mem++; //increment the memory access counter
	
	// Returns address of the storage of the Bitband function where &Bitband(Returns value of Port 1 LED address, Bit #) 
	bit = &BitBand(&LPC_GPIO1->FIOPIN, 28);
	os_tsk_pass(); // Pass to the next task of equal or higher priority
	
	for(;;){
		if(os_evt_wait_and(0x0004, 0xFFFF)){ 
			os_dly_wait(1); 			//delay os for 1 tick
			os_tsk_delete_self(); //delete itself
		}
	}
}//should go to idle_deamon after this

//CPU MANAGEMENT - Lowest Priority (1)
__task void cpu_mgmt (void) {
	id2 = os_tsk_self();
	cpu++;
	while(c <= 5){
		a = pow(b,c);
		c++;
	}
	c=0;
	os_evt_set(0x0004, id1);
	os_tsk_delete_self(); 
}

//APPLICATION INTERFACE - Priority (2)
__task void app_ui (void) { // make sure this executes before the device management //priority 3
	id3 = os_tsk_self(); //obtain my identity
	id4 = os_tsk_create(dev_mgmt,0x02);
	strcpy(var_logger,"part 1"); //write a partial message to logger using a mutex
	os_tsk_pass(); 							 //switch to device management
	
	for(;;){
		if(os_evt_wait_and(0x0005, 0xFFFF)){ 
			app++;
			os_dly_wait(1); 				//delay os for 1 tick
			os_tsk_delete_self(); 	//delete itself
		}
	}
}

//DEVICE MANAGEMENT - Priority (2)
__task void dev_mgmt (void){//make sure this runs after the app ui
	id4 = os_tsk_self();
	var_logger[7] = 'p';		//finishes writing to the logger
	var_logger[8] = 'a';
	var_logger[9] = 'r';
	var_logger[10] = 't';
	var_logger[11] = ' ';
	var_logger[12] = '2';
	os_tsk_pass();					//pass back to app_ui
	dev++;									//increment global variable
	os_dly_wait(1);					//delays os for 1 tick to ensure the file is closed
	os_tsk_delete_self(); 	//deletes itself

}

//USER INTERFACE - Highest Priority (4)
__task void usr_int (void){
	usr++;
	os_dly_wait(1);//delay os for 1 tick from RTL.h we want os delay not a real time delay
	os_tsk_delete_self(); //delete the function
}

int main (void) {
	SystemInit();
	os_tsk_create(mem_mgmt, 0x01); //create job1 and initialize system
	os_tsk_create(app_ui, 0x02);
	os_tsk_create(usr_int, 0x03);
	os_tsk_delete_self();

	os_tsk_delete_self();
}
