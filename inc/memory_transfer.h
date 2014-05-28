/*
 * memory_transfer.h
 *
 *  Created on: 26.5.2014
 *      Author: Bojan
 */

#ifndef MEMORY_TRANSFER_H_
#define MEMORY_TRANSFER_H_
#include "payload_generator.h"
#include "stdint.h"
typedef struct {
	uint16_t preambula;
	uint16_t chunk_number;
	uint32_t chunk_size;
} Header;

typedef struct {
	uint8_t hash[16];
	uint8_t part[PAYLOAD_SIZE_BYTES];
} Chunk;

void dma_config(void);
void get_header(void);
void transfer_chunk(uint32_t src, uint32_t dest, int transfer_size);
void clear_interupts();
void start_verf(void);
void error_occured(void);
int verify(void);
void check_end();
void verify_header();
void get_end();
void init_new_transfer();
#endif /* MEMORY_TRANSFER_H_ */
