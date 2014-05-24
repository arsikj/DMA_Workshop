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

// TODO: insert other definitions and declarations here

int main(void)
{
    e_iap_status iap_status;

    /* Fill the flash with payload and hash data */
    iap_status = (e_iap_status) generator_init();
    if (iap_status != CMD_SUCCESS)
        while(1);   // Error !!!


    // TODO: insert code here

    // Enter an infinite loop
    while(1) {
    }

    return 0;
}
