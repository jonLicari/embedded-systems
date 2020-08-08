/*#############################################################################/
	Using threads, signal, and wait for thread scheduling and synchronization
	CMSIS RTX Example
	Anita Tino
*#############################################################################*/
#include "LPC17xx.h"
#include "cmsis_os.h"
#include "RTL.H"
#include "LED.h"
#include "GLCD.h"

#define __USE_LCD 0		// uncomment for LCD use
#define __FI 1 				//Font index 16x24

//int a=0,b=0,c=0;

void led_Thread1 (void const *argument);
void led_Thread2 (void const *argument);
void led_Thread3 (void const *argument);

osThreadDef(led_Thread1, osPriorityBelowNormal, 1, 0);	//A
osThreadDef(led_Thread2, osPriorityNormal, 1, 0);				//B
osThreadDef(led_Thread3, osPriorityAboveNormal, 1, 0);	//C

/*###########################################################
	Virtual Timer declaration and call back method
############################################################*/
osThreadId T_led_ID1;
osThreadId T_led_ID2;	
osThreadId T_led_ID3;	

// Custom Delay
void delay(int ctr){
	int t;
	for(t = 0; t < ctr; t++)
	{
			t++;
	} 
}

// Toggle the LED associated with the timer
void callback(void const *param){
	switch( (uint32_t) param){
		case 0:										// ------------------- A
			LED_On(5); LED_Off(6); LED_Off(7);
			osSignalSet	(T_led_ID1,0x01);
			//a++;
			osDelay(8000);
		  LED_Off(5); //LED_Off(6);
			break;
		case 1:										// ------------------- B
			LED_Off(5); LED_On(6); LED_Off(7);
			osSignalSet	(T_led_ID2,0x02);
		//	b++;
		  osDelay(8000);
		  LED_Off(6); //LED_Off(7);
		  break;
		case 2:										// ------------------- C
			LED_Off(5); LED_Off(6); LED_On(7);
			osSignalSet	(T_led_ID3,0x03);
			//c++;
			osDelay(8000);
			LED_Off(7);
			break;
	}
}
// Defining the Virtual Timers
osTimerDef(timer0_handle, callback);
osTimerDef(timer1_handle, callback);
osTimerDef(timer2_handle, callback);
//#############################################################

/*#############################################################
  Flash LED 0, signal to thread 2, wait for 3 to finish
*#############################################################*/
void led_Thread1 (void const *argument) { //PROCESS A
	for (;;) {
		osSignalWait (0x01,osWaitForever);
		LED_On(0);
		delay(200000);
		osDelay(1000);
		//osDelay(20000);
		LED_Off(0); 
		
	}
}

/*################################################################
  Flash LED 2, signal to thread 3, wait for thread 1 to finish
*################################################################*/
void led_Thread2 (void const *argument) { //PROCESS B
	for (;;) 	{
		osSignalWait (0x02,osWaitForever);
		LED_Off(1);	
		LED_On(1);
		delay(100000);
		osDelay(1000);
    //osDelay(10000);		
		LED_Off(1);
	}
}

/*################################################################
  Flash LED 4, signal to thread 1, wait for thread 2 to finish
*################################################################*/
void led_Thread3 (void const *argument){ //PROCESS C
	for (;;) 	{
		osSignalWait (0x03,osWaitForever);
		LED_Off(2);
		LED_On(2);
		delay(50000);	// 5000 ~= 1.07ms	
		//osDelay(5000);
		osDelay(1000);
		LED_Off(2);
	}
}

/*###################################################################
  Create and start threads
 *###################################################################*/
int main (void) {
	//Virtual timer create and start
	osTimerId timer_0 = osTimerCreate(osTimer(timer0_handle), osTimerPeriodic, (void *)0);	// Creates timer_0 and specifies info for its callback
	osTimerId timer_1 = osTimerCreate(osTimer(timer1_handle), osTimerPeriodic, (void *)1);	
	osTimerId timer_2 = osTimerCreate(osTimer(timer2_handle), osTimerPeriodic, (void *)2);
		
 #ifdef __USE_LCD
  GLCD_Init();                               // Initialize graphical LCD (if enabled 
  GLCD_Clear(White);                         // Clear graphical LCD display   
  GLCD_SetBackColor(Blue);
  GLCD_SetTextColor(Yellow);
  GLCD_DisplayString(0, 0, __FI, "     COE718 Demo    ");
  GLCD_SetTextColor(White);
  GLCD_DisplayString(1, 0, __FI, "      lab 4  Q1     ");
  GLCD_DisplayString(2, 0, __FI, " RMS-Fixed Priority "); // Change 
	GLCD_DisplayString(3, 0, __FI, "      j.licari      "); // author signature
#endif 
	
  LED_Init ();	
	// the integer value is used to represent the computation time (C)
	osTimerStart(timer_0, 40000);	//6000ms countdown period ??
	osTimerStart(timer_1, 40000);	//B
	osTimerStart(timer_2, 20000);	//C	
	
	//Signal and wait threads
	T_led_ID1 = osThreadCreate(osThread(led_Thread1), NULL);
	T_led_ID2 = osThreadCreate(osThread(led_Thread2), NULL);
  T_led_ID3 = osThreadCreate(osThread(led_Thread3), NULL);
	osDelay(osWaitForever);               						
	
	for (;;);
}

