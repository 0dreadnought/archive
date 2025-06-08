import mesa
from random import randint

import mesa.agent

FREE = 0
BUSY = 1
RTRN = 2
CHRG =3

GREN = 0
RIPE = 1

GRND = 1
RIVR = 2

class Scout(mesa.Agent):

    def __init__(self, pos, model, init_state=0):

        super().__init__(id, model)

        self.NUMBER_OF_CELLS = self.model.grid.width
        
        self.state = init_state
        self.x,self.y = pos
        self.next_x = self.x
        self.next_y = self.y
        self.last_state = -1

        self.last_x = -1
        self.last_y = -1

        self.targeted = False

        self.topbot = True
        self.leftright = True
        
        self.confused = True

        self.life = self.NUMBER_OF_CELLS*self.NUMBER_OF_CELLS*2

    @property
    def x_dir(self):
        if self.leftright:
            return 1
        return -1

    @property
    def y_dir(self):
        if self.topbot:
            return 1
        return -1
    
    @property
    def cur_bush(self):
        return [A for A in self.model.get_agents_of_type(Bush) if A.x == self.x and A.y == self.y]
        

    def step(self):
        if self.life<1:
            self.model.grid.remove_agent(self)
            self.remove()
        if self.life < self.NUMBER_OF_CELLS*2+2 or self.state == CHRG:
            self.chrg()
        elif self.state == RTRN:
            self.rtrn()
        elif self.state == BUSY:
            self.wait()
        else:
            self.move()
    
    def chrg(self):
        if self.state!=CHRG:
            self.last_state = self.state
        self.state = CHRG
        if self.last_x == -1:
            self.last_x,self.last_y = (self.x,self.y)
        if self.x == self.y == 0:
            self.maintain()
        else:
            if self.x != 0:
                self.next_x-=1
            else:
                self.next_y-=1

    def maintain(self):
        if(self.life < self.NUMBER_OF_CELLS*self.NUMBER_OF_CELLS*2):
            self.life += self.NUMBER_OF_CELLS*2+2
        else:
            self.state = RTRN

    def rtrn(self):
        if self.last_state == BUSY:
            if self.x != self.last_x:
                self.next_x+=1
            elif self.y != self.last_y:
                self.next_y+=1
            else:
                self.state = self.last_state
                self.last_x,self.last_y = (-1,-1)
                self.last_state = -1
        else:
            self.state = self.last_state
            self.confused = True
            self.last_state = -1
            self.last_x,self.last_y = (-1,-1)

    def findpath(self):
        if len(self.cur_bush)==1:
            self.confused = False
        else:
            if self.x%2==1:
                self.next_x = self.x - 1
            elif(self.y==0):
                self.next_y = self.y + 1
            else:
                self.next_y = self.y - 1

    def wait(self):
        if not self.targeted:
            picker =[A for A in self.model.get_agents_of_type(Picker) if A.state == FREE and A.tx==A.ty ==0]
            if len(picker):
                self.targeted = True
                picker[0].tx, picker[0].ty = self.x,self.y
        cur_bush = [A for A in self.model.get_agents_of_type(Bush) if A.x == self.x and A.y == self.y and A.state == GREN]
        if len(cur_bush) ==1:
            self.state = FREE
            self.targeted = False

    def move(self):
        if self.confused:
            self.findpath()
        elif len([A for A in self.model.get_agents_of_type(Scout) if self.x == A.x and self.y == A.y and A.state ==BUSY])==0 and len([A for A in self.model.get_agents_of_type(Bush) if A.x == self.x and A.y == self.y and A.state == RIPE])==1:
            self.state = BUSY
            picker =[A for A in self.model.get_agents_of_type(Picker) if A.state == FREE and A.tx ==0 and A.ty==0]
            if len(picker):
                self.targeted = True
                picker[0].tx, picker[0].ty = self.x,self.y
        else:
            if self.x + self.x_dir not in range(0,self.NUMBER_OF_CELLS):
                self.leftright = not self.leftright
            if self.y + self.y_dir not in range(0,self.NUMBER_OF_CELLS):
                self.topbot = not self.topbot
            cur_path = [A for A in self.model.get_agents_of_type(Path) if A.x == self.x and A.y==self.y]
            if len(cur_path)==1:
                self.next_x += 2*self.x_dir
            else:
                self.next_y+=self.y_dir     
   
    def advance(self):
        self.x = self.next_x
        self.y = self.next_y
        self.model.grid.move_agent(self,[self.x,self.y])
        self.life -=1
        self.model.energy_use+=1



