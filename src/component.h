#ifndef COMPONENT_H_PGUVQNWZ
#define COMPONENT_H_PGUVQNWZ

#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>
#include <cassert>
#include <type_traits>

struct Component;
class Entity;

using ComponentID = std::size_t;

ComponentID getUniqueComponentID() {
    static ComponentID lastID = 0u;
    return lastID++;
}

template <typename T>
ComponentID getComponentTypeID() {
    static_assert(std::is_base_of<Component, T>::value,
            "T must inherit from Component");

    static ComponentID typeID = getUniqueComponentID();
    return typeID;
}


struct Component {
    Entity* entity;

    virtual void init() {}

    virtual void update(float elapsedTime) {}
    virtual void draw() {}

    virtual ~Component() {};
};

#endif /* end of include guard: COMPONENT_H_PGUVQNWZ */
