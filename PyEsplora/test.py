#!/usr/bin/env python

from Tkinter import *
from Esplora import *

led = 0

def ledToggle(event):
    global led
    led = 1 - led
    Esplora.writeDigital(13, led)
    
def draw():
    angle = (1023 - Esplora.readPotentiometer())/2.84
    if angle > 359:
        angle = 359
    canvas.itemconfig(ovalSpin, extent=angle)
    l = Esplora.readLightSensor()/4
    color = '#%02x%02x%02x' % (l, l, l)
    canvas.itemconfig(ovalLight, fill=color)
    l = Esplora.readMicrophone() * 16
    if l > 255:
        l = 255
    color = '#%02x%02x%02x' % (0, l, 0)
    canvas.itemconfig(ovalBttn0, fill=color)
    l = (Esplora.readTempF() - 65) * 64
    if l < 0:
        l = 0
    if l > 255:
        l = 255
    color = '#%02x%02x%02x' % (l, 0, 0)
    canvas.itemconfig(ovalBttn1, fill=color)
    root.after(20, draw)

#
# Open new canvas and connect to Esplora
#
Esplora.open()
root=Tk()
root.title("Esplora")
canvas=Canvas(root, width = 400, height = 400, bg = 'black')
canvas.bind("<Button-1>", ledToggle)
ovalSpin  = canvas.create_arc(10, 10, 190, 190, start=0, extent=1, fill="blue")
ovalLight = canvas.create_oval(210, 10, 390, 190)
ovalBttn0 = canvas.create_oval((10, 210, 190, 390), fill='red')
ovalBttn1 = canvas.create_oval((210, 210, 390, 390), fill='red')        
canvas.pack()
root.after(100, draw)
root.mainloop()
Esplora.close()
