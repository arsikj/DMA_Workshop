/*
 * button.c
 *
 *  Created on: 26.5.2014
 *      Author: Bojan
 */

#include "button.h"
#include "LPC17xx.h"

void button_init(void) {
	LPC_GPIO2->FIODIR |= (1 << 1); /* EINT1 as input */
	LPC_GPIOINT->IO2IntEnF |= (1 << 11); /* Enable falling edge irq 2.11 */
	NVIC_EnableIRQ(EINT3_IRQn); /* enable irq in nvic */
}
void EINT3_IRQHandler(void) {

	LPC_GPIOINT->IO2IntClr |= (1 << 11); // Clear pending interrupt
	while (LPC_GPIOINT->IntStatus & (1 << 11)); //wait to clear interupt
	button_click();
}
