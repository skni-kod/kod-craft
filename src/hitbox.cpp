#include "hitbox.h"

Hitbox::Hitbox(void * parent, HitboxParentType type, EntityPosition offset, EntityPosition size) {
	this->parent.vid = parent;
	this->type = type;

	this->offset = offset;
	this->size = size;
	this->size *= 0.5;

	switch (type) {
	case TYPE_ENTITY:
		this->parent.ent->hitboxes.push_back(this);
		break;
	case TYPE_BLOCK:
		this->parent.blk->hitboxes.push_back(this);
		break;
	}
}

double Hitbox::collideWithTerrain(EntityPosition A, EntityPosition B) {
	assert(this->type == TYPE_ENTITY);

	EntityPosition minPos = this->getWorldMinimum();
    EntityPosition maxPos = this->getWorldMaximum();

    minPos-=maxHitboxSize;
    maxPos+=maxHitboxSize;

    BlockInstance block = BlockInstance(this->parent.ent->dimension, minPos.x, minPos.y, minPos.z);

    double collisionSum = 0;

    for (WorldPos x = minPos.x; x < maxPos.x; x++) {
        BlockInstance blockX = block;
        for (WorldPos y = minPos.y; y < maxPos.y; y++) {
            BlockInstance blockY = blockX;
            for (WorldPos z = minPos.z; z < maxPos.z; z++) {
                double delta = blockY.get().checkCollision(A, B-A, this, blockY.getX(), blockY.getY(), blockY.getZ());
                if (delta != 0) {
                    collisionSum = std::max(collisionSum, delta);
                }
                blockY = blockY.getInstanceAt(Zpos);
            }
            blockX = blockX.getInstanceAt(Ypos);
        }
        block = block.getInstanceAt(Xpos);
    }

    return collisionSum;
}

bool pointBetweenPoints(double A, double B, double C) {
	if (A < B && B < C) return true;
	if (A > B && B > C) return true;
	return false;
}

bool pointInsideCube(EntityPosition point, EntityPosition cubePos, EntityPosition cubeSize) {
	if (pointBetweenPoints(cubePos.x-cubeSize.x, point.x, cubePos.x+cubeSize.x)==false) return false;
	if (pointBetweenPoints(cubePos.y-cubeSize.y, point.y, cubePos.y+cubeSize.y)==false) return false;
	if (pointBetweenPoints(cubePos.z-cubeSize.z, point.z, cubePos.z+cubeSize.z)==false) return false;
	return true;
}

double Hitbox::collideWithBlock(EntityPosition position, EntityPosition velocity, Hitbox * other, WorldPos x, WorldPos y, WorldPos z) {
	if (other->type != TYPE_BLOCK) return 0;
	if (this->type != TYPE_ENTITY) return 0;

	EntityPosition positionThis = this->offset;
	positionThis += position;
	EntityPosition sizeThis = this->size;

	EntityPosition positionOther = other->offset;
	positionOther += {(double)(x), (double)(y), (double)(z)};
	EntityPosition sizeOther = other->size;

	EntityPosition sizeSum = sizeThis + sizeOther;
	EntityPosition minBound = positionOther - sizeSum;
	EntityPosition maxBound = positionOther + sizeSum;

	if (velocity.x>0) {
		std::swap(minBound.x, maxBound.x);
	}
	if (velocity.y>0) {
		std::swap(minBound.y, maxBound.y);
	}
	if (velocity.z>0) {
		std::swap(minBound.z, maxBound.z);
	}

	EntityPosition A = positionThis;
	EntityPosition B = A+velocity;

	if (pointInsideCube(A, positionOther, sizeSum) == true || pointInsideCube(B, positionOther, sizeSum) == false) return 0; 

	double intersectionPoint = 0;

	if (pointBetweenPoints(A.x, maxBound.x, B.x)) {
		double thisPoint = 1-(maxBound.x - B.x)/velocity.x;
		EntityPosition thisPoint3D = velocity*thisPoint+A;

		if (pointBetweenPoints(minBound.y, thisPoint3D.y, maxBound.y) == false) goto xBoundCheckFail;
		if (pointBetweenPoints(minBound.z, thisPoint3D.z, maxBound.z) == false) goto xBoundCheckFail;

		intersectionPoint = std::max(intersectionPoint, thisPoint);
	}
	xBoundCheckFail:

	if (pointBetweenPoints(A.y, maxBound.y, B.y)) {
		double thisPoint = 1-(maxBound.y - B.y)/velocity.y;
		EntityPosition thisPoint3D = velocity*thisPoint+A;

		if (pointBetweenPoints(minBound.x, thisPoint3D.x, maxBound.x) == false) goto yBoundCheckFail;
		if (pointBetweenPoints(minBound.z, thisPoint3D.z, maxBound.z) == false) goto yBoundCheckFail;

		intersectionPoint = std::max(intersectionPoint, thisPoint);
	}
	yBoundCheckFail:

	if (pointBetweenPoints(A.z, maxBound.z, B.z)) {
		double thisPoint = 1-(maxBound.z - B.z)/velocity.z;
		EntityPosition thisPoint3D = velocity*thisPoint+A;

		if (pointBetweenPoints(minBound.x, thisPoint3D.x, maxBound.x) == false) goto zBoundCheckFail;
		if (pointBetweenPoints(minBound.y, thisPoint3D.y, maxBound.y) == false) goto zBoundCheckFail;

		intersectionPoint = std::max(intersectionPoint, thisPoint);
	}
	zBoundCheckFail:


	if (intersectionPoint>1) return 1;
	if (intersectionPoint<0) return 0;

	return intersectionPoint;
}

EntityPosition Hitbox::getWorldCenter() {
	// it's impossible to get world position from block template alone -> hitbox must be attached to an entity
	assert(this->type == TYPE_ENTITY);
	return this->offset + this->parent.ent->pos;
}

EntityPosition Hitbox::getWorldMinimum() {
	return this->getWorldCenter() - this->size;
}

EntityPosition Hitbox::getWorldMaximum() {
	return this->getWorldCenter() + this->size;
}
