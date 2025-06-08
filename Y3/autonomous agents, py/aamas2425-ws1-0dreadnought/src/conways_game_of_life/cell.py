import mesa


class Cell(mesa.Agent):
    
    DEAD = 0
    ALIVE = 1

    def __init__(self, pos, model, init_state=DEAD):
        
        super().__init__(pos, model)
        self.x, self.y = pos
        self.state = init_state
        self._nextState = None

    @property
    def isAlive(self):
        return self.state == self.ALIVE

    @property
    def neighbors(self):
        return self.model.grid.iter_neighbors((self.x, self.y), True)

    def step(self):
        
        # Get the neighbors and apply the rules on whether to be alive or dead
        # at the next time step.
        live_neighbors = sum(neighbor.isAlive for neighbor in self.neighbors)

        # Assume nextState is unchanged, unless changed below.
        self._nextState = self.state

        if self.isAlive:
            if live_neighbors < 2 or live_neighbors > 3:
                self._nextState = self.DEAD
        else:
            if live_neighbors == 3:
                self._nextState = self.ALIVE

    def advance(self):
        self.state = self._nextState
  #TODO Complete
