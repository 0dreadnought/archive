from pydoc import doc
import mesa
import random
import numpy as np
from random import randint
from warehouseT3.agents import PickerRobot
from warehouseT3.agents import Box



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
        boxxs=[]
        while n_robots>0 or n_boxes>0:
            for contents, (x, y) in self.grid.coord_iter():
                n = self.random.random()
                if n < 0.01 and n_robots>0:
                    if([x,y] not in robs):
                        robs.append([x,y])
                        n_robots-=1
                if n > 0.99 and n_boxes>0 and x>0:
                    if([x,y] not in boxxs):
                        boxxs.append([x,y])
                        n_boxes-=1
        for (x,y) in robs:
            new_agent = PickerRobot((x,y),self)
            self.grid.place_agent(new_agent, (x, y))
            self.schedule.add(new_agent)
        
        for (x,y) in boxxs:
            new_agent = Box((x,y),self)
            self.grid.place_agent(new_agent, (x, y))
            self.schedule.add(new_agent)

        self.running = True

    def step(self):
        """
        * Run while there are Undone boxes, otherwise stop running model.
        """
        
        if (len(self.get_agents_of_type(Box))!=0):
            self.schedule.step()
        else:
            self.running=False