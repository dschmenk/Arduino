"""
PONG! Demo - bounce the ball
"""
xBall = 50
yBall = 10
sizeBall = 6
xMove = 1
yMove = 1

def setup():
    size(300,200) # set size of window

def updateBall(colorBall):
    global xBall, yBall, xMove, yMove
    # Update ball position
    xBall = xBall + xMove
    yBall = yBall + yMove
    # Draw ball
    fill(colorBall)
    rect(xBall, yBall, sizeBall, sizeBall)
    # Bounce ball
    if xBall > width - sizeBall or xBall < 0:
        xMove = -xMove # Bounce off walls
    if yBall < 0:
        yMove = -yMove # Bounce off ceiling
    if yBall > height - sizeBall:
        yMove = -yMove # Bounce off floor

def draw():
    background(0)
    updateBall(255)
