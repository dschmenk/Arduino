# Arduino
Arduino, Python, and Processing library source code for tethering the Esplora to a PC running Python/Processing.

## Esplora Processing Interface
Download the Esplora package for Processing
1. Open http://github.com/dschmenk/Arduino in Safari.
2. Click on 'Download ZIP' on right side of screen
3. Drag the Downloads/Arduino-master folder to the Documents/Processing folder.

## Using libraries and modules
Start a new sketch, add the serial port library, import the Esplora module and see how easy it is to use. From the Processing menu, click through "Sketch/Add File..." and add the "Arduino-master/Processing/Esplora/Esplora_Processing.py" file.

## A simple example
```
add_library('serial')
from Esplora_Processing import Esplora

def setup():
    Esplora.open(Serial)
    
def draw():
    background(map(Esplora.readSlider(), 1023, 0, 0, 255))
```
## Programming the Esplora firmware
If your Esplora needs to be programmed with the serial protocol firmware for use with Python, open the SerEsplora project in the Arduino IDE and update the Esplora.

## Basic API:
	Esplora.open(Serial): Open Esplora connection for use
		return: None

	Esplora.close(): Close Esplora connection
		return: None

	Esplora.writeLED(red, green, blue): Write RGB (or brightness) to LED
 		return: None

	Esplora.writeBuzzer(hertz, duration): Write buzzer tone
  		return: None

	Esplora.readJoystick(): Read joystick position
 		return: [-512..511, -512..511]

	Esplora.readAccelerometer(): Read accelerometer values
 		return: [-512..511, -512..511, -512..511]

	Esplora.readButtons(): Read 4 diamond pad buttons and joystick button
		return: [0..1, 0..1, 0..1, 0..1, 0..1]

	Esplora.readSlider(): Read slider value
		return: 0..1023

	Esplora.readLightSensor(): Read light level
		return: 0..1023

	Esplora.readMicrophone(): Read sound level
		return: 0..1023

	Esplora.readTempC(): Read temperature in Celsius
		return: -100..200

	Esplora.readTempF(): Read temperature in Fahrenheit
		return: -100..200


## Low-Level API:
	Esplora.readDigital(pin): Read the value on a digital pin
		return: 0..1

	Esplora.readAnalog(pin): Read the value on an analog pin
		return: 0..255

	Esplora.writeDigital(pin, value): Write a value to a digital pin
		return: None

	Esplora.writePwm(pin, value): Write a value to an analog (PWM) pin
		return: None


## TFT API (for optional Esplora TFT screen):
	Esplora.tftSize(): Read the size of the TFT display
		return: [tftWidth, tftHeight]

	Esplora.tftBackground(red, green, blue): Fill TFT background with color
		return: None

	Esplora.tftFill(red, green, blue): Set fill color for 2D primitives
		return: None

	Esplora.tftStroke(red, green, blue): Set stroke color for 2D primitives
		return: None

	Esplora.tftNoFill(): Don't fill 2D primitives
		return: None

	Esplora.tftNoStroke(): Don't outline 2D primitives
		return: None

	Esplora.tftRect(xpos, ypos, width, height): Draw a rectangle
		return: None

	Esplora.tftLine(x1, y1, x2, y2): Draw a line
		return: None

	Esplora.tftCircle(xpos, ypos, radius): Draw a circle
		return: None

	Esplora.tftText(string, xpos, ypos): Draw a text string
		return: None

	Esplora.tftTextSize(size): Set the text size
		return: None
 

