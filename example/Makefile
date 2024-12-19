.DEFAULT_GOAL := upload

build: 
	avr-gcc -mmcu=attiny15 blinker.S -o blinker.out
	avr-objcopy -O binary blinker.out blinker.bin

upload: build
	isp write blinker.bin
