import mesa


class Cell(mesa.Agent):
    
    HORIZ = 1
    VERT = 0

    def __init__(self, pos, model, init_state=None):
        
        super().__init__(pos, model)
        self.x, self.y = pos
        self.state = init_state
        self._nextState = None
        self.next_x = self.x
        self.next_y = self.y

    @property
    def isHoriz(self):
        return self.state == self.HORIZ
    
    @property
    def isVert(self):
        return self.state == self.VERT    

    def step(self):

        n = self.random.random()

        if self.isHoriz:
            if n < 0.5:
                self.next_x = self.x+1
            else:
                self.next_x = self.x-1
        else:
            if n < 0.5:
                self.next_y = self.y+1
            else:
                self.next_y = self.y-1

        
        # Get the neighbors and apply the rules on whether to be alive or dead
        # at the next time step.

        
            
        
    def advance(self):
        self.x = self.next_x
        self.y = self.next_y
        try:
            self.model.grid.move_agent(self,[self.x,self.y])
        except:
            if self.isHoriz:
                self.model.grid.remove_agent(self)
                self.remove()
        
  #TODO Complete
