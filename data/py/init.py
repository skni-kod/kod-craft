import game

game.defineBlock("air",
    solid = False
    )

game.defineBlock("stone",
    solid = True
    )

def generateEmptyChunk(x, y, z, size):
    return ((["air"]*size)*size)*size # size³ 3D array of "air"

game.defineDimension("kodland", generateEmptyChunk)
