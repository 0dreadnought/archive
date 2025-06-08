from pydoc import doc
import mesa
import numpy as np
from warehouse.agents import PickerRobot
from warehouse.agents import Box
from .agents import DONE,FREE,BUSY

def free_bots(model):
    return len([A for A in model.schedule.agents if isinstance(A,PickerRobot) and A.state == FREE])

def busy_bots(model):
    return len([A for A in model.schedule.agents if isinstance(A,PickerRobot) and A.state == BUSY])

def pending_boxes(model):
        """
        It counts how many boxes are still pending.
        """
        return len([a for a in model.schedule.agents if isinstance(a,Box) and a.state != DONE ])

class Warehouse(mesa.Model):
    """ Model representing an automated warehouse"""

    def __init__(self, n_robots=2, n_boxes=20, width=50, height=50,seed=123):
        """
            * Set schedule defining model activation
            * Sets the number of robots as per user input
            * Sets the grid space of the model
            * Create n Robots as required and place them randomly on the edge of the left side of the 2D space.
            * Create m Boxes as required and place them randomly within the model (Hint: To simplify you can place them in the same horizontal position as the Robots). Make sure robots or boxes do not overlap with each other.
        """
        self.tick = 0
        self.schedule = mesa.time.SimultaneousActivation(self)
        self.n_robots = n_robots
        self.n_boxes = n_boxes
        # Use a simple grid, where edges wrap around.
        self.grid = mesa.space.SingleGrid(width, height, torus=True)
        y_s = []
        for n in range(self.n_robots):
            heading = (1, 0)
            #append element in vector 
            x = 1
            y = 1
            while True:
                y = self.random.randint(1,height-1)
                if self.grid.is_cell_empty((x,y)):
                    break
            
            y_s.append(y)
            pr = PickerRobot(n,(x,y),self)
            self.schedule.add(pr)            
            self.grid.place_agent(pr,(x,y))

        for n in range(self.n_boxes):
            x = 1
            y = 1
            while True:
                x = self.random.randint(4,width-1)
                y = self.random.choice(y_s)
                if self.grid.is_cell_empty((x,y)):
                    break

            b = Box(n+self.n_robots,(x,y),self)
            self.schedule.add(b)            
            self.grid.place_agent(b,(x,y))

        self.running = True

        # (1 of 2) We create a Data Collector for our model.
        # Data Collector allows to collect key data from the model (environment) and the individual agents.
        # key:value pairs define the label and function or attributes (model or agent) that we want to collect data from.
        self.datacollector = mesa.DataCollector(
            model_reporters={
                            "pending_boxes": pending_boxes,
                            "free_robots": free_bots,
                            "busy_robots": busy_bots
                            #TODO Need to add fields for BUSY and FREE robots
                            #HINT you can get all agents from scheduler and then filter them by state. You can create a separate function or use lambda expressions.
            }, 
            #TODO Need to add fields for the type of agent.
            #HINT: You can use: str(a.__class__.__name__) to get the string from the type of an object a. 
        )


    def step(self):
        """
        * Run while there are Undone boxes, otherwise stop running model.
        """
        self.tick = self.tick + 1
        if len([a for a in self.schedule.agents if isinstance(a,Box) and a.state != DONE ]) > 0:
            self.schedule.step()
        else:
           self.running = False
        
        # (2 of 2) We tell the model to collect data at every step of the simulation
        self.datacollector.collect(self)
        