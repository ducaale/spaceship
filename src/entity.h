/*
 * https://github.com/SuperV1234/Tutorials/blob/master/DiveIntoC%2B%2B11/5_Entities/p8.cpp
 */

#ifndef ENTITY_H
#define ENTITY_H

#include <bitset>
#include <array>
#include <vector>
#include <memory>

#include "component.h"
struct Manager;

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;

using Group = std::size_t;
using GroupBitset = std::bitset<maxGroups>;
using ComponentBitset = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;

class Entity {
private:
    Manager& manager;

    bool alive = true;
    int layer = 0;
    std::vector<std::unique_ptr<Component>> components;

    ComponentArray componentArray;
    ComponentBitset componentBitset;

    GroupBitset groupBitset;

public:
    Entity(Manager& manager) : manager(manager) {}

    void update(float elapsedTime) {
        for(auto& c : components) c->update(elapsedTime);
    }

    void draw() {
        for(auto& c : components) c->draw();
    }

    int getLayer() const { return layer; }
    void setLayer(int layer) { this->layer = layer; }
    bool isAlive() const { return alive; }
    void destroy() { alive = false; }

    template <typename T>
    bool hasComponent() const {
        return componentBitset[getComponentTypeID<T>()];
    }

    bool hasGroup(Group group) const {
        return groupBitset[group];
    }

    void addGroup(Group group);
    void delGroup(Group group) {
        if(hasGroup(group)) groupBitset[group] = false;
    }

    template <typename T, typename... Targs>
    T& addComponent(Targs&&... mArgs) {
        assert(!hasComponent<T>());

        T* c = new T(std::forward<Targs>(mArgs)...);
        c->entity = this;

        std::unique_ptr<Component> uPtr{c};
        components.emplace_back(std::move(uPtr));

        componentArray[getComponentTypeID<T>()] = c;
        componentBitset[getComponentTypeID<T>()] = true;

        c->init();

        return *c;
    }

    template <typename T>
    T& getComponent() const {
        assert(hasComponent<T>());

        auto ptr = componentArray[getComponentTypeID<T>()];
        return *static_cast<T*>(ptr);
    }
};
#endif /* ENTITY_H */
