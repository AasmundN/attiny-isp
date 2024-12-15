#ifndef __SERIAL__
#define __SERIAL__

#include <stdint.h>

void SERIAL_init(void);

void SERIAL_transmit_char(char ch);

void SERIAL_transmit_string(char *str);

void SERIAL_set_rx_cb(void (*cb)(uint8_t));

#endif // __SERIAL__
