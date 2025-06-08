from optparse import check_builtin
import mesa

NUMBER_OF_CELLS = 50

LEFT = -1
RIGHT = 1
NOT = 0
UP = 1
DOWN = -1

BUSY = 0
FREE = 1

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
        self.bx = -1
        self.by = -1
        self.next_x = self.x
        self.next_y = self.y
        self.x_dir = RIGHT
        self.y_dir = NOT
        self.ff = True

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
        elif act == "move":
            self.move()
        else:
            self.wait()



    # Robot decision model

    def deliberate(self):
        """
        
        Simple rule-based architecture, should determine the action to execute based on the robot state.

        """
        action = ""

        
        if self.isBusy:
            if (self.x == 0 and self.y==NUMBER_OF_CELLS-2) or (self.x==1 and self.y == NUMBER_OF_CELLS-1):
                action = "drop_off"
            else: 
                action = "move_payload"
        
        else:
            if self.bx<0:
                self.find_box()
            if (abs(self.x - self.bx)==1 and self.y==self.by) or (self.x==self.bx and abs(self.y - self.by)==1):
                action = "pick"
            elif self.bx>=0:
                action = "move"
            else:
                action = "wait"

        return action

    
    # Robot actions

    def find_path(self):
        if not self.isBusy:
            if(self.x == self.bx):
                self.x_dir = NOT
                if(self.y < self.by):
                    self.y_dir = UP
                else:
                    self.y_dir = DOWN

            elif(self.y==self.by):
                self.y_dir = NOT
                if(self.x < self.bx):
                    self.x_dir = RIGHT
                else:
                    self.x_dir = LEFT

            else:
                self.y_dir = NOT
                if(self.x<self.bx):
                    self.x_dir = RIGHT
                else:
                    self.x_dir = LEFT

        else:
            if(self.x == 0):
                if(self.y < NUMBER_OF_CELLS-1):
                    self.y_dir = UP
                    self.x_dir = NOT
                else:
                    self.y_dir = DOWN
                    self.x_dir = NOT

            else:
                if(self.x<0):
                    self.x_dir = RIGHT
                    self.y_dir = NOT
                else:
                    self.x_dir = LEFT
                    self.y_dir = NOT


    def find_box(self):
        def sortFirst(val):
            return val[0]
        boxxs = self.model.get_agents_of_type(Box)
        distance = [[abs(self.x- A.x)+abs(self.y -A.y),A] for A in boxxs if A.state ==FREE]
        if len(distance)>0:
            distance.sort(key=sortFirst)
            self.bx = distance[0][1].x
            self.by = distance[0][1].y
            distance[0][1].state = BUSY
        else:
            self.bx = NUMBER_OF_CELLS*2
        



    def move_payload(self):
        
        
        """
        * Obtains the box whose id is in the payload (Hint: you can use the method: self.model.schedule.agents to iterate over existing agents.)
        * move the payload together with the robot
        """
        self.move()
        boxx = self.model.get_agents_of_type(Box)
        box = [A for A in boxx if (A.x == self.bx and A.y == self.by)]
        if self.next_x != self.x or self.next_y != self.y:
            self.bx = self.x
            self.by = self.y
            box[0].next_x = self.x
            box[0].next_y = self.y
        # TODO implement

    def wait(self):
        """
        Keep the same position as the current one.
        """
        pass

    def move(self):
        if self.bx ==NUMBER_OF_CELLS*2:
            self.model.grid.remove_agent(self)
            self.remove()
        else:
            if self.ff:
                self.find_path()
            """
            Move the robot towards the boxes from left to right.
            """
            self.ff = True
            frontRob = [A for A in self.model.get_agents_of_type(PickerRobot) if (A.next_x==(self.x+self.x_dir) and A.next_y ==(self.y+self.y_dir)) or (A.x==(self.x+self.x_dir) and A.y ==(self.y+self.y_dir))]
            frontBox = [A for A in self.model.get_agents_of_type(Box) if (A.next_x==(self.x+self.x_dir) and A.next_y ==(self.y+self.y_dir)) or (A.x==(self.x+self.x_dir) and A.y ==(self.y+self.y_dir))]
            if len(frontRob)+len(frontBox)==0:
                self.next_x=self.x+self.x_dir
                self.next_y=self.y+self.y_dir
            else:
                self.ff = False
                if self.x ==0:
                    pass
                else:
                    g = self.x_dir
                    if self.x_dir==0:
                        self.x_dir= -self.y_dir
                    else:
                        self.x_dir = NOT
                    if self.y_dir == 0:
                        self.y_dir = -g
                    else:
                        self.y_dir = NOT
                    if self.x+self.x_dir in (0,NUMBER_OF_CELLS) and self.y+self.y_dir in (0,NUMBER_OF_CELLS):
                        frontRob = [A for A in self.model.get_agents_of_type(PickerRobot) if (A.next_x==(self.x+self.x_dir) and A.next_y ==(self.y+self.y_dir))  or (A.x==(self.x+self.x_dir) and A.y ==(self.y+self.y_dir))]
                        frontBox = [A for A in self.model.get_agents_of_type(Box) if (A.next_x==(self.x+self.x_dir) and A.next_y ==(self.y+self.y_dir))  or (A.x==(self.x+self.x_dir) and A.y ==(self.y+self.y_dir))]
                        if len(frontRob)+len(frontBox)==0:
                            self.next_x=self.x+self.x_dir
                            self.next_y=self.y+self.y_dir
        
    def pick(self):
        """
        * change robot state to Busy
        * find out the id of the box next to the robot
        * store the box id in the payload of the robot
        """
        self.state = BUSY
    
    def drop_off(self):
        """
        * change state of the robot to Free
        * Get the Box whose id is in the payload and remove it from the grid and change its state to Done.
        * Remove payload from robot
        * move agent to next position ahead of the box
        """
        self.state = FREE
        boxx = self.model.get_agents_of_type(Box)
        box = [A for A in boxx if A.x == self.bx and A.y == self.by]
        self.model.grid.remove_agent(box[0])
        box[0].remove()
        self.bx = -1
        self.by = -1
   
    def advance(self):
        self.x = self.next_x
        self.y = self.next_y   
        self.model.grid.move_agent(self,[self.next_x,self.next_y])           



class Box(mesa.Agent):
    """Represents a Box in the warehouse."""
    def __init__(self, pos, model, init_state=FREE):
        """
        Intialise state and position of the box
        """
        super().__init__(id, model)
        self.state = init_state
        self.x, self.y = pos
        self.next_x = self.x
        self.next_y = self.y

    
    def advance(self):
        
        if self.x !=self.next_x or self.y != self.next_y:
            self.x = self.next_x
            self.y = self.next_y
            self.model.grid.move_agent(self,[self.x,self.y])
