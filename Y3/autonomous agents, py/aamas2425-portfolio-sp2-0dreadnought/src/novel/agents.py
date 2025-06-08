import mesa
from random import randint

import mesa.agent

FREE = 0
BUSY = 1
RTRN = 2
CHRG = 3
SLEP = 4

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
        self.max_life = self.life

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
        bush = [A for A in self.model.get_agents_of_type(Bush) if A.x == self.x and A.y == self.y]
        return bush[0] if len(bush)>0 else None
        

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
        if self.life < self.max_life:
            self.life += self.NUMBER_OF_CELLS*2+2
        else:
            self.life = self.max_life
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
        if self.cur_bush is not None:
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
            self.find_pk()
        if self.cur_bush.state == GREN:
            self.state = FREE
            self.targeted = False
    
    def sortkey(self,l):
        return abs(self.x-l.x)+abs(l.y-self.y)
    
    def find_pk(self):
        picker =[A for A in self.model.get_agents_of_type(Picker) if A.state == SLEP and A.tx ==-1]
        if len(picker):
            self.targeted = True
            picker.sort(key=self.sortkey)
            picker[0].tx, picker[0].ty = self.x,self.y
            picker[0].bush = self.cur_bush

    def move(self):
        if self.confused:
            self.findpath()
        elif len([A for A in self.model.get_agents_of_type(Scout) if self.x == A.x and self.y == A.y and A.state ==BUSY])==0 and len([A for A in self.model.get_agents_of_type(Bush) if A.x == self.x and A.y == self.y and A.state == RIPE])==1:
            self.state = BUSY
            self.find_pk()
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
        self.state = SLEP
        self.last_state = SLEP
        self.x, self.y = pos
        self.next_x = self.x
        self.next_y = self.y
        self.life = self.NUMBER_OF_CELLS*self.NUMBER_OF_CELLS*2
        self.max_life = self.life
        self.tx = -1
        self.ty = -1
        self.bush = None

        self.targeted = False

        self.path = []
        self.path_cost = 0
        self.wt = False

        self.storage = 0

    
    def moves(self, x, y):
        return [((A.x,A.y),A.state) for A in self.model.grid.iter_neighbors((x, y), False) if isinstance(A,Path)]
    
    def bushes(self,x,y):
        return [(A.x,A.y) for A in self.model.grid.iter_neighbors((x, y),False) if isinstance(A,Bush)]

    def step(self):
        if self.life<1:
            self.model.grid.remove_agent(self)
            self.remove()
        if self.wt:
            self.wt = False
        else:
            self.optimize_work()
            if self.storage == 10 or self.life<10 or self.state == CHRG:
                self.find_mt()
            elif self.state == SLEP:
                self.sleep()
            else:
                self.move()

    def optimize_work(self):
        for A in self.model.get_agents_of_type(Picker):
            if A.state != CHRG and not A.targeted:
                apath,acost = A.find_path(self.tx,self.ty)
                spath, scost = self.find_path(A.tx,A.ty)
                if acost<=self.path_cost and scost<=A.path_cost and not(acost==self.path_cost and scost==A.path_cost):
                    A.state, self.state = self.state, A.state
                    A.tx, self.tx = self.tx,A.tx
                    self.ty, A.ty = A.ty, self.ty
                    self.path = spath
                    self.path_cost = scost
                    A.path = apath
                    A.path_cost = acost
                    A.bush, self.bush = self.bush, A.bush

    def sleep(self):
        if self.tx!=-1:
            self.path, self.path_cost = self.find_path(self.tx,self.ty)
            self.state = BUSY
        
    
    def find_mt(self):
        if self.state!= CHRG:
            self.last_state = self.state
        self.state = CHRG
        if not self.targeted:
            mt = [A for A in self.model.get_agents_of_type(Maintainer) if A.state == FREE and A.tbot is None]
            if len(mt)>0:
                mt[0].tbot = self
                self.targeted = True
        if self.life==self.max_life:
            self.state = self.last_state
            self.targeted = False
        
    def move(self):
        if len(self.path)==0:
            self.pick()
        else:
            self.next_x,self.next_y = self.path[0][0]
            if self.path[0][1]==RIVR:
                self.wt = True
                self.path_cost-=RIVR
            else:
                self.path_cost-=GRND
            self.path.pop(0)

    def find_path(self, x, y):
        if (x,y) in self.bushes(self.x,self.y) or x==y==-1:
            return [],0
        temp_path_1 = [self.moves(self.x,self.y)[0]]
        path_cost_1 = temp_path_1[0][1]
        last_tile = (self.x,self.y)
        newx , newy = temp_path_1[0][0]
        while (x,y) not in self.bushes(newx,newy):
            move = [A for A in self.moves(x=newx,y=newy) if A[0]!=last_tile]
            if len(move) ==0:
                path_cost_1 = 999
                break
            temp_path_1.append(move[0])
            last_tile = (newx,newy)
            path_cost_1 += temp_path_1[len(temp_path_1)-1][1]
            newx , newy = temp_path_1[len(temp_path_1)-1][0]
        if len(self.moves(self.x,self.y))==1:
            return temp_path_1, path_cost_1
        
        temp_path_2 = [self.moves(self.x,self.y)[1]]
        last_tile = (self.x,self.y)
        path_cost_2 = temp_path_2[0][1]
        newx , newy = temp_path_2[0][0]
        while (x,y) not in self.bushes(newx,newy):
            move = [A for A in self.moves(x=newx,y=newy) if A[0]!=last_tile]
            if len(move)==0:
                return temp_path_1, path_cost_1
            temp_path_2.append(move[0])
            path_cost_2 += temp_path_2[len(temp_path_2)-1][1]
            last_tile = (newx,newy)
            newx , newy = temp_path_2[len(temp_path_2)-1][0]
        return (temp_path_2,path_cost_2) if path_cost_1>path_cost_2 else (temp_path_1,path_cost_1)
    

    def pick(self):
        self.bush.timer = randint(300,500)
        self.bush.state = GREN
        self.storage+=1
        self.ty, self.tx = (-1,-1)
        self.state = SLEP

    
    def advance(self):
        
        self.x = self.next_x
        self.y = self.next_y
        self.model.grid.move_agent(self,[self.x,self.y])
        if self.state not in [SLEP,CHRG]:
            self.life-=1
            self.model.energy_use+=1

