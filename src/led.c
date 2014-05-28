/*
 * led.c
 *
 *  Created on: 25.5.2014
 *      Author: Team 6: Mila, Dejan, Bojan
 */

#include "led.h"
#include "LPC17xx.h"

/*
 * initialization of leds
 */
void led_init(void) {
	//Set P0.26 to 00 - GPIO
	LPC_PINCON->PINSEL1 &= (~(3 << 20));
	//Set P2.0 to 00 -GPIO
	LPC_PINCON->PINSEL4 &= (~3);
	//Set P0.22 to 00 -GPIO
	LPC_PINCON->PINSEL1 &= (~(3 << 12));
	//Set GPIO - P0.26 - to be output
	LPC_GPIO0->FIODIR |= (1 << 26);
	//Set GPIO - P2.0 to be output
	LPC_GPIO2->FIODIR |= 1;
	//Set GPIO - P0.22 to be output
	LPC_GPIO0->FIODIR |= (1 << 22);
}

/**
 * writing high level at pin 26 on GPIO0
 */
void led_green_on(void) {
	LPC_GPIO0->FIOSET = (1 << 26);
}

/**
 * writing high level at pin 0 on GPIO2
 */
void led_yellow_on(void) {
	LPC_GPIO2->FIOSET = 1;
}

/**
 * writing high level at pin 22 on GPIO0
 */
void led_red_on(void) {
	LPC_GPIO0->FIOSET = (1 << 22);
}

/**
 * writing low level at pin 26 on GPIO0
 */
void led_green_off(void) {
	LPC_GPIO0->FIOCLR = (1 << 26);
}

/**
 * writing low level at pin 0 on GPIO2
 */
void led_yellow_off(void) {
	LPC_GPIO2->FIOCLR = 1;
}

/**
 * writing low level at pin 22 on GPIO0
 */
void led_red_off(void) {
	LPC_GPIO0->FIOCLR = (1 << 22);
}

/*
 * inverting state of green led
 */
void led_green_invert(void) {
	int ledstate;
	// Read current state of GPIO P0_0..31, which includes green led
	ledstate = LPC_GPIO0->FIOPIN;
	// Turn off LED if it is on
	// (ANDing to ensure we only affect the LED output)
	LPC_GPIO0->FIOCLR = ledstate & (1 << 26);
	// Turn on LED if it is off
	// (ANDing to ensure we only affect the LED output)
	LPC_GPIO0->FIOSET = ((~ledstate) & (1 << 26));
}

/*
 * inverting state of yellow led
 */
void led_yellow_invert(void) {
	int ledstate;
	// Read current state of GPIO P0_0..31, which includes yellow led
	ledstate = LPC_GPIO2->FIOPIN;
	// Turn off LED if it is on
	// (ANDing to ensure we only affect the LED output)
	LPC_GPIO2->FIOCLR = ledstate & (1);
	// Turn on LED if it is off
	// (ANDing to ensure we only affect the LED output)
	LPC_GPIO2->FIOSET = ((~ledstate) & (1));
}

/*
 * inverting state of red led
 */
void led_red_invert(void) {
	int ledstate;
	// Read current state of GPIO P0_0..31, which includes LED2 (red)
	ledstate = LPC_GPIO0->FIOPIN;
	// Turn off LED if it is on
	// (ANDing to ensure we only affect the LED output)
	LPC_GPIO0->FIOCLR = ledstate & (1 << 22);
	// Turn on LED if it is off
	// (ANDing to ensure we only affect the LED output)
	LPC_GPIO0->FIOSET = ((~ledstate) & (1 << 22));
}
