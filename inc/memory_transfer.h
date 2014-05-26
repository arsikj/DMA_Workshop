/*
 * memory_transfer.h
 *
 *  Created on: 26.5.2014
 *      Author: Bojan
 */

#ifndef MEMORY_TRANSFER_H_
#define MEMORY_TRANSFER_H_
#include "payload_generator.h"

#define NUMBER_OF_SECTORS FLASH_USER_SECTORS

static unsigned int headers[NUMBER_OF_SECTORS];

void init_transfer(void);

void config_channel(int *src, int *dest, int chunk_size, int number_of_chunks);

#endif /* MEMORY_TRANSFER_H_ */
