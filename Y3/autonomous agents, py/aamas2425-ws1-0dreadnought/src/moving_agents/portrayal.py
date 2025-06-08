def portrayCell(cell):
    if cell is None:
        raise AssertionError
    return {
        "Shape": "rect",
        "w": 1,
        "h": 1,
        "Filled": "true",
        "Layer": 0,
        "x": cell.x,
        "y": cell.y,
        "Color": "red" if cell.isVert else "blue" if cell.isHoriz else "white",
    }
  #TODO complete
