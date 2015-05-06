add_library('serial')
from Esplora_Processing import Esplora

xCal = 0
yCal = 0

def setup():
    global xCal, yCal
    Esplora.open(Serial)
    size(255,255)
    xCal, yCal = Esplora.readJoystick()
    background(0)
    
def draw():
    bttn0, bttn1, bttn2, bttn3, bttnJoy = Esplora.readButtons()
    if bttn0 == 0:
        exit()
    if bttnJoy == 0:
        background(0)
    size = map(Esplora.readSlider(), 0, 1023, 31, 1)
    bright = map(Esplora.readLightSensor(), 0, 255, 0, 255)
    stroke(bright)
    fill(bright)
    xRaw, yRaw = Esplora.readJoystick()
    x = map(xRaw - xCal, -512, 511, 0, 255)
    y = map(yRaw - yCal, -512, 511, 0, 255)
    if x < 0:
        x = 0
    if x > width - size:
        x = width - size
    if y < 0:
        y = 0
    if y > height - size:
        y = height - size
    rect(x, y, size, size)
    
