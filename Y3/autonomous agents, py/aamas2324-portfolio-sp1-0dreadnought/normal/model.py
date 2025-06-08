from pydoc import doc
import mesa

from random import randint
from normal.agents import Scout, Path, Bush, Picker



class Farm(mesa.Model):
    score = 0
    
    def __init__(self, nscouts, npickers, width = 9):

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

        for contents,(x,y) in self.grid.coord_iter():
            if(x%2==1):
                agent = Path((x,y),self)
                paths.append((x,y))
                if(x == river):
                    agent.state = 2
            else:
                if(count in spots):
                    agent = Path((x,y),self)
                    paths.append((x,y))
                else:    
                    agent = Bush((x,y),self)
            
            count+=1
            self.grid.place_agent(agent,(x,y))
            self.schedule.add(agent)

        while(nscouts>0):
            x = randint(0,width-1)
            y = randint(0,width-1)
            agent = Scout((x,y),self)
            self.grid.place_agent(agent,(x,y))
            self.schedule.add(agent)
            nscouts-=1
 
        while(npickers>0):
            p = randint(0,len(paths)-1)
            x,y = paths[p]
            paths.pop(p)
            agent = Picker((x,y),self)
            self.grid.place_agent(agent,(x,y))
            self.schedule.add(agent)
            npickers-=1

        self.running = True

    def step(self):
        
        if len([A for A in self.get_agents_of_type(Bush) if A.state ==0])>0:
            self.schedule.step()