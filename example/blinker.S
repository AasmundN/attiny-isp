;
; ATtiny15L LED blinker using timer/counter0
; INT0 to change blinking rate
;

#define __SFR_OFFSET 0
#include <avr/io.h>

.global main
main:
  rcall gpio_init
  rcall timer_init
  sei                                     ; global interrupt enable
main_loop:                                ; main program loop
  rjmp main_loop

gpio_init:
  sbi DDRB, PB4                           ; LED pin as output
  sbi PORTB, PB4                          ; LED pin high
  sbi PORTB, PB2                          ; SW pin pullup
  ldi r16, (1 << ISC01)
  out MCUCR, r16                          ; INT0 falling edge
  ldi r16, (1 << INT0)
  out GIMSK, r16                          ; INT0 interrupt enable
  ret

timer_init:
  ldi r16, (1 << CS00) | (1 << CS02)
  out TCCR0, r16                          ; CK/1024
  ldi r16, (1 << TOIE0)
  out TIMSK, r16                          ; enable overflow interrupt
  ret

.global TIMER0_OVF_vect                   ; TIMER0 overflow isr
TIMER0_OVF_vect:
  in r16, PORTB                           ; toggle LED pin
  ldi r17, (1 << PB4)
  eor r16, r17
  out PORTB, r16
  reti

.global INT0_vect                         ; INT0 isr
INT0_vect:
  ldi r16, (1 << CS00)                    ; change timer prescaler to change blink rate
  in r17, TCCR0
  eor r17, r16
  out TCCR0, r17
  reti

.global __vector_default                  ; default interrupt vector
__vector_default:
  reti

.end
