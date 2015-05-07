"""
Plot Esplora light, sound, and temperature in a scrolling window.
"""
add_library('serial')
from Esplora_Processing import Esplora

light = [0]
sound = [0]
temp  = [0]

def setup():
    global light, sound, temp
    Esplora.open(Serial)
    size(400,300)
    x = 1
    while x < width:
        light.append(height/2)
        sound.append(height/2)
        temp.append(height/2)
        x = x + 1
    
def draw():
    global light, sound, temp
    background(0)
    # Remove rightmost element from lists
    light.pop()
    sound.pop()
    temp.pop()
    # Map full light sensor range
    bright = map(Esplora.readLightSensor(), 0, 1023, height - 1, 0)
    # Map full sound sensor range
    loud = map(Esplora.readMicrophone(), 0, 695, height - 1, 0)
    # Map room temperature range
    hot = map(Esplora.readTempF(), 40, 100, height - 1, 0)
    # Insert new value into beginning of lists
    light.insert(0, bright)
    sound.insert(0, loud)
    temp.insert(0, hot)
    # Draw a plot of all three sensors
    x = 1
    while x < width:
        stroke(255, 0, 0) # Red
        line(x - 1, temp[x - 1], x, temp[x])
        stroke(0, 255, 0) # Green
        line(x - 1, sound[x - 1], x, sound[x])
        stroke(0, 0, 255) # Blue
        line(x - 1, light[x - 1], x, light[x])
        x = x + 1

