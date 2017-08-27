#include "entity.h"
#include "manager.h"

void Entity::addGroup(Group group) {
    groupBitset[group] = true;
    manager.addToWaitingGroup(this, group);
}

