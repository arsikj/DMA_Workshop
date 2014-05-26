/*
 * led.c
 *
 *  Created on: 25.5.2014
 *      Author: Bojan
 */

#include "led.h"
#include "LPC17xx.h"

void led_init(void) {
	//Set P0.26 to 00 - GPIO
	LPC_PINCON->PINSEL1 &= (~(3 << 20));
	//Set P2.0 to 00 -GPIO
	LPC_PINCON->PINSEL4 &= (~3);
	//Set GPIO - P0.26 - to be output
	LPC_GPIO0->FIODIR |= (1 << 26);
	//Set GPIO - P2.0 to be output
	LPC_GPIO2->FIODIR |= 1;
}
void led_green_on(void) {
	LPC_GPIO0->FIOSET = (1 << 26);
}
void led_yellow_on(void) {
	LPC_GPIO2->FIOSET = 1;
}
void led_green_off(void) {
	LPC_GPIO0->FIOCLR = (1 << 26);
}

void led_yellow_off(void) {
	LPC_GPIO2->FIOCLR = 1;
}
void led_green_invert(void) {
	int ledstate;

	// Read current state of GPIO P0_0..31, which includes LED2
	ledstate = LPC_GPIO0->FIOPIN;
	// Turn off LED if it is on
	// (ANDing to ensure we only affect the LED output)
	LPC_GPIO0->FIOCLR = ledstate & (1 << 26);
	// Turn on LED if it is off
	// (ANDing to ensure we only affect the LED output)
	LPC_GPIO0->FIOSET = ((~ledstate) & (1 << 26));
}
void led_yellow_invert(void) {
	int ledstate;

	// Read current state of GPIO P0_0..31, which includes LED2
	ledstate = LPC_GPIO2->FIOPIN;
	// Turn off LED if it is on
	// (ANDing to ensure we only affect the LED output)
	LPC_GPIO2->FIOCLR = ledstate & (1);
	// Turn on LED if it is off
	// (ANDing to ensure we only affect the LED output)
	LPC_GPIO2->FIOSET = ((~ledstate) & (1));
}
