#ifndef MANAGER_H
#define MANAGER_H

#include <vector>
#include <memory>
#include <algorithm>

#include "entity.h"

struct Manager {
private:
    std::vector<std::unique_ptr<Entity>> entities;
    std::array<std::vector<Entity*>, maxGroups> groupedEntities;

public:
    void update(float elapsedTime) {
        for(auto& e : entities) e->update(elapsedTime);
    }

    void draw() {
        for(auto& e : entities) e->draw();
    }

    void addToGroup(Entity* entity, Group group) {
        groupedEntities[group].emplace_back(entity);
    }

    std::vector<Entity*>& getEntitiesByGroup(Group group) {
        return groupedEntities[group];
    }

    void refresh() {
        for(int i = 0; i < maxGroups; i++) {
            auto& v = groupedEntities[i];
            v.erase(
                std::remove_if(std::begin(v), std::end(v),
                    [i](Entity* entity) {
                        return !entity->isAlive() || !entity->hasGroup(i);
                    }),
                std::end(v));
        }

        entities.erase(
                std::remove_if(std::begin(entities), std::end(entities),
                    [](const std::unique_ptr<Entity>& e) {
                        return !e->isAlive();
                    }),
                std::end(entities));
    }

    Entity& addEntity() {
        Entity* e = new Entity(*this);
        std::unique_ptr<Entity> uPtr{e};
        entities.emplace_back(std::move(uPtr));

        return *e;
    }
};

void Entity::addGroup(Group group) {
    groupBitset[group] = true;
    manager.addToGroup(this, group);
}

#endif /* MANAGER_H */
