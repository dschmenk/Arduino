add_library('serial')
from Esplora_Processing import Esplora

xBall = 50
yBall = 10
sizeBall = 6
xMove = 1
yMove = 1
paddle = 0
widthPaddle = 30
heightPaddle = 6

def setup():
    size(300,200) # set size of window
    Esplora.open(Serial)

def updatePaddle(colorPaddle):
    global paddle
    # Update paddle position
    paddle = map(Esplora.readSlider(), 1023, 0, 0, width - widthPaddle)
    fill(colorPaddle)
    rect(paddle, height - heightPaddle, widthPaddle, heightPaddle)
    
def updateBall(colorBall):
    global xBall, yBall, xMove, yMove
    # Update ball position
    xBall = xBall + xMove
    yBall = yBall + yMove
    fill(colorBall)
    rect(xBall, yBall, sizeBall, sizeBall)
    # Bounce ball
    if xBall > width - sizeBall or xBall < 0:
        xMove = -xMove # Bounce off walls
    if yBall < 0:
        yMove = -yMove # Bounce off top
    if yBall > height - heightPaddle - sizeBall:
        # Check if ball hit paddle or game over
        if xBall + sizeBall >= paddle and xBall <= paddle + widthPaddle:
            yMove = -yMove # Bounce off paddle
        else:
            exit() # Missed ball - game over
    
def draw():
    background(0)
    updatePaddle(128)
    updateBall(255)
