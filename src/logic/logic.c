#include "logic.h"

char* logicprintHello(void)
{
  char* output = "Hello world!";
  printf("%s", output);
  return output;
}

void inputBuffer_block_init(void)
{
  memset(input_buffer, 0, sizeof(input_buffer)*BUFFER_LEN);
}

void arrayIntoBufferedValue(uint8_t* out_buf, uint8_t* in_buf, size_t len)
{
  size_t counter = (len-1);
  uint8_t local_buf = *out_buf;
  for(size_t i = 0; i < len; i++)
  {
//    local_buf |= (uint8_t)(in_buf[i]<<i); // FOR OTHER WAY AROUND!
//    printf("Local_buf:%d\n", (int)local_buf);
    local_buf |= (uint8_t)(in_buf[counter]<<i);
    *out_buf = local_buf;
    counter--;
  }
}

void bufferValueIntoArray(uint8_t (*out_buf)[BUFFER_LEN], uint8_t in_buf)
{
  size_t counter = (BUFFER_LEN-1);
  uint8_t local_buf = 0;
  for(size_t i = 0; i < BUFFER_LEN; i++)
  {
    local_buf = (in_buf & ((uint8_t)(1<<counter)));
//    printf("local_buf:%d\n", local_buf);
    *out_buf[i] = local_buf > 0 ? 1 : 0;
    counter--;
  }
}

uint8_t inputBuffer_block_exec(uint8_t* input)
{
  uint8_t joku = *input;
  joku = 0;
  return joku;
}
