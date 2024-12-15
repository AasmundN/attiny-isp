; 
; blinker.S
; 
; Author : AasmundN
;
; Simple LED blinker. Toggles all pins in PORTB
;
; Assemble and link using:
;   avr-gcc -nostdlib -mmcu=attiny15 blinker.S -o blinker.out
;   avr-objcopy -O binary blinker.out blinker.bin
;  
; WARN file ending must be .S
;

#define __SFR_OFFSET 0
#include <avr/io.h>

.global main
  
main:
  rcall gpio_init
main_loop:          ; main program loop
  in r16, PORTB     ; read PORTB
  ldi r17, 0xff

  eor r16, r17      ; toggle bits in PORTB
  out PORTB, r16    ; wirte PORTB

  rcall delay
  rcall main_loop


gpio_init:          ; initialize PORTB as output
  ldi r16, 0xff
  out DDRB, r16     ; direction output
  out PORTB, r16    ; drive all pins high
  ret


delay:              ; delay routine
  ldi r17, 0xff
  ldi r20, 0x00
delay_loop_outer:
  cp r17, r20
  breq end
  dec r17           
  ldi 18, 0xff
delay_loop_inner:
  nop
  nop
  nop
  nop
  nop
  dec r18
  cp r18, r20
  brne delay_loop_inner
  rjmp delay_loop_outer
end:
  ret


.end
