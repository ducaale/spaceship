#ifndef ENTITY_H
#define ENTITY_H

#include <bitset>
#include <array>
#include <vector>
#include <memory>

#include "component.h"

constexpr std::size_t maxComponents = 32;

using ComponentBitset = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;

class Entity {
private:
    bool alive = true;
    std::vector<std::unique_ptr<Component>> components;

    ComponentArray componentArray;
    ComponentBitset componentBitset;

public:
    void update(float elapsedTime) {
        for(auto& c : components) c->update(elapsedTime);
    }

    void draw() {
        for(auto& c : components) c->draw();
    }

    bool isAlive() const { return alive; }
    void destroy() { alive = false; }

    template <typename T>
    bool hasComponent() const {
        return componentBitset[getComponentTypeID<T>()];
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
