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

EntityPosition Hitbox::collideWithBlock(Hitbox * other, WorldPos x, WorldPos y, WorldPos z) {
	EntityPosition noCollision = {0, 0, 0};
	if (other->type != TYPE_BLOCK) return noCollision;
	if (this->type != TYPE_ENTITY) return noCollision;

	EntityPosition positionDifference = this->offset;
	positionDifference += this->parent.ent->pos;
	positionDifference -= other->offset;
	positionDifference -= {(double)(x), (double)(y), (double)(z)};


	EntityPosition hitboxSum = this->size;
	hitboxSum += other->size;

	#define checkDistance(axis, direction) (positionDifference.axis direction hitboxSum.axis)
	#define checkDistanceAxis(axis) std::min(checkDistance(axis, -), checkDistance(axis, +))
	double distanceToEdge = checkDistanceAxis(x);
	distanceToEdge = std::min(distanceToEdge, checkDistanceAxis(y));
	distanceToEdge = std::min(distanceToEdge, checkDistanceAxis(z));

	if (distanceToEdge <= 0) return noCollision;

	EntityPosition pushDistance = normalize(this->parent.ent->vel);
	pushDistance*=-1.0/distanceToEdge;

	return pushDistance;
}
