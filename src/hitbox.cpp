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
#include <iostream>
EntityPosition Hitbox::collideWithBlock(Hitbox * other, WorldPos x, WorldPos y, WorldPos z) {
	if (other->type != TYPE_BLOCK) return noCollision;
	if (this->type != TYPE_ENTITY) return noCollision;

	EntityPosition positionThis = this->offset;
	positionThis += this->parent.ent->pos;
	EntityPosition sizeThis = this->size;

	EntityPosition positionOther = other->offset;
	positionOther += {(double)(x), (double)(y), (double)(z)};
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


	// EntityPosition distancesPos = (positionOther - sizeOther) - (positionThis + sizeThis);
	EntityPosition distancesNeg = (positionOther + sizeOther) - (positionThis - sizeThis);

	EntityPosition vel = this->parent.ent->vel;
	EntityPosition distancesToEdges;

	// distancesToEdges.x = vel.x > 0 ? distancesPos.x : distancesNeg.x;
	// distancesToEdges.y = vel.y > 0 ? distancesPos.y : distancesNeg.y;
	// distancesToEdges.z = vel.z > 0 ? distancesPos.z : distancesNeg.z;

	distancesToEdges = distancesNeg;
	distancesToEdges*=-1;

	// double distanceToEdge = std::min(distancesToEdges.x, std::min(distancesToEdges.y, distancesToEdges.z));
	double distanceToEdge = distancesToEdges.z;


	if (distanceToEdge >= 0) return noCollision;

	if (distancesToEdges.x >= 0) return noCollision;
	if (distancesToEdges.y >= 0) return noCollision;
	if (distancesToEdges.z >= 0) return noCollision;

	std::cout << distancesToEdges.x << ", " << distancesToEdges.y << ", " << distancesToEdges.z << "\n";

	EntityPosition pushDistance = normalize(vel);
	pushDistance*=distanceToEdge;

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