class Picker(mesa.Agent):
    
    def __init__(self, pos, model, init_state=0):

        super().__init__(id, model)
        self.NUMBER_OF_CELLS = self.model.grid.width
        self.state = init_state
        self.x, self.y = pos
        self.next_x = self.x
        self.next_y = self.y
        self.life = self.NUMBER_OF_CELLS*self.NUMBER_OF_CELLS*2
        self.tx = 0
        self.ty = 0
        self.ltx = 0
        self.lty = 0
        self.path = []
        self.wt = False
        self.storage = 0

    
    def moves(self, x, y):
        return [(A.x,A.y) for A in self.model.grid.iter_neighbors((x, y), False) if isinstance(A,Path)]
    
    def bushes(self,x,y):
        return [(A.x,A.y) for A in self.model.grid.iter_neighbors((x, y),False) if isinstance(A,Bush)]

    def step(self):
        if self.life<1:
            self.model.grid.remove_agent(self)
            self.remove()
        if self.wt:
            self.wt = False
        elif self.state != RTRN and (self.storage == 10 or self.life<max(self.x,self.tx)*self.NUMBER_OF_CELLS):
            self.rtrn()
        elif self.state == FREE:
            if self.tx!=0 or self.ty!=0:
                self.state = BUSY
                self.path = self.find_path()
            else:
                self.move()
                if len([A for A in self.model.get_agents_of_type(Path) if A.x == self.next_x and A.y==self.next_y and A.state == RIVR]):
                    self.wt = True
        else:
            self.move()
            if len([A for A in self.model.get_agents_of_type(Path) if A.x == self.next_x and A.y==self.next_y and A.state == RIVR]):
                self.wt = True

    def rtrn(self):
        self.state = RTRN
        if self.tx!=0 or self.ty!=0:
            self.ltx,self.lty = self.tx,self.ty
            self.ty = 0
            self.tx = 0
        self.path = self.find_path()
        
    def move(self):
        if len(self.path)==0:
            if self.ty ==self.tx == 0:
                if self.x == self.y ==0:
                    self.maintain()
                else:
                    self.next_x, self.next_y = (0,0)
            else:
                self.pick()
        else:
            self.next_x,self.next_y = self.path[0]
            self.path.pop(0)

    def maintain(self):
        self.model.score += self.storage
        self.storage=0
        if(self.life < self.NUMBER_OF_CELLS*self.NUMBER_OF_CELLS*2):
            self.life += self.NUMBER_OF_CELLS*2+2
        elif self.state == RTRN:
            self.state = FREE
            self.tx,self.ty = self.ltx,self.lty

    def find_path(self):
        if (self.tx,self.ty) in self.bushes(self.x,self.y) or (self.tx,self.ty) in self.moves(self.x,self.y):
            return []
        temp_path = [self.moves(self.x,self.y)[0]]
        last_tile = (self.x,self.y)
        newx , newy = temp_path[len(temp_path)-1]
        while True:
            if (self.tx,self.ty)in self.bushes(newx,newy) or (self.tx,self.ty) in self.moves(newx,newy):
                return temp_path
            else:
                move = [A for A in self.moves(x=newx,y=newy) if A!=last_tile]
                if len(move)==0:
                    break
                else:
                    temp_path.append(move[0])
                    last_tile = (newx,newy)
                    newx , newy = temp_path[len(temp_path)-1]
        temp_path = [self.moves(self.x,self.y)[1]]
        last_tile = (self.x,self.y)
        newx , newy = temp_path[len(temp_path)-1]
        while not (self.tx,self.ty)in self.bushes(newx,newy) and not (self.tx,self.ty) in self.moves(newx,newy):
            move = [A for A in self.moves(x=newx,y=newy) if A!=last_tile]
            temp_path.append(move[0])
            last_tile = (newx,newy)
            newx , newy = temp_path[len(temp_path)-1]
        return temp_path    
    

    def pick(self):
        berry = [A for A in self.model.get_agents_of_type(Bush) if A.x == self.tx and A.y == self.ty]
        berry[0].timer = randint(300,500)
        berry[0].state = GREN
        self.storage+=1
        self.ty, self.tx = (0 ,0)
        self.path = self.find_path()
        self.state = FREE

    
    def advance(self):
        
        self.x = self.next_x
        self.y = self.next_y
        self.model.grid.move_agent(self,[self.x,self.y])
        self.life-=1
        self.model.energy_use+=1

class Bush(mesa.Agent):
    
    def __init__(self, pos, model, init_state=0):

        super().__init__(id, model)
        self.timer = randint(50,300)
        self.state = init_state
        self.x, self.y = pos
        self.next_x = self.x
        self.next_y = self.y

    def step(self):
        if self.state == GREN:
            self.grow()
        else:
            self.wait()
        
    def wait(self):
        pass

    def grow(self):
        self.timer-=randint(0,3)

    def advance(self):
        if self.timer<0:
            self.state = RIPE
        

class Path(mesa.Agent):
    
    def __init__(self, pos, model, init_state=1):

        super().__init__(id, model)
        self.state = init_state
        self.x, self.y = pos

    
    def advance(self):
        
        pass
