add_library('serial')
from Esplora_Processing import Esplora

xCal = 0
yCal = 0
zCal = 0

def setup():
    global xCal, yCal, zCal
    Esplora.open(Serial)
    size(255,255)
    xCal, yCal, zCal = Esplora.readAccelerometer()
    background(0)
    
def draw():
    xRaw, yRaw, zRaw = Esplora.readAccelerometer()
    x = map(xRaw - xCal, 127, -128, 0, 255)
    y = map(yRaw - yCal, 127, -128, 255, 0)
    z = map(zRaw - zCal, 127, -128, 0, 255)
    size = map(Esplora.readSlider(), 0, 1023, 31, 1)
    #bright = map(Esplora.readLightSensor(), 0, 255, 0, 255)
    stroke(z)
    fill(z)
    rect(x, y, size, size)
    
