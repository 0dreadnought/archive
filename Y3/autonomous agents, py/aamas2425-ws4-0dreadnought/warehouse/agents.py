import mesa

NUMBER_OF_CELLS = 50
BUSY = 0
FREE = 1
UNDONE = 0
DONE = 1

class PickerRobot(mesa.Agent):
    """Represents a Robot of the warehouse."""

    def __init__(self, id, pos, model, init_state=FREE):
        """
        Initialise state attributes, including:
          * current and next position of the robot
          * state (FREE/BUSY)
          * payload (id of any box the robot is carrying)
        """
        super().__init__(id, model)
        self.x, self.y = pos
        self.next_x, self.next_y = None, None
        self.state = init_state
        self.payload = []

    @property
    def isBusy(self):
        return self.state == BUSY

    def step(self):
        """
        * Obtain action as a result of deliberation
        * trigger action
        """
        action = getattr(self,self.make_decision())
        action()

    # Robot decision model
    
    def make_decision(self):
        """
        
        Simple rule-based architecture, should determine the action to execute based on the robot state.

        """
        action = "wait"
        if self.state == FREE:
            next_position =  (self.x + 1, self.y)
            if not self.model.grid.is_cell_empty(next_position):
                action =  "pick"
            elif next_position[0] < NUMBER_OF_CELLS -1 :
                action = "move_fw"
        else:   
            if self.pos[0]-1 == 0:
                action = "drop_off"
            else:
                action = "move_bw"
        return action

    
    # Robot actions

    def move(self):
        """
        Move robot to the next position.
        """

        self.model.grid.move_agent(self,(self.next_x,self.next_y))

    def move_payload(self):
        """
        * Obtains the box whose id is in the payload (Hint: you can use the method: self.model.schedule.agents to iterate over existing agents.)
        * move the payload together with the robot
        """
        box = [ a for a in self.model.schedule.agents if isinstance(a,Box) and a.unique_id == self.payload ]

        if len(box)>0:
            self.model.grid.move_agent(box[0],(self.x,self.y))

    def wait(self):
        """
        Keep the same position as the current one.
        """
        self.next_x = self.x
        self.next_y = self.y

    def move_fw(self):
        """
        Move the robot towards the boxes from left to right.
        """
        self.next_x = self.x+1
        self.next_y = self.y
        self.move()

    def move_bw(self):
        """
        Move the robot and the payload towards the collection point (right to left).
        """
        self.next_x = self.x-1
        self.next_y = self.y
        self.move()
        self.move_payload()

    def pick(self):
        """
        * change robot state to Busy
        * find out the id of the box next to the robot
        * store the box id in the payload of the robot
        """
        self.state = BUSY
        nbs = [nb for nb in self.model.grid.get_neighbors((self.x, self.y), False)]

        for i in range(len(nbs)):
            if isinstance(nbs[i],Box):
                box = nbs[0]
                self.payload = box.unique_id

    def drop_off(self):
        """
        * change state of the robot to Free
        * Get the Box whose id is in the payload and remove it from the grid and change its state to Done.
        * Remove payload from robot
        * move agent to next position ahead of the box
        """
        self.state = FREE
        box = [ a for a in self.model.schedule.agents if isinstance(a,Box) and a.unique_id == self.payload ]
        if len(box)>0:
            box[0].state = DONE
            self.model.grid.remove_agent(box[0])
        self.payload = None
        self.next_x = self.next_x + 1
        self.move()

    def advance(self):
       """
       Advances position of the robot.
       """
       self.x = self.next_x
       self.y = self.next_y


class Box(mesa.Agent):
    """Represents a Box in the warehouse."""
    def __init__(self, id, pos, model, init_state=UNDONE):
        """
        Intialise state and position of the box
        """
        super().__init__(id, model)
        self.state = UNDONE
        self.x, self.y = pos