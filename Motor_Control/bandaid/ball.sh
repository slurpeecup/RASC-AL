avr-gcc main.c -o main.elf -mmcu=atmega328 -DF_CPU=16000000UL -Os
avr-objcopy main.elf -O ihex main.hex
avrdude -c arduino -p m328p -b 57600 -U flash:w:"main.hex":a -P /dev/ttyUSB0
