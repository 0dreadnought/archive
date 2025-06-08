from pydoc import doc
import mesa
import random
import tracemalloc
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

from novel.agents import Scout as nvs
from novel.agents import Path as nvp
from novel.agents import Bush as nvb
from novel.agents import Picker as nvpc
from novel.agents import Maintainer as nvm

def get_score(model):
    return model.score

def get_euse(model):
    return model.energy_use

class Farm(mesa.Model):
    score = 0
    mode = None
    energy_use = 0
    
    def __init__(self, mode, nscouts, npickers, nmaintainers, width = 9):

        super().__init__()
        self.schedule = mesa.time.SimultaneousActivation(self)
        self.grid = mesa.space.MultiGrid(width, width, torus=False)
        flip = True
        count = 0
        spots = [0]
        paths = []
        self.mode = mode
        
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
        if mode == "Novel":
            for contents,(x,y) in self.grid.coord_iter():
                if(x%2==1):
                    agent = nvp((x,y),self)
                    paths.append((x,y))
                    if(x == river):
                        agent.state = 2
                else:
                    if(count in spots):
                        agent = nvp((x,y),self)
                        paths.append((x,y))
                    else:    
                        agent = nvb((x,y),self)
                
                count+=1
                self.grid.place_agent(agent,(x,y))
                self.schedule.add(agent)
            while(nscouts>0):
                x = randint(0,width-1)
                y = randint(0,width-1)
                agent = nvs((x,y),self)
                self.grid.place_agent(agent,(x,y))
                self.schedule.add(agent)
                nscouts-=1
            x,y =[0,0]
            while(npickers>0):
                agent = nvpc((x,y),self)
                self.grid.place_agent(agent,(x,y))
                self.schedule.add(agent)
                npickers-=1
            
            while nmaintainers>0:
                agent = nvm((x,y),self)
                self.grid.place_agent(agent,(x,y))
                self.schedule.add(agent)
                nmaintainers-=1

        elif mode=="Extended":
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
                "energy": get_euse

            },
        )

    def step(self):
        
        self.schedule.step()
        self.datacollector.collect(self)
