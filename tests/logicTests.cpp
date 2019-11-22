#include "CppUTest/TestHarness.h"

extern "C"
{
#include "logic.h"
}

uint8_t buffer[BUFFER_LEN];
uint8_t buf = 0;
static uint8_t test_input[8][BUFFER_LEN] = {{0,0,0}, {0,0,1}, {0,1,0}, {0,1,1}, {1,0,0}, {1,0,1}, {1,1,0}, {1,1,1}};

static uint8_t mask = 0x0f; // 4 bits

TEST_GROUP(logic)
{
  void updateBuffer()
  {
    memcpy(buffer, input_buffer, BUFFER_LEN);
  }

  void setup()
  {
    memset(buffer, 0, sizeof(buffer)*BUFFER_LEN);
    buf = 0;
    inputBuffer_block_init();
  }

  void teardown()
  {
    memset(buffer, 0, sizeof(buffer)*BUFFER_LEN);
    buf = 0;
  }

};

TEST(logic, InputWithPointersHaveSameValues)
{
  uint8_t test_input_num[BUFFER_LEN];
  memset(test_input_num, 0, sizeof(uint8_t)*BUFFER_LEN);
  uint8_t (*test_input_num_p)[BUFFER_LEN] = &test_input_num;

  for(uint8_t i = 0; i < 8; i++)
  {
    memcpy(*test_input_num_p, test_input[i], sizeof(uint8_t)*BUFFER_LEN);
    CHECK_EQUAL(test_input_num[i], (*test_input_num_p)[i]);
  }
}

TEST(logic, ArrayReadCorrectlyIntoBuffer)
{
  uint8_t test_input_num[BUFFER_LEN];
  memset(test_input_num, 0, sizeof(uint8_t)*BUFFER_LEN);
  uint8_t (*test_input_num_p)[BUFFER_LEN] = &test_input_num;

  CHECK_EQUAL(0, buf);
  uint8_t bits = 0x00; // 0b000

  for(uint8_t i = 0; i < 8; i++)
  {
    memcpy(*test_input_num_p, test_input[i], sizeof(uint8_t)*BUFFER_LEN);
    bits = i; // 0b000 -> 0b111
    arrayIntoBufferedValue(&buf, test_input_num_p);
    BITS_EQUAL(bits, buf, mask);
    buf = 0;
  }
  for(uint8_t i = 0; i < 8; i++)
  {
    bits = (uint8_t)(7-i); // 0b111 -> 0b000
    memcpy(*test_input_num_p, test_input[bits], sizeof(uint8_t)*BUFFER_LEN);
    arrayIntoBufferedValue(&buf, test_input_num_p);
    BITS_EQUAL(bits, buf, mask);
    buf = 0;
  }
}

TEST(logic, BufferReadCorrectlyIntoArray)
{
  uint8_t (*buffer_p)[BUFFER_LEN] = &buffer;
  buf = 0x00; // 000

  for(uint8_t i = 0; i < 8; i++)
  {
    buf = i; // 0b000 -> 0b111
    bufferValueIntoArray(buffer_p, buf);
    CHECK_EQUAL(test_input[i][0], (*buffer_p)[0]);  // 0b00[0] -> 0b11[1]
    CHECK_EQUAL(test_input[i][1], (*buffer_p)[1]);  // 0b0[0]0 -> 0b1[1]1
    CHECK_EQUAL(test_input[i][2], (*buffer_p)[2]);  // 0b[0]00 -> 0b[1]11
  }

  for(int i = 0; i < 8; i++)
  {
    buf = uint8_t(7-i); // 0b111 -> 0b000
    bufferValueIntoArray(buffer_p, buf);
    CHECK_EQUAL(test_input[buf][0], (*buffer_p)[0]);  // 0b00[0] -> 0b11[1]
    CHECK_EQUAL(test_input[buf][1], (*buffer_p)[1]);  // 0b0[0]0 -> 0b1[1]1
    CHECK_EQUAL(test_input[buf][2], (*buffer_p)[2]);  // 0b[0]00 -> 0b[1]11
  }
}

TEST(logic, LoadCorrectValueIntoShiftRegister)
{
  #define TEST_NUM 3

  uint8_t shift_reg[TEST_NUM] = {0x04, 0x06, 0x05};
  uint8_t local_output_buf = 0;
  uint8_t output_bit = 0;
  uint8_t input_bit = 0;
  uint8_t load_values = 0;
  uint8_t load_flag = 0;
  uint8_t local_output_buf_array[BUFFER_LEN];
  memset(local_output_buf_array, 0, sizeof(uint8_t)*BUFFER_LEN);
  uint8_t (*local_output_buf_array_p)[BUFFER_LEN] = &local_output_buf_array;

  for(uint8_t test = 0; test < TEST_NUM; test++)
  {
    load_flag = 1;
    load_values = shift_reg[test];
    local_output_buf = 0; // REMEMBER TO INITIALIZE THE OUTPUT BEFORE USING THE FUNCTION!
    output_bit = inputBuffer(&local_output_buf, local_output_buf_array_p, input_bit, load_values, load_flag);
    load_flag = 0;
    CHECK_EQUAL(shift_reg[test], local_output_buf);
    CHECK_EQUAL((*local_output_buf_array_p)[BIT_LEN], output_bit);
  }
  #undef TEST_NUM
}

