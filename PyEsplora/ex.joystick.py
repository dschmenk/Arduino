#!/usr/bin/env python

from Tkinter import *
from esplora import *

def draw():
    joystick = Esplora.readJoystick()
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
