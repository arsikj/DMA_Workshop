/*
 * memory_transfer.c
 *
 *  Created on: 26.5.2014
 *      Author: Bojan
 */

#include "memory_transfer.h"
#include "LPC17xx.h"
#include "md5.h"
#include "led.h"
Header header;
Chunk chunks[];
int current = 0;
int current_vrf = 0;
void dma_config(void) {
	//power dma
	LPC_SC->PCONP |= (1 << 29);
	//clear interrupts
	LPC_GPDMA->DMACIntErrClr = 0xff;
	LPC_GPDMA->DMACIntTCClear = 0xff;
	//enable dma
	LPC_GPDMA->DMACConfig = 1;
	while (!(LPC_GPDMA->DMACConfig & 0x01))
		;
	//enable interrupts
	NVIC_EnableIRQ(DMA_IRQn);
}

void get_header(void) {
	unsigned int control = ((2) | (0x0FFF)) | (2 << 18) | (2 << 21) | (1 << 26)
			| (1 << 27) | (1 << 31);
	LPC_GPDMACH0->DMACCSrcAddr = sector_start_address[FLASH_USER_HEADER_SECTOR];
	LPC_GPDMACH0->DMACCDestAddr = (uint32_t) &header;
	LPC_GPDMACH0->DMACCControl |= control;
	LPC_GPDMACH0->DMACCConfig |= 0x0c001;
}

void transfer_chunk(uint32_t src, uint32_t dest, int transfer_size) {
	unsigned int control = (((16 + transfer_size) / 4) | (0x0FFF)) | (1 << 18)
			| (2 << 21) | (1 << 26) | (1 << 27) | (1 << 31);

	LPC_GPDMACH0->DMACCSrcAddr = src; //koja adresa treba tuka
	LPC_GPDMACH0->DMACCDestAddr = dest;
	LPC_GPDMACH0->DMACCControl |= control;
	LPC_GPDMACH0->DMACCConfig |= 0x0c001;

}

void DMA_IRQHandler(void) {
	clear_interupts();

	if (chunks == 0) { //header transfered
		//Chunk c[header.chunk_size];
		//chunks = c;
	}
	if (current <= header.chunk_number) {
		uint32_t src = (sector_start_address[FLASH_USER_PAYLOAD_START_SECTOR])
				+ (current * (16 + header.chunk_size));
		transfer_chunk(src, &chunks[current], header.chunk_size);
		current++;
	} else {
//proveri kraj

	}

}
void clear_interupts(void) {
	LPC_GPDMA->DMACIntTCClear = 1;
	int error_status = LPC_GPDMA->DMACIntErrStat;
	if (error_status & 1) {
		LPC_GPDMA->DMACIntErrClr = 1;
		error_occured(); //stavi crvenko da trepka
		return;
	}
}

void start_verf(void) {
	while (1) {
		int res = verify();
		if (res == 2) {
			led_yellow_on();
			return;
		}
		if (!res) {
			led_green_on();
			return;
		}
	}
}

int verify(void) {
	if (current_vrf >= current) {
		return 1;
	}
	MD5_CTX ctx;
	MD5_Init(&ctx);
	MD5_Update(&ctx, chunks[current_vrf].part, header.chunk_size);
	unsigned char result[16];
	MD5_Final(result, &ctx);
	int i = 0;
	for (i = 0; i < 6; ++i) {
		if (*(result + i) != chunks[current_vrf].hash[i]) {
			return 2;
		}
	}
	current_vrf++;
	if (current_vrf == header.chunk_number) {
		return 0;
	}
	return 1;
}
