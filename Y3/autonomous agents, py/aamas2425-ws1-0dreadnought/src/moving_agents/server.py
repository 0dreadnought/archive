import mesa

from .portrayal import portrayCell
from .model import ConwaysGameOfLife

canvas_element = mesa.visualization.CanvasGrid(portrayCell, 30, 30, 500, 500)

server = mesa.visualization.ModularServer(
    ConwaysGameOfLife, [canvas_element], "Game of Life", {"height": 30, "width": 30}
)