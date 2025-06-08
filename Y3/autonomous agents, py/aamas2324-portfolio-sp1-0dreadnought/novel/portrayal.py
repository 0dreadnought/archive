from extended.agents import Scout as es
from extended.agents import Path as ep
from extended.agents import Bush as eb

from normal.agents import Scout as ns
from normal.agents import Path as np
from normal.agents import Bush as nb

from novel.agents import Scout as nvs
from novel.agents import Path as nvp
from novel.agents import Bush as nvb
from novel.agents import Maintainer as nvm


def farm_portrayal(agent):
   
   if isinstance(agent,(es,ns,nvs)):
        return scout_portrayal(agent)
   elif isinstance(agent,(ep,np,nvp)):
        return path_portrayal(agent)
   elif isinstance(agent,(eb,nb,nvb)):
       return bush_portrayal(agent)
   elif isinstance(agent, nvm):
       return maintainer_portrayal(agent)
   else:
       return picker_portrayal(agent)       

def scout_portrayal(robot):

    if robot is None:
        raise AssertionError
    return {
        "Shape": "rect",
        "w": 1,
        "h": 1,
        "Filled": "true",
        "Layer": 1,
        "x": robot.x,
        "y": robot.y,
        "Color": "orange" if robot.state == 1 else "gray" if robot.state == 0 else "pink",
    }

def picker_portrayal(robot):

    if robot is None:
        raise AssertionError
    return {
        "Shape": "rect",
        "w": 1,
        "h": 1,
        "Filled": "true",
        "Layer": 1,
        "x": robot.x,
        "y": robot.y,
        "Color": "purple" if robot.state == 1 else "black",
    } if robot.model.mode =="Novel" else {
        "Shape": "rect",
        "w": 1,
        "h": 1,
        "Filled": "true",
        "Layer": 1,
        "x": robot.x,
        "y": robot.y,
        "Color": "purple" if robot.state == 1 else "lime" if robot.state == 2 else "yellow"

    }
    
def path_portrayal(path):

    if path is None:
        raise AssertionError
    return {
        "Shape": "rect",
        "w": 1,
        "h": 1,
        "Filled": "true",
        "Layer": 0,
        "x": path.x,
        "y": path.y,
        "Color": "blue" if path.state == 2 else "white" if path.x==path.y==0 else "brown",
    }

def bush_portrayal(bush):
    if bush is None:
        raise AssertionError
    return {
        "Shape": "rect",
        "w": 1,
        "h": 1,
        "Filled": "true",
        "Layer": 0,
        "x": bush.x,
        "y": bush.y,
        "Color": "red" if bush.state == 1 else "green",
    }

def maintainer_portrayal(robot):
    if robot is None:
        return AssertionError
    return{
        "Shape": "rect",
        "w":1,
        "h":1,
        "Filled":"true",
        "Layer": 1,
        "x": robot.x,
        "y": robot.y,
        "Color": "magenta" if robot.state == 1 else "yellow" if robot.state == 0 else "lime"
    }