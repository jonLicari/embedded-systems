/*----------------------------------------------------------------------------
* Name: KBD.c
* Purpose: MCB1700 low level Joystick
* Version: V2.0
*----------------------------------------------------------------------------
* This file is part of the uVision/ARM development tools.
* This software may only be used under the terms of a valid, current,
* end user licence from KEIL for a compatible version of KEIL software
* development tools. Nothing else gives you the right to use this software.
*
* This software is supplied "AS IS" without warranties of any kind.
*
* Copyright (c) 2008 Keil - An ARM Company. All rights reserved.
*----------------------------------------------------------------------------
* History:
* V2.0 - updated by Anita Tino for LPC1768
*----------------------------------------------------------------------------*/
#include <LPC17xx.H> /* LPC17xx definitions */
#include "KBD.h"
uint32_t KBD_val = 0;

/*----------------------------------------------------------------------------
initialize Joystick
*----------------------------------------------------------------------------*/

void KBD_Init (void) {
LPC_SC->PCONP |= (1 << 15); /* enable power to GPIO & IOCON */
/* P1.20, P1.23..26 is GPIO (Joystick) */
LPC_PINCON->PINSEL3 &= ~((3<< 8)|(3<<14)|(3<<16)|(3<<18)|(3<<20));
/* P1.20, P1.23..26 is input */
LPC_GPIO1->FIODIR &= ~((1<<20)|(1<<23)|(1<<24)|(1<<25)|(1<<26));
}

/*----------------------------------------------------------------------------
Get Joystick value.. part of get_button
*----------------------------------------------------------------------------*/
uint32_t KBD_get (void) {
uint32_t kbd_val;

kbd_val = (LPC_GPIO1->FIOPIN >> 20) & KBD_MASK;
return (kbd_val);
}

/*----------------------------------------------------------------------------
Get Joystick value
*----------------------------------------------------------------------------*/
uint32_t get_button (void) {
uint32_t val = 0;
val = KBD_get(); /* read Joystick state */
val = (~val & KBD_MASK); /* key pressed is read as a non '0' value*/
return (val);
}
