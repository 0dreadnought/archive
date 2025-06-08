import mesa
import numpy as np
from random import randint

from extended.agents import Scout as es
from extended.agents import Path as ep
from extended.agents import Bush as eb
from extended.agents import Picker as epc

from normal.agents import Scout as ns
from normal.agents import Path as np
from normal.agents import Bush as nb
from normal.agents import Picker as npc

def get_avg_time(model):
    return model.tick/(model.score if model.score>0 else 1)

def get_score(model):
    return model.score

class Farm(mesa.Model):
    score = 0
    tick = 0
    energy_use=0
    
    def __init__(self, mode, nscouts, npickers, width = 9):

        super().__init__()
        self.schedule = mesa.time.SimultaneousActivation(self)
        self.grid = mesa.space.MultiGrid(width, width, torus=False)
        flip = True
        count = 0
        spots = [0]
        paths = []

        
        while(spots[len(spots)-1]<width*width):
            if flip:
                spot = spots[len(spots)-1] + 3*width-1
            else:
                spot = spots[len(spots)-1] + width + 1
            flip = not flip
            spots.append(spot)
        
        river = 0
        while(river%2==0):
            river = randint(0,width-1)
        
        if mode:
            for contents,(x,y) in self.grid.coord_iter():
                if(x%2==1):
                    agent = ep((x,y),self)
                    paths.append((x,y))
                    if(x == river):
                        agent.state = 2
                else:
                    if(count in spots):
                        agent = ep((x,y),self)
                        paths.append((x,y))
                    else:    
                        agent = eb((x,y),self)
                
                count+=1
                self.grid.place_agent(agent,(x,y))
                self.schedule.add(agent)
            while(nscouts>0):
                x = randint(0,width-1)
                y = randint(0,width-1)
                agent = es((x,y),self)
                self.grid.place_agent(agent,(x,y))
                self.schedule.add(agent)
                nscouts-=1
            x,y =[0,0]
            while(npickers>0):
                agent = epc((x,y),self)
                self.grid.place_agent(agent,(x,y))
                self.schedule.add(agent)
                npickers-=1
        else:
            for contents,(x,y) in self.grid.coord_iter():
                if(x%2==1):
                    agent = np((x,y),self)
                    paths.append((x,y))
                    if(x == river):
                        agent.state = 2
                else:
                    if(count in spots):
                        agent = np((x,y),self)
                        paths.append((x,y))
                    else:    
                        agent = nb((x,y),self)
                
                count+=1
                self.grid.place_agent(agent,(x,y))
                self.schedule.add(agent)
            while(nscouts>0):
                x = randint(0,width-1)
                y = randint(0,width-1)
                agent = ns((x,y),self)
                self.grid.place_agent(agent,(x,y))
                self.schedule.add(agent)
                nscouts-=1
            while(npickers>0):
                p = randint(0,len(paths)-1)
                x,y = paths[p]
                paths.pop(p)
                agent = npc((x,y),self)
                self.grid.place_agent(agent,(x,y))
                self.schedule.add(agent)
                npickers-=1

        self.running = True

        self.datacollector = mesa.DataCollector(
            model_reporters={
                "score": get_score,
                "time": get_avg_time,

            },
        )

    def step(self):
        self.tick = self.tick + 1
        self.schedule.step()
        self.datacollector.collect(self)
        