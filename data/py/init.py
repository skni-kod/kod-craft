import game

air = game.Block("air",
    solid = False,
    visible = False
    )

stone = game.Block("stone",
    texture = "stone.png",
    solid = True
    )

def generateEmptyChunk(x, y, z, size):
    return [[["air"]*size]*size]*size # size³ 3D array of "air"

def generateFlatLand(x, y, z, size):
    if z<0:
        return [[[stone]*size]*size]*size

    return [[[air]*size]*size]*size

kodland = game.Dimension("kodland", generateFlatLand, chunkSize=8)


def onWorldLoad():
    global player
    player = game.getPlayer()
    player.setDimension(kodland)
    game.setPlayerCameraOffset(0, 0, 1.75)

game.setOnWorldLoadCallback(onWorldLoad)


def onPlayerPositionChanged():
    player.move(-0.1, 0.1, 0.1)
    player.applyForce(0.0, 0.001, 0.0)

game.setOnPlayerPositionChangedCallback(onPlayerPositionChanged)