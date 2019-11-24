#include "logic.h"

void inputBuffer_block_init(void)
{
  buffer_value = 0;
  buffer_p = &buffer;
  buffer_value_p = &buffer_value;
  memset(input_buffer, 0, sizeof(uint8_t)*BUFFER_LEN);
  memset(buffer, 0, sizeof(uint8_t)*BUFFER_LEN);
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
      local_in_buf = (*in_buf)[i]; // FOR OTHER WAY AROUND!
    } else
    {
      local_in_buf = (*in_buf)[counter];
    }
    local_out_buf |= (uint8_t)(local_in_buf<<i);
    *out_buf = local_out_buf;
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
      local_buf = (in_buf & ((uint8_t)(1<<i)));
    } else
    {
      local_buf = (in_buf & ((uint8_t)(1<<counter)));
    }
    local_buf = (local_buf > 0) ? 1 : 0;
    (*out_buf)[i] = local_buf;
  }
}

uint8_t inputBuffer(uint8_t* buffer_input, uint8_t (*buffer_array)[BUFFER_LEN], uint8_t input_bit, uint8_t loaded_input, uint8_t load_flag, uint8_t direction_flag)
{
  uint8_t output = (*buffer_array)[BIT_LEN];
  uint8_t dir = 0;
  if(direction_flag == 0) dir = 0;
  if(direction_flag == 1) dir = 0;

  if(load_flag == 1)
  {
    bufferValueIntoArray(buffer_array, loaded_input, direction_flag);
    arrayIntoBufferedValue(buffer_input, buffer_array, dir);
//    printf("buffer_input: %d, (*buffer_array): %d %d %d, input_bit:%d, loaded_input: %d\n", *buffer_input, (*buffer_array)[0], (*buffer_array)[1], (*buffer_array)[2], input_bit, loaded_input);
//    printf("\n");
//    printf("buffer_input: %d, (*buffer_array): %d %d %d, input_bit:%d, loaded_input: %d\n", *buffer_input, (*buffer_array)[0], (*buffer_array)[1], (*buffer_array)[2], input_bit, loaded_input);

    output = (*buffer_array)[BIT_LEN];
    return output;  // Return last bit
  } else // SHIFT BITS:
  {
    output = (*buffer_array)[BIT_LEN];
//    for(uint8_t i = 1; i < BUFFER_LEN; i++)
//    {
//      (*buffer_array)[BUFFER_LEN-i] = (*buffer_array)[BIT_LEN-i]; // SHIFT BITS, EXCEPT LAST!
//    }
    (*buffer_array)[2] = (*buffer_array)[1]; // SHIFT BITS, EXCEPT LAST!
    (*buffer_array)[1] = (*buffer_array)[0]; // SHIFT BITS, EXCEPT LAST!
    (*buffer_array)[0] = input_bit;  // SHIFT INPUT INTO FIRST BIT
//    printf("\n");
//    printf("buffer_input: %d, (*buffer_array): %d %d %d, input_bit:%d, loaded_input: %d\n", *buffer_input, (*buffer_array)[0], (*buffer_array)[1], (*buffer_array)[2], input_bit, loaded_input);
//    arrayIntoBufferedValue(buffer, &local_val);
//    local_val = local_val >> 1;
//    bufferValueIntoArray(buffer_array, local_val);
    return output;  // Return last bit
  }
  return 2;
}
