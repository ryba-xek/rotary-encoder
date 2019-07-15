# rotation-sensor
Multipole AS5047D motor shaft sensor emulating standard UVW hall effect sensor signals

Emulates Hall sensors UVW signals for 14-pole BLDC motor.

This code is supposed to be uploaded via standard AVR ISP programmer.
Please set fuses to `avrdude -c usbasp -p m328p -P usb -e -U lfuse:w:0xF7:m -U hfuse:w:0xD9:m -U efuse:w:0xFF:m`

http://eleccelerator.com/fusecalc/fusecalc.php?chip=atmega328p&LOW=F7&HIGH=D9&EXTENDED=FF&LOCKBIT=FF