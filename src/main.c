#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#include "gpio.h"
#include "isp.h"
#include "serial.h"

typedef enum
{
  WRITE = 'w',
  WRITE_ERASE = 'e',
  WRITE_VERIFY = 'v',
  WRITE_VERIFY_ERASE = 'W',
  END_OF_DATA = 0x69, // must find a char that never occurs in programs
  ACK = 'a',
  SUCCESS = 's'
} command_t;

typedef enum
{
  IDLE,
  DOWNLOAD,
  PROGRAM,
} state_t;

volatile state_t state = IDLE;
volatile uint8_t program_buffer[512];
volatile int program_counter;
volatile int program_size;

void download_program(uint8_t byte); // forward declaration
void parse_command(uint8_t command)
{
  switch (command)
  {
  case WRITE_VERIFY_ERASE:
    state = DOWNLOAD;
    program_counter = 0;
    SERIAL_set_rx_cb(download_program);
    SERIAL_transmit_char(ACK);
    break;
  default:
    break;
  }
}

void download_program(uint8_t byte)
{
  // todo: check for buffer overflow
  program_buffer[program_counter++] = byte;

  if (byte != END_OF_DATA)
    return;

  state = PROGRAM;
  program_size = program_counter;
  SERIAL_set_rx_cb(parse_command);
}

int main(void)
{
  GPIO_init();
  ISP_init();
  SERIAL_init();
  SERIAL_set_rx_cb(parse_command);

  while (1)
  {
    if (state != PROGRAM)
      continue;

    if (ISP_programming_enable() != ISP_SUCCESS)
    {
      state = IDLE;
      SERIAL_set_rx_cb(parse_command);
      continue;
    }

    ISP_write_program(ISP_ERASE_VERIFY, (uint16_t *)program_buffer, program_size / 2);

    SERIAL_transmit_char(SUCCESS);

    state = IDLE;
    SERIAL_set_rx_cb(parse_command);
  }

  return 0;
}
