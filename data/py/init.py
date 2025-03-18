import game

air = game.Block("air",
    solid = False,
    visible = False
    )

stone = game.Block("stone",
    texture = "stone.png",
    solid = True
    )


keys = {
    "forward": game.Key(ord('w'))
};


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
    player.addHitbox(0,0,1, 1,1,2)
    player.setPosition(0.1, 0.2, 10)

game.setOnWorldLoadCallback(onWorldLoad)

def onTick():
    player.applyForce(0.0, 0.0, -0.01)

game.setOnTickCallback(onTick)

def onPlayerPositionChanged():
    ...

game.setOnPlayerPositionChangedCallback(onPlayerPositionChanged)