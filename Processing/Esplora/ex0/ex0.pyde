add_library('serial')
from Esplora_Processing import Esplora

def setup():
    Esplora.open(Serial)
    size(400,300)
    
def draw():
    slider = map(Esplora.readSlider(), 1023, 0, 0, 255)
    background(slider)
    text("Hello", 0, 10)
    Esplora.writeLED(slider)
    
