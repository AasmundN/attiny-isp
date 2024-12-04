#include <avr/io.h>

#include "spi.h"

/**
 * Note on clock speed:
 *
 * The minimum SCK period for the ATtiny15L is 4 times
 * its clock period. The ATtiny15L runs at 1.6MHz and
 * the ATtiny SPI runs at 3.3MHz with prescaler of 16.
 */
void SPI_init(void)
{
  // select alternative SPI pins
  PORTMUX.CTRLB |= PORTMUX_SPI0_bm;

  // MOSI, SCK
  PORTC.DIR |= PIN2_bm | PIN0_bm;

  // sus
  SPI0.CTRLB |= SPI_SSD_bm;

  // master mode with prescaler 16
  SPI0.CTRLA |= SPI_PRESC_DIV16_gc | SPI_MASTER_bm;

  // enable SPI0
  SPI0.CTRLA |= SPI_ENABLE_bm;
}

uint8_t SPI_shift_data(uint8_t data)
{
  // write data to TX buffer
  SPI0.DATA = data;

  // wait for TX complete
  while (!(SPI0.INTFLAGS & SPI_IF_bm))
    ;

  // return received data
  return SPI0.DATA;
}
