#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#include "isp.h"
#include "spi.h"

#define HIGH_BYTE_bm 0b1000

static isp_state_t isp_state = ISP_INACTIVE;

static inline void reset_low(void) { PORTC.OUTCLR |= PIN3_bm; }
static inline void reset_high(void) { PORTC.OUTSET |= PIN3_bm; }

void ISP_init(void)
{
  SPI_init();

  // pin for driving device reset
  PORTC.DIR |= PIN3_bm;

  reset_low();
}

isp_status_t ISP_write_program(uint8_t args, uint16_t program[], int size)
{
  if (isp_state != ISP_ACTIVE)
    return ISP_ERROR;

  // erase chip memory
  if (args & ISP_ERASE)
  {
    ISP_chip_erase();

    // wait t_wd_erase
    _delay_ms(9);

    if (ISP_programming_enable() != ISP_SUCCESS)
      return ISP_ERROR;
  }

  // write program
  for (int addr = 0; addr < size; addr++)
    if (ISP_write_program_memory(addr, program[addr]) != ISP_SUCCESS)
      return ISP_ERROR;

  // read and verify program
  if (args & ISP_VERIFY)
  {
    for (int addr = 0; addr < size; addr++)
    {
      uint16_t value;
      isp_status_t status = ISP_read_program_memory(addr, &value);

      if (status != ISP_SUCCESS || value != program[addr])
        return ISP_ERROR;
    }
  }

  return ISP_SUCCESS;
}

/**
 * The following routines implement the ISP instruction set
 * See datasheet for command details.
 */

isp_status_t ISP_programming_enable(void)
{
  // pulse reset signal
  reset_high();
  _delay_us(2);
  reset_low();

  // wait for at least 20ms
  _delay_ms(20);

  // transmit command bytes
  SPI_shift_data(0xAC);
  SPI_shift_data(0x53);

  uint8_t echo = SPI_shift_data(DONT_CARE);
  SPI_shift_data(DONT_CARE);

  if (echo != 0x53)
    return ISP_ERROR;

  isp_state = ISP_ACTIVE;

  return ISP_SUCCESS;
}

isp_status_t ISP_chip_erase(void)
{
  if (isp_state != ISP_ACTIVE)
    return ISP_ERROR;

  SPI_shift_data(0xAC);
  SPI_shift_data(0x80);
  SPI_shift_data(DONT_CARE);
  SPI_shift_data(DONT_CARE);

  return ISP_SUCCESS;
}

isp_status_t ISP_write_program_memory(uint8_t word_addr, uint16_t instruction)
{
  if (isp_state != ISP_ACTIVE)
    return ISP_ERROR;

  SPI_shift_data(0x40 | HIGH_BYTE_bm);
  SPI_shift_data(0x00);
  SPI_shift_data(word_addr);
  SPI_shift_data((uint8_t)(instruction >> 8));

  _delay_ms(5);

  SPI_shift_data(0x40 & ~HIGH_BYTE_bm);
  SPI_shift_data(0x00);
  SPI_shift_data(word_addr);
  SPI_shift_data((uint8_t)instruction);

  _delay_ms(5);

  return ISP_SUCCESS;
}

isp_status_t ISP_read_program_memory(uint8_t word_addr, uint16_t *value)
{
  *value = 0x00;

  if (isp_state != ISP_ACTIVE)
    return ISP_ERROR;

  SPI_shift_data(0x20 | HIGH_BYTE_bm);
  SPI_shift_data(0x00);
  SPI_shift_data(word_addr);
  *value |= (uint16_t)SPI_shift_data(DONT_CARE) << 8;

  _delay_ms(5);

  SPI_shift_data(0x20 & ~HIGH_BYTE_bm);
  SPI_shift_data(0x00);
  SPI_shift_data(word_addr);
  *value |= (uint16_t)SPI_shift_data(DONT_CARE);

  _delay_ms(5);

  return ISP_SUCCESS;
}
