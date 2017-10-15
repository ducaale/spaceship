/*
 * https://github.com/SuperV1234/Tutorials/blob/master/DiveIntoC%2B%2B11/5_Entities/p8.cpp
 */

#include "entity.h"
#include "manager.h"

void Entity::addGroup(Group group) {
    if(!hasGroup(group)) {
        groupBitset[group] = true;
        manager.addToWaitingGroup(this, group);
    }
}

