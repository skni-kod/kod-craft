import game

game.defineBlock("air",
    solid = False,
    visible = False
    )

game.defineBlock("stone",
    solid = True
    )

def generateEmptyChunk(x, y, z, size):
    return [[["air"]*size]*size]*size # size³ 3D array of "air"

def generateFlatLand(x, y, z, size):
    if z<0:
        return [[["stone"]*size]*size]*size

    return [[["air"]*size]*size]*size

game.defineDimension("kodland", generateFlatLand)


def onWorldLoad():
    game.setPlayerDimension("kodland")

game.setOnWorldLoadCallback(onWorldLoad)