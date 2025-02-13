import game

game.defineBlock("air",
    solid = False,
    visible = False
    )

game.defineBlock("stone",
    texture = "stone.png",
    solid = True
    )

def generateEmptyChunk(x, y, z, size):
    return [[["air"]*size]*size]*size # size³ 3D array of "air"

def generateFlatLand(x, y, z, size):
    if z<0:
        return [[["stone"]*size]*size]*size

    return [[["air"]*size]*size]*size

game.defineDimension("kodland", generateFlatLand, chunkSize=8)


def onWorldLoad():
    game.setPlayerDimension("kodland")
    game.setPlayerCameraOffset(0, 0, 1.75)

game.setOnWorldLoadCallback(onWorldLoad)


def onPlayerPositionChanged():
    game.movePlayer(-0.1, 0.1, 0.1)
    game.playerApplyForce(0.0, 0.001, 0.0)

game.setOnPlayerPositionChangedCallback(onPlayerPositionChanged)