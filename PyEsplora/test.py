#!/usr/bin/env python

from Tkinter import *
from Esplora import *

led = 0

def ledToggle(event):
    global led
    led = 1 - led
    if led:
        portEsplora.write("D13=1\n")
    else:
        portEsplora.write("D13=0\n")
    
def draw():
    portEsplora.write("=A2\n")
    instr = portEsplora.readline(12).rstrip()
    angle = (1023 - int(instr))/2.84
    if angle > 359:
        angle = 359
    canvas.itemconfig(ovalSpin, extent=angle)
    portEsplora.write("=A3\n")
    instr = portEsplora.readline(12).rstrip()
    l = 255 - int(instr)/4
    color = '#%02x%02x%02x' % (l, l, l)
    canvas.itemconfig(ovalLight, fill=color)
    portEsplora.write("=D2\n")
    instr = portEsplora.readline(12).rstrip()
    if int(instr) == 0:
        color = "green"
    else:
        color = "red"
    canvas.itemconfig(ovalBttn0, fill=color)
    portEsplora.write("=D3\n")
    instr = portEsplora.readline(12).rstrip()
    if int(instr) == 0:
        color = "green"
    else:
        color = "red"
    canvas.itemconfig(ovalBttn1, fill=color)
    root.after(20, draw)


#
# Open new canvas and connect to Esplora
#
portEsplora = serial.Serial("/dev/tty.usbmodem411", 57600, timeout = 1)
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
portEsplora.close()
