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

/*
 * header of archive transferred from the flash
 */
static Header header;

/*
 * chunk(part of archive) transferred from the flash
 */
volatile static Chunk chunk1;

volatile static Chunk chunk2;

/*
 * end of archive transferred from the flash
 */
volatile uint8_t end;

/*
 * index of current chunk
 */
static int current = 0;

/*
 * configuration of DMA before channel initialization
 * called once at the beginning to enable and power DMA
 */
void dma_config(void) {
	//power DMA
	LPC_SC->PCONP |= (1 << 29);
	//clear interrupts
	LPC_GPDMA->DMACIntErrClr = 0xff;
	LPC_GPDMA->DMACIntTCClear = 0xff;
	//enable DMA
	LPC_GPDMA->DMACConfig = 1;
	while (!(LPC_GPDMA->DMACConfig & 0x01))
		;
	//enable interrupts
	NVIC_EnableIRQ(DMA_IRQn);
}

/*
 * initialization of header transfer
 */
void get_header(void) {
	//control bits for specific transfer and channel
	unsigned int control = ((2) | (0x0FFF)) | (2 << 18) | (2 << 21) | (1 << 26)
			| (1 << 27) | (1 << 31);
	//header start address
	LPC_GPDMACH0->DMACCSrcAddr = sector_start_address[FLASH_USER_HEADER_SECTOR];
	//header destination address
	LPC_GPDMACH0->DMACCDestAddr = (uint32_t) &header;
	//set control bits
	LPC_GPDMACH0->DMACCControl |= control;
	//enable transfer and interrupts (TC and EI)
	LPC_GPDMACH0->DMACCConfig |= 0x0c001;
}

/*
 * initialization of chunk (archive part) transfer
 * @param src source address of chunk to be transferred
 * @param dest destination  address of chunk transferred chunk
 * @param transer_size size of chunk
 */
void transfer_chunk(uint32_t src, uint32_t dest, int transfer_size) {
	//control bits for specific transfer and channel
	unsigned int control = (((16 + transfer_size) / 4) | (0x0FFF)) | (2 << 18)
			| (2 << 21) | (1 << 26) | (1 << 27) | (1 << 31);
	//source start address
	LPC_GPDMACH0->DMACCSrcAddr = src;
	//destination start address
	LPC_GPDMACH0->DMACCDestAddr = dest;
	//set control bits
	LPC_GPDMACH0->DMACCControl |= control;
	//enable transfer and interrupts (TC and EI)
	LPC_GPDMACH0->DMACCConfig |= 0x0c001;

}

/*
 * handler for terminal count interrupt and error interrupt
 */
void DMA_IRQHandler(void) {

	//clear interrupts before handling
	clear_interupts();

	if (current == 0) {
		verify_header();
	} else {
		//check if end of archive
		if ((uint16_t) current == header.chunk_number) {
			check_end();
		} else if (!verify()) {
			//verification failed, disable interrupts (stop verifying archive)
			NVIC_DisableIRQ(DMA_IRQn);
			//TODO: izgasi dma!!
		}
	}
}

/*
 * header verification
 */
void verify_header() {
	//valid preamble, new transfer to be initialized
	if (header.preambula == FLASH_USER_HEADER_BLOCK_DATA) {
		init_new_transfer();
	} else {
		//signal for error
		led_yellow_on();
	}
}

/*
 *initialization of new transfer (part of archive)
 */
void init_new_transfer() {

	//transfer not finished
	if (current == header.chunk_number - 1) {
		get_end();
	} else {
		uint32_t src =
				(uint32_t) ((sector_start_address[FLASH_USER_PAYLOAD_START_SECTOR])
						+ (current * (16 + header.chunk_size)));
		uint32_t chunk_address = (uint32_t) (current % 2) ? &chunk1 : &chunk2;
		transfer_chunk(src, chunk_address, header.chunk_size);
	}
//	if (current < header.chunk_number - 1) {
//		uint32_t src =
//				(uint32_t) ((sector_start_address[FLASH_USER_PAYLOAD_START_SECTOR])
//						+ (current * (16 + header.chunk_size)));
//		uint32_t chunk_address = (uint32_t) (current % 2) ? &chunk1 : &chunk2;
//		transfer_chunk(src, chunk_address, header.chunk_size);
//
//	} else {
//		get_end();
//	}
	current++;
}

/*
 * clear all pending interrupts
 */
void clear_interupts(void) {
	LPC_GPDMA->DMACIntTCClear = 1;
	int error_status = LPC_GPDMA->DMACIntErrStat;
	if (error_status & 1) {
		LPC_GPDMA->DMACIntErrClr = 1;
		error_occured();
		return;
	}
}
/*
 * verify chunk
 * @return 0 if error, 1 if success
 */
int verify(void) {
//	//local chunk for verifying
//	Chunk local_chunk;
//	int j = 0;
//	//copying data
//	for (j = 0; j < 16; ++j) {
//		local_chunk.hash[j] = chunk.hash[j];
//	}
//	for (j = 0; j < PAYLOAD_SIZE_BYTES; ++j) {
//		local_chunk.part[j] = chunk.part[j];
//	}

//start parallel transfer
	init_new_transfer();
// MD5 verification
	MD5_CTX ctx;
	MD5_Init(&ctx);
	if (current % 2 == 0) {
		MD5_Update(&ctx, chunk1.part, header.chunk_size);
	} else {
		MD5_Update(&ctx, chunk2.part, header.chunk_size);
	}
	unsigned char result[16];
	MD5_Final(result, &ctx);
//comparing result with hash
	int i = 0;
	uint8_t hash[16];
	for (i = 0; i < 16; ++i) {
		if (current % 2 == 0) {
			hash[i] = chunk1.hash[i];
		} else {
			hash[i] = chunk2.hash[i];
		}

	}

	for (i = 0; i < 6; ++i) {
		if (*(result + i) != hash[i]) {
			//signal error
			led_yellow_on();
			return 0;
		}
	}
	return 1;
}

/*
 * initialization of end of archive
 */
void get_end() {
	uint32_t src =
			(uint32_t) ((sector_start_address[FLASH_USER_PAYLOAD_START_SECTOR])
					+ ((current + 1) * (16 + header.chunk_size)));
	unsigned int control = 1 | (1 << 26) | (1 << 27) | (1 << 31);
	LPC_GPDMACH0->DMACCSrcAddr = src;
	LPC_GPDMACH0->DMACCDestAddr = (uint32_t) &end;
	LPC_GPDMACH0->DMACCControl |= control;
	LPC_GPDMACH0->DMACCConfig |= 0x0c001;
}

/*
 * verifying end block
 */
void check_end() {

	if (end == FLASH_USER_END_BLOCK_DATA) {
		led_green_on();
	} else {
		led_yellow_on();
	}
}
