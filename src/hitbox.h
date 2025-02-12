#ifndef HITBOX_H
#define HITBOX_H

class Hitbox;

#include "entity.h"

class Hitbox {
	EntityPosition offset;
	EntityPosition size;
public:
	Hitbox(EntityPosition offset, EntityPosition size);
};

#endif