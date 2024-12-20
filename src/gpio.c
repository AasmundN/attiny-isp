#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "gpio.h"

void GPIO_init(void)
{
  // builtin led as output
  PORTB.DIR |= PIN7_bm;

  // pin change interrupt on builtin switch
  PORTC.PIN4CTRL |= PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;

  sei();
}

void GPIO_toggle_led(void) { PORTB.OUTTGL |= PIN7_bm; }

/**
 * Hardware reset on button press
 */
ISR(PORTC_PORT_vect)
{
  // rapidly blick builtin led
  for (int i = 0; i < 10; i++)
  {
    _delay_ms(50);
    PORTB.OUTTGL |= PIN7_bm;
  }

  // setup watchdog timer for hardware reset
  CCP = CCP_IOREG_gc;
  WDT.CTRLA |= WDT_PERIOD_8CLK_gc;

  while (1)
    ;
}
