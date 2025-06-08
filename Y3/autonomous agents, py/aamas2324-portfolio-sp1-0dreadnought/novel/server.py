import mesa
from  novel.model import Farm
from .portrayal import farm_portrayal

SIZE_OF_CANVAS_IN_PIXELS_X = 400
SIZE_OF_CANVAS_IN_PIXELS_Y = 400

simulation_params = {
    "mode" : mesa.visualization.Choice(
        "Mode",
        "Novel",
        ["Novel", "Extended","Normal"],
        "Operation mode"
    ),
    "nscouts": mesa.visualization.Slider(
        'Number of Scouts',
        2, #default
        1, #min
        20, #max
        1, #step
        "Choose how many Scouts to include in the simulation"
    ),
    "npickers": mesa.visualization.Slider(
        'Number of Pickers',
        2, #default
        1, #min
        10, #max
        1, #step
        "Choose how many Pickers to include in the simulation"
    ),
    "nmaintainers": mesa.visualization.Slider(
        'Number of Maintainers',
        1, #default
        1, #min
        5, #max
        1, #step
        "Choose how many Maintainers to include in the simulation"
    ) 
    }

grid = mesa.visualization.CanvasGrid(farm_portrayal, 9, 9, SIZE_OF_CANVAS_IN_PIXELS_X, SIZE_OF_CANVAS_IN_PIXELS_Y)

server = mesa.visualization.ModularServer(
    Farm, [grid], "Modern Farm", simulation_params
)