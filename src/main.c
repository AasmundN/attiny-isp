#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#include "gpio.h"
#include "isp.h"

// led blinker program, see example folder
#define PROGRAM_SIZE 27
uint16_t program[PROGRAM_SIZE] = {
    0xd006,
    0xb308,
    0xef1f,
    0x2701,
    0xbb08,
    0xd005,
    0xdffa,
    0xef0f,
    0xbb07,
    0xbb08,
    0x9508,
    0xef1f,
    0xe040,
    0x1714,
    0xf059,
    0x951a,
    0xef2f,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x952a,
    0x1724,
    0xf7c1,
    0xcff3,
    0x9508,
};

int main(void)
{
  GPIO_init();
  ISP_init();

  // enter programming mode
  while (ISP_programming_enable() != ISP_SUCCESS)
  {
    _delay_ms(500);
    GPIO_toggle_led();
  }

  isp_status_t status = ISP_write_program(ISP_ERASE_VERIFY, program, PROGRAM_SIZE);

  while (status != ISP_SUCCESS)
  {
    _delay_ms(500);
    GPIO_toggle_led();
  }

  while (1)
  {
  }

  return 0;
}
