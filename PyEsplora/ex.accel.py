#!/usr/bin/env python

from Tkinter import *
from Esplora import *

def draw():
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
    root.after(20, draw)

#
# Open new canvas and connect to Esplora
#
Esplora.open()
root=Tk()
root.title("Esplora")
canvas=Canvas(root, width = 400, height = 400, bg = 'black')
canvas.pack()
root.after(100, draw)
root.mainloop()
Esplora.close()
