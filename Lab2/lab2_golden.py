# -*- coding: utf-8 -*-
"""
Created on Sat Mar 12 19:21:51 2022

@author: ym
"""
import turtle
import time
import random
# TODO uncomment the following line to use pyserial package
#import serial

# Note the serial port dev file name
# need to change based on the particular host machine
# TODO uncomment the following two lines to initialize serial port
#serialDevFile = '/dev/cu.usbmodem14201'
#ser=serial.Serial(serialDevFile, 9600, timeout=0)

serialDevFile = 'COM4'
ser=serial.Serial(serialDevFile, 115200, timeout=0)


delay = 0.1

# Score
score = 0
high_score = 0
ppa = 10

# Set up the screen
wn = turtle.Screen()
wn.title("Snake Game by @TokyoEdTech (mod by YL)")
wn.bgcolor("green")
wn.setup(width=600, height=600)
wn.tracer(0) # Turns off the screen updates

# Snake head
head = turtle.Turtle()
head.speed(0)
head.shape("square")
head.color("black")
head.penup()
head.goto(0,0)
head.direction = "stop"

# Snake food
food_rd = turtle.Turtle()
food_rd.speed(0)
food_rd.shape("circle")
food_rd.color("red")
food_rd.penup()
food_rd.goto(0,100)








segments = []

# Pen
pen = turtle.Turtle()
pen.speed(0)
pen.shape("square")
pen.color("white")
pen.penup()
pen.hideturtle()
pen.goto(0, 260)
pen.write("Score: 0  High Score: 0  P/A: 10", align="center", font=("Courier", 24, "normal"))

# Functions
def go_up():
    if head.direction != "down":
        head.direction = "up"

def go_down():
    if head.direction != "up":
        head.direction = "down"

def go_left():
    if head.direction != "right":
        head.direction = "left"

def go_right():
    if head.direction != "left":
        head.direction = "right"

def move():
    if head.direction == "up":
        y = head.ycor()
        head.sety(y + 20)

    if head.direction == "down":
        y = head.ycor()
        head.sety(y - 20)

    if head.direction == "left":
        x = head.xcor()
        head.setx(x - 20)

    if head.direction == "right":
        x = head.xcor()
        head.setx(x + 20)
        
        
        
        
        
def shaken():
    food_rd.color("gold")
     
# Keyboard bindings
wn.listen()
wn.onkey(go_up, "w")
wn.onkey(go_down, "s")
wn.onkey(go_left, "a")
wn.onkey(go_right, "d")
#modify here
wn.onkey(shaken,"j")



# Main game loop
while True:
    wn.update()
    read = ser.readline()
    control_read = read.decode()
    controller = control_read.rstrip()
    if controller == 'w':
        go_up()
    elif controller == 's':
        go_down()
    elif controller == 'a':
        go_left()
    elif controller == 'd':
        go_right()
        
        
        
   

    # Check for a collision with the border
    if head.xcor()>290 or head.xcor()<-290 or head.ycor()>290 or head.ycor()<-290:
        time.sleep(1)
        head.goto(0,0)
        head.direction = "stop"

        # Hide the segments
        for segment in segments:
            segment.goto(1000, 1000)
        
        # Clear the segments list
        segments.clear()

        # Reset the score
        score = 0

        # Reset the delay
        delay = 0.1

        pen.clear()
        pen.write("Score: {}  High Score: {}  P/A: {}".format(score, high_score, ppa), align="center", font=("Courier", 24, "normal")) 


    # Check for a collision with the food
    if head.distance(food_rd) < 20:

        # TODO: notes by Prof. Luo
        # you need to send a flag to Arduino indicating an apple is eaten
        # so that the Arduino will beep the buzzer
        # Hint: refer to the example at Serial-RW/pyserial-test.py

        # Move the food to a random spot
        if food_rd.color=='red':
            x = random.randint(-290, 290)
            y = random.randint(-290, 290)
            food_rd.goto(x,y)
        
        # Add a segment
            new_segment = turtle.Turtle()
            new_segment.speed(0)
            new_segment.shape("square")
            new_segment.color("grey")
            new_segment.penup()
            segments.append(new_segment)

        # Shorten the delay
            delay -= 0.001
            
        # Increase the score
            score += 10
            ser.write(b'E')
            if score > high_score:
                high_score = score
        
            pen.clear()
            pen.write("Score: {}  High Score: {}  P/A: {}".format(score, high_score, ppa), align="center", font=("Courier", 24, "normal"))
        else:
            
            x = random.randint(-290, 290)
            y = random.randint(-290, 290)
            food_rd.color("red")
            food_rd.goto(x,y)
        
        # Add a segment
            new_segment = turtle.Turtle()
            new_segment.speed(0)
            new_segment.shape("square")
            new_segment.color("grey")
            new_segment.penup()
            segments.append(new_segment)

        # Shorten the delay
            delay -= 0.001

        # Increase the score
            score += 20
            ser.write(b'E')
            if score > high_score:
                high_score = score
        
            pen.clear()
            pen.write("Score: {}  High Score: {}  P/A: {}".format(score, high_score, ppa), align="center", font=("Courier", 24, "normal"))

    # Move the end segments first in reverse order
    for index in range(len(segments)-1, 0, -1):
        x = segments[index-1].xcor()
        y = segments[index-1].ycor()
        segments[index].goto(x, y)

    # Move segment 0 to where the head is
    if len(segments) > 0:
        x = head.xcor()
        y = head.ycor()
        segments[0].goto(x,y)

    move()    

    # Check for head collision with the body segments
    for segment in segments:
        if segment.distance(head) < 20:
            time.sleep(1)
            head.goto(0,0)
            head.direction = "stop"
        
            # Hide the segments
            for segment in segments:
                segment.goto(1000, 1000)
        
            # Clear the segments list
            segments.clear()

            # Reset the score
            score = 0

            # Reset the delay
            delay = 0.1
        
            # Update the score display
            pen.clear()
            pen.write("Score: {}  High Score: {}  P/A: {}".format(score, high_score, ppa), align="center", font=("Courier", 24, "normal")) 

    time.sleep(delay)

wn.mainloop()
