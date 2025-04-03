#ifndef HITBOX_H
#define HITBOX_H

const double maxHitboxSize = 2;

class Hitbox;

#include "entity.h"
#include "block.h"

const EntityPosition noCollision = {0, 0, 0};

enum HitboxParentType {
	TYPE_ENTITY,
	TYPE_BLOCK
};

union HitboxParent {
	void * vid;
	BlockTemplate * blk;
	Entity * ent;
};

class Hitbox {
	EntityPosition offset;
	EntityPosition size;	// distance to edge

	HitboxParentType type;
	HitboxParent parent;
public:
	Hitbox(void * parent, HitboxParentType type, EntityPosition offset, EntityPosition size);

	double collideWithBlock(EntityPosition position, EntityPosition velocity, Hitbox * other, WorldPos x, WorldPos y, WorldPos z);
	double collideWithTerrain(EntityPosition A, EntityPosition B);

	EntityPosition getWorldCenter();	// center of the hitbox in world position
	EntityPosition getWorldMinimum();	// smallest coordinate within the hitbox
	EntityPosition getWorldMaximum();	// biggest coordinate within the hitbox
};

#endif