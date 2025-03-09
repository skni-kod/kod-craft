#ifndef HITBOX_H
#define HITBOX_H

const double maxHitboxSize = 2;

class Hitbox;

#include "entity.h"
#include "block.h"

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

	EntityPosition collideWithBlock(Hitbox * other, WorldPos x, WorldPos y, WorldPos z);

	EntityPosition getWorldCenter();	// center of the hitbox in world position
	EntityPosition getWorldMinimum();	// smallest coordinate within the hitbox
	EntityPosition getWorldMaximum();	// biggest coordinate within the hitbox
};

#endif