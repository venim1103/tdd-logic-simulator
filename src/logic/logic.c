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
  memset(buf_array, 0, sizeof(buf_array)*BUFFER_LEN);
}

void arrayIntoBufferedValue(uint8_t *out_buf, uint8_t (*in_buf)[BUFFER_LEN], uint8_t direction_flag)
{
  size_t counter = 0;
  uint8_t local_out_buf = *out_buf;
  uint8_t local_in_buf = 0;
  for(size_t i = 0; i < BUFFER_LEN; i++)
  {
    counter = BUFFER_LEN-(i+1);
    if(direction_flag == 1)
    {
//      local_buf |= (*in_buf[i]<<i); // FOR OTHER WAY AROUND!
      local_in_buf = (*in_buf)[i]; // FOR OTHER WAY AROUND!
    } else
    {
      local_in_buf = (*in_buf)[counter];
    }
    local_out_buf |= (uint8_t)(local_in_buf<<i);
    *out_buf = local_out_buf;
//    printf("\n");
//    printf("counter: %ld, out_buffer: %d, in_buffer_array: %d %d %d, local_in_buffer:%d local_out_buffer:%d\n", counter, *out_buf, (*in_buf)[0], (*in_buf)[1], (*in_buf)[2], local_in_buf, local_out_buf);
  }
}

void bufferValueIntoArray(uint8_t (*out_buf)[BUFFER_LEN], uint8_t in_buf, uint8_t direction_flag)
{
  size_t counter = 0;
  uint8_t local_buf = 0;
  for(size_t i = 0; i < BUFFER_LEN; i++)
  {
    counter = BUFFER_LEN-(i+1);
    if(direction_flag == 1)
    {
//    local_buf = (in_buf & ((uint8_t)(1<<i)));
      local_buf = (in_buf & ((uint8_t)(1<<i)));
    } else
    {
      local_buf = (in_buf & ((uint8_t)(1<<counter)));
    }
//    printf("in_buf: %d, local_buf:%d\n", in_buf, local_buf);
    local_buf = (local_buf > 0) ? 1 : 0;
    (*out_buf)[i] = local_buf;
  }
}

uint8_t inputBuffer(uint8_t* buffer, uint8_t (*buffer_array)[BUFFER_LEN], uint8_t input_bit, uint8_t loaded_input, uint8_t load_flag, uint8_t direction_flag)
{
  uint8_t dir = 0;
  if(direction_flag == 0) dir = 0;
  if(direction_flag == 1) dir = 0;

  if(load_flag == 1)
  {
    bufferValueIntoArray(buffer_array, loaded_input, direction_flag);
    arrayIntoBufferedValue(buffer, buffer_array, dir);
//    printf("\n");
//    printf("buffer: %d, (*buffer_array): %d %d %d, input_bit:%d, loaded_input: %d\n", *buffer, (*buffer_array)[0], (*buffer_array)[1], (*buffer_array)[2], input_bit, loaded_input);
    return (*buffer_array)[BIT_LEN];  // Return last bit
  } else
  {
      printf("\ninput_bit: %d\n", input_bit);
//    arrayIntoBufferedValue(buffer, &local_val);
//    local_val = local_val >> 1;
//    bufferValueIntoArray(buffer_array, local_val);
    return (*buffer_array)[BIT_LEN];  // Return last bit
  }
  return 2;
}
