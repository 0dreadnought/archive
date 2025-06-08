import mesa

from .cell import Cell


class ConwaysGameOfLife(mesa.Model):
        
   def __init__(self, width=50, height=50):

        # Set up the grid and schedule.

        # Use SimultaneousActivation which simulates all the cells
        # computing their next state simultaneously.  This needs to
        # be done because each cell's next state depends on the current
        # state of all its neighbors -- before they've changed.
        self.schedule = mesa.time.SimultaneousActivation(self)

        # Use a simple grid, where edges wrap around.
        self.grid = mesa.space.SingleGrid(width, height, torus=True)

        # Place a cell at each location, with some initialized to
        # ALIVE and some to DEAD.
        for contents, (x, y) in self.grid.coord_iter():
            cell = Cell((x, y), self)
            n = self.random.random()
            if n < 0.01:
                cell.state = cell.HORIZ
                self.grid.place_agent(cell, (x, y))
                self.schedule.add(cell)
            if n > 0.99:
                cell.state = cell.VERT
                self.grid.place_agent(cell, (x, y))
                self.schedule.add(cell)

        self.running = True

   def step(self):
        
      self.schedule.step()

   #TODO Complete