/*
IGNORE_TEST(logic, SentShiftRegisterBitsMovingCorrectly)
{
  #define TEST_NUM 3
  #define TEST_STEPS 4

  uint8_t local_output_buf = 0;
  uint8_t output_bit = 0;
  uint8_t input_bit = 0;
  uint8_t load_values = 0;
  uint8_t load_flag = 0;
  uint8_t local_output_buf_array[BUFFER_LEN];
  memset(local_output_buf_array, 0, sizeof(local_output_buf_array)*BUFFER_LEN);
  uint8_t (*local_output_buf_array_p)[BUFFER_LEN] = &local_output_buf_array;
  printf("*local_output_buf_array_p: %d %d %d", *local_output_buf_array_p[0],*local_output_buf_array_p[1],*local_output_buf_array_p[2]);

  uint8_t test_input_moving[TEST_NUM][TEST_STEPS][BUFFER_LEN] = {{{1,0,0}, {0,1,0}, {0,0,1}, {0,0,0}}, // test0
                                                          {{1,1,0}, {0,1,1}, {0,0,1}, {0,0,0}}, // test1
                                                          {{1,0,1}, {0,1,0}, {0,0,1}, {0,0,0}}};// test2

                                          //   0b100 0b010 0b001 0b000 
  uint8_t test_buffer[TEST_NUM][TEST_STEPS] = {{0x04, 0x02, 0x01, 0x00}, //test0
        
                                          //   0b110 0b011 0b001 0b000
                                               {0x06, 0x03, 0x01, 0x00}, //test1
                 
                                          //   0b101 0b010 0b001 0b000 
                                               {0x05, 0x02, 0x01, 0x00}};//test2
                 
                             // 0b100 0b110 0b101
  uint8_t shift_reg[TEST_NUM] = {0x04, 0x06, 0x05};

  for(uint8_t test = 0; test < 1; test++)
  {
    load_flag = 1;
    load_values = shift_reg[test];
    
    for(uint8_t step = 0; step < TEST_STEPS; step++)
    {
      for(uint8_t bit = 0; bit < BUFFER_LEN; bit++)
      { 
        input_bit = output_bit; // flow bits forward
        output_bit = inputBuffer(&local_output_buf, local_output_buf_array_p, input_bit, load_values, load_flag);
        CHECK_EQUAL(test_input_moving[test][step][bit], output_bit);
      }
      CHECK_EQUAL(test_buffer[test][step], local_output_buf);
    }
    load_flag = 0; // disable after each test
  }
  #undef TEST_NUM
  #undef TEST_STEPS
}

IGNORE_TEST(logic, ReceivedShiftRegisterBitsMovingCorrectly)
{
  #define TEST_NUM 3
  #define TEST_STEPS 7
  uint8_t test_input_moving[TEST_NUM][TEST_STEPS][BUFFER_LEN] = {{{0,0,0}, {1,0,0}, {0,1,0}, {0,0,1}, {0,0,0}, {0,0,0}, {0,0,0}}, // test0
                                                          {{0,0,0}, {1,0,0}, {1,1,0}, {0,1,1}, {0,0,1}, {0,0,0}, {0,0,0}}, // test1
                                                          {{0,0,0}, {1,0,0}, {0,1,0}, {1,0,1}, {0,1,0}, {0,0,1}, {0,0,0}}};// test2

                                          //   0b000 0b100 0b010 0b001 0b000 0b000 0b000
  uint8_t test_buffer[TEST_NUM][TEST_STEPS] = {{0x00, 0x04, 0x02, 0x01, 0x00, 0x00, 0x00}, //test0

                                          //   0b000 0b100 0b110 0b011 0b001 0b000 0b000
                                               {0x00, 0x04, 0x06, 0x03, 0x01, 0x00, 0x00}, //test1

                                          //   0b000 0b100 0b010 0b101 0b010 0b001 0b000 
                                               {0x00, 0x04, 0x02, 0x05, 0x02, 0x01, 0x00}};//test2
         
  for(uint8_t test = 0; test < 1; test++)
  {
    for(uint8_t step = 0; step < TEST_STEPS; step++)
    {
      CHECK_EQUAL(test_input_moving[test][step][0], 0);
      CHECK_EQUAL(test_buffer[test][step], 0);
    }
  }
  #undef TEST_NUM
  #undef TEST_STEPS
}
*/