class Bush(mesa.Agent):
    
    def __init__(self, pos, model, init_state=0):

        super().__init__(id, model)
        self.timer = randint(50,300)
        self.state = init_state
        self.x, self.y = pos

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

class Maintainer(mesa.Agent):

    def __init__(self, pos, model, init_state=0):
        super().__init__(id, model)
        self.NUMBER_OF_CELLS = self.model.grid.width
        self.state = init_state
        self.x, self.y = pos

        self.life = self.NUMBER_OF_CELLS*2*self.NUMBER_OF_CELLS
        self.max_life = self.life
        self.bank = self.NUMBER_OF_CELLS*10*self.NUMBER_OF_CELLS
        self.max_bank = self.bank
        self.storage = 0

        self.next_x = self.x
        self.next_y = self.y

        self.tbot = None

    def step(self):

        if self.life < self.NUMBER_OF_CELLS*2 or self.bank <self.NUMBER_OF_CELLS*25 or self.state == RTRN:

            self.force_return()
        elif self.state == BUSY:
            self.move()
        else:
            self.rtrn()
    
    def force_return(self):
        self.state = RTRN
        if self.x>0:
            self.next_x = self.x - 1
        elif self.y>0:
            self.next_y = self.y - 1
        else:
            self.charge()

    def rtrn(self):
        if self.tbot is not None:
            self.state = BUSY
        else:
            if self.x>0:
                self.next_x = self.x - 1
            elif self.y>0:
                self.next_y = self.y - 1
            else:
                self.charge()


    def charge(self):
        self.model.score += self.storage
        self.storage = 0
        if self.life < self.max_life or self.bank<self.max_bank:
            self.life += self.NUMBER_OF_CELLS*2
            self.bank += self.NUMBER_OF_CELLS*5
        else:
            self.life = self.max_life
            self.bank = self.max_bank
            self.state = FREE

    def maintain(self):
        self.storage += self.tbot.storage
        self.tbot.storage = 0
        self.tbot.life += self.NUMBER_OF_CELLS*5
        self.bank -= self.NUMBER_OF_CELLS*5
        if self.tbot.life>=self.tbot.max_life:
            self.tbot.life = self.tbot.max_life
            self.state = FREE
            self.tbot = None

    def move(self):
        if self.x!=self.tbot.x:
            self.next_x = self.x + (1 if self.tbot.x>self.x else -1)
        elif self.y!=self.tbot.y:
            self.next_y = self.y + (1 if self.tbot.y>self.y else -1)
        else:
            self.maintain()
        

    def advance(self):
        
        self.x = self.next_x
        self.y = self.next_y
        self.model.grid.move_agent(self,[self.x,self.y])
        if not self.x==self.y==0:
            self.life-=1
            self.model.energy_use+=1
