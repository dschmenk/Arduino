#!/usr/bin/env python

from Esplora import *

#
# Open new canvas and connect to Esplora
#
Esplora.open()
tftSize = Esplora.tftSize()
Esplora.tftBackground(255,255,255)
Esplora.tftStroke(0,0,0)
for x in xrange(0, tftSize[0], 5):
    Esplora.tftLine(0, 0, x, tftSize[1] - 1)
for y in xrange(0, tftSize[1], 5):
    Esplora.tftLine(0, 0, tftSize[0] - 1, y)
Esplora.tftFill(128, 0, 255)
Esplora.tftRect(50, 50, 50, 10)
Esplora.tftStroke(0, 255, 128)
Esplora.tftText("Esplora!", 50, 50)
Esplora.close()
