#!/usr/bin/env python

from Tkinter import *
from esplora import *

def draw():
    joystick = Esplora.readJoystick()
    pot      = Esplora.readPotentiometer()
    red   = (joystick[0] + 512) / 4
    green = (joystick[1] + 512) / 4
    blue  = pot / 4
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
