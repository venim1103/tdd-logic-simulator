#include "CppUTest/TestHarness.h"

extern "C"
{
#include "logic.h"
}

uint8_t buffer[BUFFER_LEN];
uint8_t (*buffer_p)[BUFFER_LEN] = &buffer;
uint8_t buf = 0;

static uint8_t mask = 0x0f; // 4 bits

TEST_GROUP(logic)
{
  void updateBuffer()
  {
    inputBuffer_block_exec(input_buffer);
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

TEST(logic, ArrayReadCorrectlyIntoBuffer)
{
  uint8_t test_input[8][BUFFER_LEN] = {{0,0,0}, {0,0,1}, {0,1,0}, {0,1,1}, {1,0,0}, {1,0,1}, {1,1,0}, {1,1,1}};
  CHECK_EQUAL(0, buf);
  uint8_t bits = 0x00; // 0b000

  for(uint8_t i = 0; i < 8; i++)
  {
    bits = i; // 0b000 -> 0b111
    arrayIntoBufferedValue(&buf, test_input[i], BUFFER_LEN);
    BITS_EQUAL(bits, buf, mask);
    buf = 0;
  }
}

TEST(logic, BufferReadCorrectlyIntoArray)
{
  uint8_t test_input[8][BUFFER_LEN] = {{0,0,0}, {0,0,1}, {0,1,0}, {0,1,1}, {1,0,0}, {1,0,1}, {1,1,0}, {1,1,1}};

  buf = 0x00; // 000

  for(uint8_t i = 0; i < 8; i++)
  {
    buf = i; // 0b000 -> 0b111
    bufferValueIntoArray(buffer_p, buf);
    CHECK_EQUAL(test_input[i][0], *buffer_p[0]);  // 0b00[0] -> 0b11[1]
    CHECK_EQUAL(test_input[i][1], *buffer_p[1]);  // 0b0[0]0 -> 0b1[1]1
    CHECK_EQUAL(test_input[i][2], *buffer_p[2]);  // 0b[0]00 -> 0b[1]11
  }
}

TEST(logic, SentShiftRegisterBitsMovingCorrectly)
{
  #define TEST_NUM 3
  #define TEST_STEPS 4

  uint8_t output_bit = 0;
  uint8_t input_signal = 0;

  uint8_t test_input[TEST_NUM][TEST_STEPS][BUFFER_LEN] = {{{1,0,0}, {0,1,0}, {0,0,1}, {0,0,0}, }, // test0
                                                          {{1,1,0}, {0,1,1}, {0,0,1}, {0,0,0}, }, // test1
                                                          {{1,0,1}, {0,1,0}, {0,0,1}, {0,0,0}, }};// test2

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
    for(uint8_t step = 0; step < TEST_STEPS; step++)
    {
      output_bit = inputBuffer(input_signal);
      CHECK_EQUAL(test_input[test][step][0], shift_reg[0]);
      CHECK_EQUAL(test_buffer[test][step], 0);
    }
  }
  #undef TEST_NUM
  #undef TEST_STEPS
}

IGNORE_TEST(logic, ReceivedShiftRegisterBitsMovingCorrectly)
{
  #define TEST_NUM 3
  #define TEST_STEPS 7
  uint8_t test_input[TEST_NUM][TEST_STEPS][BUFFER_LEN] = {{{0,0,0}, {1,0,0}, {0,1,0}, {0,0,1}, {0,0,0}, {0,0,0}, {0,0,0}}, // test0
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
      CHECK_EQUAL(test_input[test][step][0], 0);
      CHECK_EQUAL(test_buffer[test][step], 0);
    }
  }
  #undef TEST_NUM
  #undef TEST_STEPS
}
