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
    "forward": game.Key(ord('W')),
    "left": game.Key(ord('A')),
    "backward": game.Key(ord('S')),
    "right": game.Key(ord('D'))
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
    playerLookX, playerLookY = player.getLookVectorXY()

    forwardMoveIntent = keys["forward"].get() - keys["backward"].get()
    sidewaysMoveIntent = keys["left"].get() - keys["right"].get()

    moveIntentLength = (forwardMoveIntent**2 + sidewaysMoveIntent**2)**(1/2)
    if (moveIntentLength > 1):
        forwardMoveIntent = forwardMoveIntent / moveIntentLength
        sidewaysMoveIntent = sidewaysMoveIntent / moveIntentLength
        moveIntentLength = 1


    playerMoveX = playerLookX * forwardMoveIntent - playerLookY * sidewaysMoveIntent
    playerMoveY = playerLookY * forwardMoveIntent + playerLookX * sidewaysMoveIntent

    playerSpeed = 0.1
    playerMoveX = playerMoveX * playerSpeed
    playerMoveY = playerMoveY * playerSpeed

    gravity = -0.01

    player.move(playerMoveX, playerMoveY, 0.0)
    player.applyForce(0.0, 0.0, gravity)

game.setOnTickCallback(onTick)

def onPlayerPositionChanged():
    ...

game.setOnPlayerPositionChangedCallback(onPlayerPositionChanged)