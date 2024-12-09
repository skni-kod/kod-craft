import game

game.defineBlock("air",
    solid = False
    )

game.defineBlock("stone",
    solid = True
    )

def generateEmptyChunk(x, y, z, size):
    return 0

game.defineDimension("kodland", generateEmptyChunk)
