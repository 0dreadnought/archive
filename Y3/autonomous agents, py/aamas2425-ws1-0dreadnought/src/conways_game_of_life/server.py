import mesa

from .portrayal import portrayCell
from .model import ConwaysGameOfLife

canvas_element = mesa.visualization.CanvasGrid(portrayCell, 50, 50, 250, 250)

server = mesa.visualization.ModularServer(
    ConwaysGameOfLife, [canvas_element], "Game of Life", {"height": 50, "width": 50}
)

#TODO complete