from optparse import check_builtin
import mesa

NUMBER_OF_CELLS = 50
BUSY = 0
FREE = 1
UNDONE = 0
DONE = 1

class PickerRobot(mesa.Agent):
    """Represents a Robot of the warehouse."""
    def __init__(self, pos, model, init_state=FREE):
        """
        Initialise state attributes, including:
          * current and next position of the robot
          * state (FREE/BUSY)
          * payload (id of any box the robot is carrying)
        """
        super().__init__(id, model)
        
        self.state = init_state
        self.x,self.y = pos
        self.bid = -2
        self.next_x = self.x
        self.next_y = self.y
  

    @property
    def isBusy(self):
        return self.state == BUSY

    def step(self):

        """
        * Obtain action as a result of deliberation
        * trigger action
        """
        act = self.deliberate()
        if act == "drop_off":
            self.drop_off()
        elif act == "move_payload":
            self.move_payload()
        elif act == "pick":
            self.pick()
        elif act == "move_fw":
            self.move_fw()
        else:
            self.wait()



    # Robot decision model

    def deliberate(self):
        """
        
        Simple rule-based architecture, should determine the action to execute based on the robot state.

        """
        action = ""
        
        if self.isBusy:
            if self.x == 0:
                action = "drop_off"
            else: 
                action = "move_payload"
        
        elif not self.isBusy:
            box = [A for A in self.model.get_agents_of_type(Box) if A.y == self.y and A.x == self.x+1]
            if box:
                action = "pick"
            elif self.x!=NUMBER_OF_CELLS-1:
                action = "move_fw"
            else:
                action = "wait"

        return action

    
    # Robot actions

    def move(self):
        """
        Move robot to the next position.
        """
        # TODO implement

    def move_payload(self):
        """
        * Obtains the box whose id is in the payload (Hint: you can use the method: self.model.schedule.agents to iterate over existing agents.)
        * move the payload together with the robot
        """
        self.next_x=self.x-1
        boxx = self.model.get_agents_of_type(Box)
        box = [A for A in boxx if A.id == self.bid]
        box[0].next_x = self.x
        # TODO implement

    def wait(self):
        """
        Keep the same position as the current one.
        """
        pass

    def move_fw(self):
        """
        Move the robot towards the boxes from left to right.
        """
        self.next_x=self.x+1
    
    def move_bw(self):
        """
        Move the robot and the payload towards the collection point (right to left).
        """
        self.next_x=self.x-1
        
    def pick(self):
        """
        * change robot state to Busy
        * find out the id of the box next to the robot
        * store the box id in the payload of the robot
        """
        self.state = BUSY
        box = [A for A in self.model.get_agents_of_type(Box) if A.y == self.y and A.x == self.x+1]
        self.bid = box[0].id
    
    def drop_off(self):
        """
        * change state of the robot to Free
        * Get the Box whose id is in the payload and remove it from the grid and change its state to Done.
        * Remove payload from robot
        * move agent to next position ahead of the box
        """
        self.state = FREE
        boxx = self.model.get_agents_of_type(Box)
        box = [A for A in boxx if A.id == self.bid]
        box[0].state = DONE
        self.model.grid.remove_agent(box[0])
        box[0].remove()
        self.bid = -1
   
    def advance(self):
       """
       Advances position of the robot.
       """
       self.x = self.next_x
       self.model.grid.move_agent(self,[self.x,self.y])


class Box(mesa.Agent):
    """Represents a Box in the warehouse."""
    def __init__(self, pos,id, model, init_state=UNDONE):
        """
        Intialise state and position of the box
        """
        super().__init__(id, model)
        self.state = init_state
        self.x, self.y = pos
        self.id = id
        self.next_x = self.x
        self.next_y = self.y

    
    def advance(self):

       self.x = self.next_x
       self.y = self.next_y
       self.model.grid.move_agent(self,[self.x,self.y])
