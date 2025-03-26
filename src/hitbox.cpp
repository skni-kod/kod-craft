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

EntityPosition Hitbox::collideWithTerrain(EntityPosition position, EntityPosition velocity) {
	assert(this->type == TYPE_ENTITY);

	EntityPosition minPos = this->getWorldMinimum();
    EntityPosition maxPos = this->getWorldMaximum();

    minPos-=maxHitboxSize;
    maxPos+=maxHitboxSize;

    BlockInstance block = BlockInstance(this->parent.ent->dimension, minPos.x, minPos.y, minPos.z);

    for (WorldPos x = minPos.x; x < maxPos.x; x++) {
        BlockInstance blockX = block;
        for (WorldPos y = minPos.y; y < maxPos.y; y++) {
            BlockInstance blockY = blockX;
            for (WorldPos z = minPos.z; z < maxPos.z; z++) {
                EntityPosition delta = blockY.get().checkCollision(position, velocity, this, blockY.getX(), blockY.getY(), blockY.getZ());
                if (delta != noCollision) {
                    return delta;
                }
                blockY = blockY.getInstanceAt(Zpos);
            }
            blockX = blockX.getInstanceAt(Ypos);
        }
        block = block.getInstanceAt(Xpos);
    }

    return noCollision;
}

EntityPosition Hitbox::collideWithBlock(EntityPosition position, EntityPosition velocity, Hitbox * other, WorldPos x, WorldPos y, WorldPos z) {
	if (other->type != TYPE_BLOCK) return noCollision;
	if (this->type != TYPE_ENTITY) return noCollision;

	EntityPosition positionThis = this->offset;
	positionThis += position;
	EntityPosition sizeThis = this->size;

	EntityPosition positionOther = other->offset;
	positionOther += {(double)(x), (double)(y), (double)(z)};
	// positionOther += {0.5, 0.5, 0.5}; // static block offset - remove/change if necessary
	EntityPosition sizeOther = other->size;



	if (positionOther.x>positionThis.x) {
		std::swap(positionOther.x, positionThis.x);
		std::swap(sizeThis.x, sizeOther.x);
	}
	if (positionOther.y>positionThis.y) {
		std::swap(positionOther.y, positionThis.y);
		std::swap(sizeThis.y, sizeOther.y);
	}
	if (positionOther.z>positionThis.z) {
		std::swap(positionOther.z, positionThis.z);
		std::swap(sizeThis.z, sizeOther.z);
	}

	EntityPosition distancesNeg = (positionOther + sizeOther) - (positionThis - sizeThis);

	EntityPosition vel = velocity;
	EntityPosition distancesToEdges;


	distancesToEdges = distancesNeg;
	distancesToEdges*=-1;

	double distanceToEdge = std::max(distancesToEdges.x, std::max(distancesToEdges.y, distancesToEdges.z));

	if (distancesToEdges.x >= 0) return noCollision;
	if (distancesToEdges.y >= 0) return noCollision;
	if (distancesToEdges.z >= 0) return noCollision;

	// if (distanceToEdge>-0.01) return noCollision; // short distances may cause infnite loops thx to floating point


	EntityPosition pushDistance = normalize(vel);

	double maxAxisLength = std::max(std::abs(pushDistance.x), std::max(std::abs(pushDistance.y), std::abs(pushDistance.z)));

	pushDistance*=distanceToEdge/maxAxisLength;

	return pushDistance;
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
