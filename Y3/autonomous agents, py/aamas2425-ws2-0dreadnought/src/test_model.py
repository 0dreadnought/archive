import unittest


from warehouse.model import *
class TestModel(unittest.TestCase):

    def test_model_creation(self):
        wh = Warehouse(n_robots=1, n_boxes=5)
        self.assertIsNotNone(wh)
        self.assertEqual(len(wh.get_agents_of_type(PickerRobot)), 1)
        self.assertEqual(len(wh.get_agents_of_type(Box)), 5)
    
    def test_robot_make_decision(self):
         # TODO implement
         pass
        
        

if __name__ == '__main__':
    unittest.main()