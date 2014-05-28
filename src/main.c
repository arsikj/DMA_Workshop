/*
 ===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
 ===============================================================================
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include "payload_generator.h"
#include "iap_driver.h"

// TODO: insert other include files here
#include "led.h"

// TODO: insert other definitions and declarations here

int main(void) {
	e_iap_status iap_status;
	led_init();
	//notify user for data loading
	led_red_on();
	/* Fill the flash with payload and hash data */

	iap_status = (e_iap_status) generator_init();
	if (iap_status != CMD_SUCCESS)
		while (1)
			;   // Error !!!

	// TODO: insert code here
	button_init();
	//power off leds in case they work
	led_green_off();
	led_yellow_off();
	led_red_off();
	// Enter an infinite loop
	while (1) {

	}

	return 0;
}

/*
 * error in channel (red led blink)
 */
void error_occured(void) {
	while (1) {
		led_red_invert();
		int i = 0;
		for (i = 0; i < 10000000; ++i)
			;
	}
}
