//conditional execution code example
#include "LPC17xx.h"

int main(void){
	int r1 = 1, r2 = 0;
	
	while(r1 <= 0x07){
		if((r1 - r2) > 0){
		  r1 = r1 + 2;
	}
		else{
			r2 = r2 + 1;
		}
	}
	return 0;
}


/* //barrel shifter code
#include "LPC17xx.h"

int main(void){
	int r1 = 1, r2 = 0, r3 = 5;
	
	while(r2 <= 0x18){
		if((r1 - r2) > 0){
		  r1 = r1 + 2;
			r2 = r1 + (r3*4);
			r3 = r3/2;
	}
		else{
			r2 = r2 + 1;
		}
	}
	return 0;
}*/
