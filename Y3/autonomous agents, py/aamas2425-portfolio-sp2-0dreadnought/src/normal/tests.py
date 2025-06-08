import unittest
import os
import sys
sys.path.append(os.path.abspath('..'))
from normal.model import Farm
from normal.agents import Scout, Picker, Bush, Path, BUSY, FREE, GREN, RIPE, RTRN, CHRG, RIVR, GRND

field = Farm(nscouts = 2, npickers = 2, width = 9)
for A in field.get_agents_of_type(Path):
    A.state = GRND
scout = field.get_agents_of_type(Scout)[0]
picker = field.get_agents_of_type(Picker)[0]

class TestScout(unittest.TestCase):

    def test_advance(self):
        battery = scout.life
        x,y = scout.x, scout.y
        scout.next_x,scout.next_y = (1,1)
        scout.advance()
        self.assertNotEqual((x,y),(scout.x,scout.y),"Scout is not moving | A")
        self.assertLess(scout.life,battery, "Battery is not draining | A")

    
    def test_findpath(self):
        scout.next_x,scout.next_y = (1,1)
        scout.advance()
        scout.findpath()
        scout.advance()
        scout.findpath()
        scout.advance()
        self.assertFalse(scout.confused,"Scout can't orient in the field | FP")
        scout.confused = True
        scout.next_x,scout.next_y = (0,1)
        scout.advance()
        scout.findpath()
        self.assertFalse(scout.confused,"Scout can't orient when over bush | FP")
        scout.advance()
        

    def test_move(self):
        scout.next_x,scout.next_y = (0,1)
        scout.advance()
        scout.confused = True
        scout.move()
        self.assertFalse(scout.confused,"findpath() is not called when scout is confused | M")
        scout.move()
        self.assertEqual(scout.state,BUSY,"Scout can't detect ripe bush | M")

        scout.state = FREE
        for A in field.get_agents_of_type(Bush):
            A.state = GREN

        scout.next_x,scout.next_y = (0,8)
        scout.topbot, scout.leftright = True,False
        scout.advance()
        scout.move()
        scout.advance()
        self.assertTrue(scout.leftright,"x direction is not changed when facing border | M")
        self.assertFalse(scout.topbot,"y direction is not changed when facing border | M")
        self.assertEqual((scout.x,scout.y), (0,7),"Scout's moves unintended when facing corner over bush | M")
        scout.next_x,scout.next_y = (0,0)
        scout.advance()
        scout.move()
        scout.advance()
        self.assertEqual((scout.x,scout.y), (2,0),"Scout's moves unintended when facing corner over path | M")
        scout.move()
        scout.advance()
        self.assertEqual((scout.x,scout.y), (2,1),"Scout's moves unintended when over bush | M")
    
    def test_wait(self):
        for A in field.get_agents_of_type(Bush):
            A.state = RIPE
        scout.next_x,scout.next_y = (0,1)
        scout.advance()
        scout.state = BUSY
        scout.wait()
        self.assertEqual(scout.state,BUSY, "Ripe bush is not found, when should | W")
        A = [A for A in field.get_agents_of_type(Bush) if A.x==0 and A.y == 1]
        A[0].state = GREN
        scout.wait()
        self.assertEqual(scout.state,FREE, "Ripe bush is found, when shouldn't | W")  

    def test_maintain(self):
        scout.state = CHRG
        scout.life = 10
        scout.maintain()
        self.assertGreater(scout.life,10,"Scout is not charging | MT")
        scout.life = 999
        scout.maintain()
        self.assertEqual(scout.state,RTRN,"Scout's state is not changing when fully charged | MT")

    def test_chrg(self):
        scout.state = BUSY
        scout.next_x, scout.next_y = (2,1)
        scout.advance()
        scout.chrg()
        self.assertEqual(scout.last_state, BUSY,"Last state is not saved | C")
        self.assertEqual(scout.state, CHRG, "CHRG state is not asigned | C")
        self.assertEqual((scout.last_y,scout.last_x),(1,2), "Last position is not saved | C")
        self.assertEqual(scout.next_x,1,"Next position is not intended | C")
        scout.advance()
        scout.chrg()
        scout.advance()
        scout.chrg()
        scout.advance()
        scout.life = 999
        scout.chrg()
        self.assertEqual(scout.state,RTRN,"maintain() is not called | C")
        scout.advance()

    def test_return(self):
        scout.last_state = BUSY
        scout.last_x, scout.last_y = (2,1)
        scout.next_x,scout.next_y = (1,0)
        scout.advance()
        scout.rtrn()
        scout.advance()
        self.assertEqual(scout.x,2,"Unintended x move | R")
        scout.rtrn()
        scout.advance()
        self.assertEqual(scout.y,1,"Unintended y move | R")
        scout.rtrn()
        self.assertEqual(scout.state,BUSY,"Last state is not recalled | R")
        scout.last_state = FREE
        scout.rtrn()
        self.assertTrue(scout.confused,"Unintended sequence when last state is not BUSY | R")
    
    def test_step(self):
        for A in field.get_agents_of_type(Bush):
            A.state = RIPE
        scout.confused = False
        scout.next_x,scout.next_y = (2,3)
        scout.state = FREE
        scout.life = 999
        scout.advance()
        scout.step()
        self.assertEqual(scout.state,BUSY,"move() is not called when should | S")
        scout.step()
        self.assertEqual((scout.x,scout.y),(2,3),"wait() in not called when should | S")
        scout.life = 5
        scout.step()
        self.assertEqual(scout.state,CHRG,"chrg() is not called when should | S")
        scout.advance()
        scout.life = 999
        scout.last_state = FREE
        scout.state = RTRN
        scout.step()
        self.assertEqual(scout.state,FREE,"rtrn() is not called when should | S")




