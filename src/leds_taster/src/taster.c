/*
 * taster.c
 *
 *  Created on: 24.5.2014
 *      Author: Bojan
 */

#include <LPC17xx.h>

void init_taster(void) {
	NVIC_EnableIRQ(EINT3_IRQn);

}

void EINT3_IRQHandler(void) {
	//TODO povikue se funkcija sto treba da pocne ss parsiranje
	// http://stackoverflow.com/questions/22448446/writing-gpio-interrupt-handlers-for-lpc1769-cortex-m3
	check_file();
}

