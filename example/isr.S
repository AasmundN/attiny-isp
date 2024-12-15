; 
; isr.S
; 
; Author : AasmundN
;
; Assemble and link using:
;   avr-gcc -mmcu=attiny15 isr.S -o isr.out
;   avr-objcopy -O binary isr.out isr.bin
;

#define INT0_bm (1 << INT0)
#define PIN3_bm (1 << PB3)

; I/O register offset
#define __SFR_OFFSET 0
#include <avr/io.h>


.section .text

.global main
main:
  rcall io_init
loop:                           ; main program loop
  rjmp loop

io_init:                        ; initialize gpio
  sbi DDRB, PB3                 ; PB3 as outputs
  ldi r16, INT0_bm              ; enable INT0 interrupt on falling edge (default)
  out GIMSK, r16
  sei                           ; enable global interrupts
  ret


.global INT0_vect               ; INT0 handler
INT0_vect:
  sbic PINB, PB2                ; really simple debounce
  reti 

  in r16, PINB                  ; read pin values
  ldi r17, PIN3_bm
  eor r16, r17                  ; toggle led on PB3
  out PORTB, r16

  reti


.global __vector_default        ; default interrupt vector
__vector_default:
    reti
    
.end

