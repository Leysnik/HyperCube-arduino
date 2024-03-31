# HyperCube-arduino
A sketch for controlling the address LED strip using an IR remote control. \
There are 9 different modes of operation of the LED strip, changing brightness and speed. 
All parameters are recorded in EEPROM.
# Setup
This scetch uses the [FastLed](https://github.com/FastLED/FastLED), IRremote and EEPROM libraries for arduino. \
At first, set the pins of the strip and IR reciever, maximum amperage of the power supply(in mA) and number of LEDs on one edge. 
```
#define EdgeLeds _
#define EdgeCount 24
#define AmpMax ____
#define LedDI _
#define IRPin _
#define LedType WS2812
```
Redact ``` void Input() ``` switch cases for your IR remote control

