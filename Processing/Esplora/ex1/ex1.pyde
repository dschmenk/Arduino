"""
PONG! Demo - draw the ball
"""
xBall = 50
yBall = 10
sizeBall = 6

def setup():
    size(300,200) # set size of window

def updateBall(colorBall):
    global xBall, yBall
    # Draw ball
    fill(colorBall)
    rect(xBall, yBall, sizeBall, sizeBall)

def draw():
    background(0)
    updateBall(255)
