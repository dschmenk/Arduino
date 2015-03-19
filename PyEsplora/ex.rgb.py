#!/usr/bin/env python

from Esplora import *

#
# Open new canvas and connect to Esplora
#
print "Press Joystick button to exit..."
Esplora.open()
while Esplora.readButtons()[4] != 0:
    joystick = Esplora.readJoystick()
    pot      = Esplora.readSlider()
    red   = (joystick[0] + 512) / 4
    green = (joystick[1] + 512) / 4
    blue  = pot / 4
    Esplora.writeRGB(red, green, blue)
Esplora.close()
