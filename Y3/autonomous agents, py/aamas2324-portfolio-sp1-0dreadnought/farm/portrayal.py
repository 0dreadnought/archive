from farm.agents import Scout, Path, Bush


def farm_portrayal(agent):
   
   if isinstance(agent,Scout):
        return scout_portrayal(agent)
   elif isinstance(agent,Path):
        return path_portrayal(agent)
   elif isinstance(agent,Bush):
       return bush_portrayal(agent)
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
        "Color": "purple" if robot.state == 1 else "lime" if robot.state == 2 else "yellow",
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