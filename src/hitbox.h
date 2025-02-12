#ifndef HITBOX_H
#define HITBOX_H

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
	EntityPosition size;

	HitboxParentType type;
	HitboxParent parent;
public:
	Hitbox(void * parent, HitboxParentType type, EntityPosition offset, EntityPosition size);
};

#endif