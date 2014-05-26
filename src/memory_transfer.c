/*
 * memory_transfer.c
 *
 *  Created on: 26.5.2014
 *      Author: Bojan
 */

#include "memory_transfer.h"
#include "LPC17xx.h"
void init_transfer(void) {

	LPC_GPDMA->DMACConfig = 0x01;
	while (!(LPC_GPDMA->DMACConfig & 0x01))
		;
}

void config_channel(int *src, int *dest, int chunk_size, int number_of_chunks) {
	int control = ((chunk_size * 2/*ne e dobro*/) | (0x0FFF)) | (2 << 18)
			| (2 << 21) | (1 << 26) | (1 << 27) | (1 << 31);

	LPC_GPDMACH0->DMACCSrcAddr = &src;
	LPC_GPDMACH0->DMACCDestAddr = &dest;
	LPC_GPDMACH0->DMACCControl = control;
	LPC_GPDMACH0->DMACCConfig = 0x0C001;
	NVIC_EnableIRQ(DMA_IRQn);
}

void DMA_IRQHandler(void) {
	LPC_GPDMA->DMACIntTCClear |= 1;
	int error_status = LPC_GPDMA->DMACIntErrStat;
	if (error_status & 1) {
		LPC_GPDMA->DMACIntErrClr |= 1;
		error_ocured(); //stavi crvenko da trepka
		return;
	}
	transfer_finish();
}
