#ifndef D_logic_H
#define D_logic_H

/* Libraries */
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* Constants */
#define BIT_LEN 2
#define BUFFER_LEN BIT_LEN + 1

/* Variables */
uint8_t input_buffer[BUFFER_LEN];

/* Functions */
char* logicprintHello(void);
void inputBuffer_block_init(void);
void arrayIntoBufferedValue(uint8_t* out_buf, uint8_t* in_buf, size_t len);
void bufferValueIntoArray(uint8_t (*out_buf)[BUFFER_LEN], uint8_t in_buf);
uint8_t inputBuffer_block_exec(uint8_t* input);

#endif

