# rotation-sensor
Multipole AS5047D motor shaft sensor emulating standard UVW hall effect sensor signals

Please note, that AS5047d position is shifted by 1mm from the center of the board.

Emulates Hall sensors UVW signals for 14-pole BLDC motor.


## Building
`platformio run -e ATMEGA328P`

## Flashing
This code is supposed to be uploaded via standard AVR ISP programmer.
Please set fuses to `L=0xF7 H=0xD9 E=0xFF`

`avrdude -c usbasp -p m328p -P usb -e -U lfuse:w:0xF7:m -U hfuse:w:0xD9:m -U efuse:w:0xFF:m`

http://eleccelerator.com/fusecalc/fusecalc.php?chip=atmega328p&LOW=F7&HIGH=D9&EXTENDED=FF&LOCKBIT=FF

upload with `platformio run -t program -e ATMEGA328P`

## Testing
`platformio test -e native`