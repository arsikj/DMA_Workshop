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

static Header header;
static Chunk chunk;
static uint64_t end;

static int current = 0;
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
	unsigned int control = (((16 + transfer_size) / 4) | (0x0FFF)) | (2 << 18)
			| (2 << 21) | (1 << 26) | (1 << 27) | (1 << 31);
	LPC_GPDMACH0->DMACCSrcAddr = src; //koja adresa treba tuka
	LPC_GPDMACH0->DMACCDestAddr = dest;
	LPC_GPDMACH0->DMACCControl |= control;
	LPC_GPDMACH0->DMACCConfig |= 0x0c001;

}

void DMA_IRQHandler(void) {
	clear_interupts();

	if (current == 0) {
		verify_header();
	} else {
		if (current == (header.chunk_number + 1)) {
			check_end();
		} else if (!verify()) {
			NVIC_DisableIRQ(DMA_IRQn);
		}
	}

}

void verify_header() {
	if (header.preambula == 0xABBA) {
		init_new_transfer();
	} else {
		led_yellow_on();
	}
}
void init_new_transfer() {

	if (current <= header.chunk_number) {
		uint32_t src = (uint32_t) ((0x00005000)
				+ (current * (16 + header.chunk_size)));
		transfer_chunk(src, (uint32_t) &chunk, header.chunk_size);
		current++;
	} else {
		get_end();
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
	while (verify())
		;
}

int verify(void) {
	Chunk local_chunk;
	int j = 0;
	for (j = 0; j < 16; ++j) {
		local_chunk.hash[j] = chunk.hash[j];
	}
	for (j = 0; j < PAYLOAD_SIZE_BYTES; ++j) {
		local_chunk.part[j] = chunk.part[j];
	}

	init_new_transfer();

	MD5_CTX ctx;
	MD5_Init(&ctx);
	MD5_Update(&ctx, local_chunk.part, header.chunk_size);
	unsigned char result[16];
	MD5_Final(result, &ctx);

	int i = 0;
	for (i = 0; i < 6; ++i) {
		if (*(result + i) != local_chunk.hash[i]) {
			led_yellow_on();
			return 0;
		}
	}
	return 1;
}
void get_end() {
	uint32_t src = (uint32_t) ((0x00005000)
			+ (header.chunk_number * (16 + header.chunk_size)));
	unsigned int control = (2 | (0x0FFF)) | (2 << 18) | (2 << 21) | (1 << 26)
			| (1 << 27) | (1 << 31);
	LPC_GPDMACH0->DMACCSrcAddr = src; //koja adresa treba tuka
	LPC_GPDMACH0->DMACCDestAddr = &end;
	LPC_GPDMACH0->DMACCControl |= control;
	LPC_GPDMACH0->DMACCConfig |= 0x0c001;
}
void check_end() {
	//proveri end
	int i = 0;
	for (i = 0; i < 8; ++i) {

	}
	led_green_on();
}
