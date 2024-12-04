#ifndef __SPI__
#define __SPI__

#include <stdint.h>

#define DONT_CARE 0x00

void SPI_init(void);

uint8_t SPI_shift_data(uint8_t data);

#endif // __SPI__
