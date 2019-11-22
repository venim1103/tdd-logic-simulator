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
uint8_t buf_array[BUFFER_LEN];

/* Functions */
char* logicprintHello(void);
void inputBuffer_block_init(void);
void arrayIntoBufferedValue(uint8_t *out_buf, uint8_t (*in_buf)[BUFFER_LEN], uint8_t direction_flag);
void bufferValueIntoArray(uint8_t (*out_buf)[BUFFER_LEN], uint8_t in_buf, uint8_t direction_flag);
uint8_t inputBuffer(uint8_t* buffer, uint8_t (*buffer_array)[BUFFER_LEN], uint8_t input_bit, uint8_t loaded_input, uint8_t load_flag, uint8_t direction_flag);

#endif
