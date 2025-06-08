from pydoc import doc
import mesa
import random
import numpy as np
from random import randint
from warehouse.agents import PickerRobot
from warehouse.agents import Box
from .agents import UNDONE



class Warehouse(mesa.Model):
    """ Model representing an automated warehouse"""
    def __init__(self, n_robots, n_boxes, width=50, height=50):
        """
            * Set schedule defining model activation
            * Sets the number of robots as per user input
            * Sets the grid space of the model
            * Create n Robots as required and place them randomly on the edge of the left side of the 2D space.
            * Create m Boxes as required and place them randomly within the model (Hint: To simplify you can place them in the same horizontal position as the Robots). Make sure robots or boxes do not overlap with each other.
        """
        super().__init__()
        self.schedule = mesa.time.SimultaneousActivation(self)
        self.grid = mesa.space.SingleGrid(width, height, torus=False)
        # TODO implement
        robs=[]
        while(len(robs)!=n_robots):
            y = randint(0,width-1)
            if y not in robs:
                robs.append(y)
        for x in robs:
            y = randint(0,2)
            rob = PickerRobot((y,x),self)
            self.grid.place_agent(rob, (y,x))
            self.schedule.add(rob)
        
        boxx=[]
        i=0
        while(len(boxx)!=n_boxes):
            y = height-1-randint(0,height*0.1)
            if([y,robs[i]] not in boxx):
                boxx.append([y,robs[i]])
            i+=1
            if i==len(robs):
                i=0
        i=0
        for x in boxx:
            box = Box((x[0],x[1]),i,self)
            i+=1
            self.grid.place_agent(box, (x[0],x[1]))
            self.schedule.add(box)

        self.running = True

    def step(self):
        """
        * Run while there are Undone boxes, otherwise stop running model.
        """
        
        if (len(self.get_agents_of_type(Box))!=0):
            self.schedule.step()
        else:
            self.running=False