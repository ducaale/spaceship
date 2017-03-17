#ifndef MANAGER_H
#define MANAGER_H

#include <vector>
#include <memory>
#include <algorithm>

#include "entity.h"

struct Manager {
private:
    std::vector<std::unique_ptr<Entity>> entities;

public:
    void update(float elapsedTime) {
        for(auto& e : entities) e->update(elapsedTime);
    }

    void draw() {
        for(auto& e : entities) e->draw();
    }

    void refresh() {
        entities.erase(
                std::remove_if(std::begin(entities), std::end(entities),
                    [](const std::unique_ptr<Entity>& e) {
                        return !e->isAlive();
                    }),
                std::end(entities));
    }

    Entity& addEntity() {
        Entity* e = new Entity();
        std::unique_ptr<Entity> uPtr{e};
        entities.emplace_back(std::move(uPtr));

        return *e;
    }
};

#endif /* MANAGER_H */
