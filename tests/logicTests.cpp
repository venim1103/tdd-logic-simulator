#include "CppUTest/TestHarness.h"

extern "C"
{
#include "logic.h"
}

uint8_t buffer[BUFFER_LEN];
uint8_t buf = 0;

TEST_GROUP(logic)
{
  uint8_t mask = 0xf; // 4 bits

  void updateBuffer()
  {
    inputBuffer_block_exec(input_buffer);
    memcpy(buffer, input_buffer, BUFFER_LEN);
  }

  void setup()
  {
    memset(buffer, 0, sizeof(buffer)*BUFFER_LEN);
    inputBuffer_block_init();
  }

  void teardown()
  {
    memset(buffer, 0, sizeof(buffer)*BUFFER_LEN);
  }

};

IGNORE_TEST(logic, FailMe)
{
  FAIL("@logic: First test should fail!");
  STRCMP_EQUAL("Hello world!", logicprintHello());
}

TEST(logic, arrayReadCorrectlyIntoBuffer)
{
  uint8_t test_input[8][BUFFER_LEN] = {{0,0,0}, {0,0,1}, {0,1,0}, {0,1,1}, {1,0,0}, {1,0,1}, {1,1,0}, {1,1,1}};
  CHECK_EQUAL(0, buf);
  arrayIntoBufferedValue(&buf, test_input[0], BUFFER_LEN);
  BITS_EQUAL(0b000, buf, mask);
  buf = 0;

  arrayIntoBufferedValue(&buf, test_input[1], BUFFER_LEN);
  BITS_EQUAL(0b001, buf, mask);
  buf = 0;

  arrayIntoBufferedValue(&buf, test_input[2], BUFFER_LEN);
  BITS_EQUAL(0b010, buf, mask);
  buf = 0;

  arrayIntoBufferedValue(&buf, test_input[3], BUFFER_LEN);
  BITS_EQUAL(0b011, buf, mask);
  buf = 0;

  arrayIntoBufferedValue(&buf, test_input[4], BUFFER_LEN);
  BITS_EQUAL(0b100, buf, mask);
  buf = 0;

  arrayIntoBufferedValue(&buf, test_input[5], BUFFER_LEN);
  BITS_EQUAL(0b101, buf, mask);
  buf = 0;

  arrayIntoBufferedValue(&buf, test_input[6], BUFFER_LEN);
  BITS_EQUAL(0b110, buf, mask);
  buf = 0;

  arrayIntoBufferedValue(&buf, test_input[7], BUFFER_LEN);
  BITS_EQUAL(0b111, buf, mask);
  buf = 0;
}

TEST(logic, BufferReadCorrectlyIntoArray)
{
  buf = 0b000;
  uint8_t (*buffer_p)[BUFFER_LEN] = &buffer;

  bufferValueIntoArray(buffer_p, buf);
  CHECK_EQUAL(0, *buffer_p[0]); CHECK_EQUAL(0, *buffer_p[1]); CHECK_EQUAL(0, *buffer_p[2]);

  buf = 0b001;
  bufferValueIntoArray(buffer_p, buf);
  CHECK_EQUAL(0, *buffer_p[0]); CHECK_EQUAL(0, *buffer_p[1]); CHECK_EQUAL(1, *buffer_p[2]);

  buf = 0b010;
  bufferValueIntoArray(buffer_p, buf);
  CHECK_EQUAL(0, *buffer_p[0]); CHECK_EQUAL(1, *buffer_p[1]); CHECK_EQUAL(0, *buffer_p[2]);

  buf = 0b011;
  bufferValueIntoArray(buffer_p, buf);
  CHECK_EQUAL(0, *buffer_p[0]); CHECK_EQUAL(1, *buffer_p[1]); CHECK_EQUAL(1, *buffer_p[2]);

  buf = 0b100;
  bufferValueIntoArray(buffer_p, buf);
  CHECK_EQUAL(1, *buffer_p[0]); CHECK_EQUAL(0, *buffer_p[1]); CHECK_EQUAL(0, *buffer_p[2]);

  buf = 0b101;
  bufferValueIntoArray(buffer_p, buf);
  CHECK_EQUAL(1, *buffer_p[0]); CHECK_EQUAL(0, *buffer_p[1]); CHECK_EQUAL(1, *buffer_p[2]);

  buf = 0b110;
  bufferValueIntoArray(buffer_p, buf);
  CHECK_EQUAL(1, *buffer_p[0]); CHECK_EQUAL(1, *buffer_p[1]); CHECK_EQUAL(0, *buffer_p[2]);

  buf = 0b111;
  bufferValueIntoArray(buffer_p, buf);
  CHECK_EQUAL(1, *buffer_p[0]); CHECK_EQUAL(1, *buffer_p[1]); CHECK_EQUAL(1, *buffer_p[2]);
}

IGNORE_TEST(logic, inputShiftRegisterCorrectDirection)
{
  
}
