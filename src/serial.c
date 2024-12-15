#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>

#include "serial.h"

#define USART_BAUD(BAUD) (F_CPU * 4 / BAUD)

void (*rx_cb)(uint8_t data);

void default_rx_cb(uint8_t data) { SERIAL_transmit_char(data); }

void SERIAL_init(void)
{
  cli();

  // set TX pin as output and drive high
  PORTB.OUTSET |= PIN2_bm;
  PORTB.DIR |= PIN2_bm;

  // set baud rate
  USART0.BAUD = USART_BAUD(9600);

  // R;X complete interrupt enable
  USART0.CTRLA |= USART_RXCIE_bm;

  // enable RX and TX
  USART0.CTRLB |= USART_TXEN_bm | USART_RXEN_bm;

  rx_cb = default_rx_cb;

  sei();
}

void SERIAL_transmit_char(char ch)
{
  // wait for transmit buffer empty
  while (!(USART0.STATUS & USART_DREIF_bm))
    ;

  // transmit data
  USART0.TXDATAL = ch;
}

void SERIAL_transmit_string(char *str)
{
  int ch_index = 0;
  while (str[ch_index] != '\0')
    SERIAL_transmit_char(str[ch_index++]);
}

void SERIAL_set_rx_cb(void (*cb)(uint8_t))
{
  rx_cb = cb;
}

ISR(USART0_RXC_vect)
{
  uint8_t data = USART0.RXDATAL;
  rx_cb(data);
}
