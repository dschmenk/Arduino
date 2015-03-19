#!/usr/bin/env python
from Esplora import *

#
# Open new canvas and connect to Esplora
#
print "Press Switch 1 to exit..."
Esplora.open()
while Esplora.readButtons()[0] != 0:
    accel = Esplora.readAccelerometer()
    red   = (accel[0]) / 2
    if red < 0:
        red = 0
    green = (accel[1]) / 2
    if green < 0:
        green = 0
    blue  = (accel[2]) / 2
    if blue < 0:
        blue = 0
    Esplora.writeRGB(red, green, blue)
Esplora.close()