class TestPicker(unittest.TestCase):

    def test_advance(self):
        battery = picker.life
        picker.x,picker.y = 0,0
        picker.next_x,picker.next_y = 1,1
        picker.advance()
        self.assertEqual((picker.x,picker.y), (1,1), "Picker is not moving | A")
        self.assertLess(picker.life, battery, "Battery is not draining | A")
    
    def test_pick(self):
        for A in field.get_agents_of_type(Bush):
            A.state = RIPE
        picker.pick(2,1)
        berry = [A for A in field.get_agents_of_type(Bush) if A.x ==2 and A.y == 1]
        self.assertEqual(berry[0].state, GREN,"Berry is not picked | P")

    def test_maintain(self):
        picker.state = RTRN
        picker.life = 10
        picker.maintain()
        self.assertGreater(picker.life,10,"Picker is not charging | MT")
        picker.life = 999
        picker.maintain()
        self.assertEqual(picker.state,FREE,"State is not changed after charging | MT")
    
    def test_find_path(self):
        picker.x,picker.y = (1,3)
        path = picker.find_path()
        self.assertEqual(path,[(1,2),(1,1),(1,0)],"Unintended path found | FP")
        picker.x,picker.y = (3,7)
        path = picker.find_path()
        self.assertEqual(path,[(3,8),(2,8),(1,8),(1,7),(1,6),(1,5),(1,4),(1,3),(1,2),(1,1),(1,0)], "Unintended path found | FP")
    
    def test_rtrn(self):
        picker.state = FREE
        picker.rtrn()
        self.assertEqual(picker.state, RTRN,"State is not changed | R")

    def test_move(self):
        picker.path = []
        picker.state = RTRN
        picker.next_x, picker.next_y = 1,0
        picker.move()
        picker.advance()
        self.assertEqual((picker.x,picker.y), (0,0), "Unintended sequence when state == RTRN | M")
        picker.life = 999
        picker.move()
        self.assertEqual(picker.state, FREE, "Unintended sequence when state == RTRN | M")
        picker.state = FREE
        for A in field.get_agents_of_type(Bush):
            A.state = RIPE
        picker.next_x, picker.next_y = 3,1
        picker.storage = 0
        picker.advance()
        scout.state = BUSY
        scout.next_x,scout.next_y = (2,1)
        scout.advance()
        picker.move()
        self.assertEqual(picker.storage, 1,"pick() is not called | M" )
        scout.state = FREE
        picker.move()
        self.assertNotEqual((picker.x,picker.y), (3,3), "Picker is not moving | M")
        picker.path = [(2,8)]
        picker.move()
        picker.advance()
        self.assertEqual((picker.x,picker.y), (2,8),"Picker is not folowing its path when present | M")

    def test_step(self):
        picker.x,picker.y = (1,3)
        path = [A for A in field.get_agents_of_type(Path) if A.x ==1 and A.y ==2]
        path[0].state = RIVR
        picker.path = []
        picker.wt = True
        picker.state = FREE
        picker.storage = 10
        picker.step()  
        self.assertFalse(picker.wt, "Picker is waiting | S")
        picker.step()
        self.assertEqual(picker.state, RTRN, "rtrn() is not called when should | S")
        picker.step()
        self.assertEqual((picker.next_x,picker.next_y), (1,2), "move() is not called when should | S")
        self.assertTrue(picker.wt, "Picker is not waiting when stepping into river | S")


if __name__=="__main__": 

    unittest.main()