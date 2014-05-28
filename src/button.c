/*
 * button.c
 *
 *  Created on: 26.5.2014
 *      Author: Team 6: Bojan, Dejan, Mila
 */

#include "button.h"
#include "LPC17xx.h"
#include "memory_transfer.h"

/*
 * button interrupt initialization
 */
void button_init(void) {
	LPC_GPIO2->FIODIR |= (1 << 1); /* EINT1 as input */
	LPC_GPIOINT->IO2IntEnF |= (1 << 11); /* Enable falling edge irq 2.11 */
	NVIC_EnableIRQ(EINT3_IRQn); /* enable irq in nvic */
}
/*
 * interrupt button handler
 */
void EINT3_IRQHandler(void) {
	LPC_GPIOINT->IO2IntClr |= (1 << 11); // Clear pending interrupt
	while (LPC_GPIOINT->IntStatus & (1 << 11))
		; //wait to clear interrupt

	//start DMA transfers and verification
	dma_config();
	get_header();
}
