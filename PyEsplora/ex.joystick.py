#!/usr/bin/env python

from Esplora import *

#
# Open new canvas and connect to Esplora
#
print "Press switch 1 to exit..."
Esplora.open()
while Esplora.readButtons()[0] != 0:
    joystick = Esplora.readJoystick()
    print joystick
Esplora.close()
