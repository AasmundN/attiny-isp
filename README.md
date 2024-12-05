# ATtiny In-circuit Serial Programmer (ISP)

In-circuit Serial Programmer for the first generation of [ATtiny microchontrollers](https://en.wikipedia.org/wiki/ATtiny_microcontroller_comparison_chart). These devices are very simple and can only be programmed in assembler; perfect for learning! The ISP programmer is implemented on an ATtiny1607.

## How to...

Compile the source files for ATtiny1607 using the GNU toolchain for AVR (avr-gcc etc) and CMake. You must also install the device family pack (DFP) for the ATtiny1607 and place it in a folder named _device_pack_. 

The example folder includes a simple led blinker program in assembler for the ATtiny15L. Commands for assembling and linking the program can be found in the same folder.

Will add circuit diagrams later...

## Further work

- Add a cool serial interface where you can paste programs to be written to the ATtiny program memory
- Document code
- Write a cool assembler program 
- PCB?

## Resources

- [ATtiny15L datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/doc1187.pdf)
- [ATtiny1607 datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/ATtiny807_1607-Data-Sheet-40002030A.pdf)
- [About AVR assembly](https://www.nongnu.org/avr-libc/user-manual/assembler.html#ass_isr)
