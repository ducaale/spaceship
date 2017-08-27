#ifndef MANAGER_H
#define MANAGER_H

#include <vector>
#include <queue>
#include <memory>
#include <algorithm>

#include "entity.h"
#include "groups.h"

struct Manager {
private:
    std::vector<std::unique_ptr<Entity>> entities;
    std::array<std::vector<Entity*>, maxGroups> groupedEntities;

    std::queue<std::unique_ptr<Entity>> waitingEntities;
    std::queue<std::pair<Entity*, Group>> waitingGroups;
public:

    void update(float elapsedTime) {
        for(auto& e : entities) e->update(elapsedTime);
    }

    void draw() {
        for(auto& e : groupedEntities[Groups::drawable]) e->draw();
    }

    void addToGroup(Entity* entity, Group group) {
        groupedEntities[group].emplace_back(entity);
    }

    void addToWaitingGroup(Entity* entity, Group group) {
        waitingGroups.emplace(std::make_pair(entity, group));
    }

    std::vector<Entity*>& getEntitiesByGroup(Group group) {
        return groupedEntities[group];
    }

    Entity* getByGroup(Group group) {
        return groupedEntities[group][0];
    }

    void refresh() {
        while(!waitingEntities.empty()) {
            entities.push_back(std::move(waitingEntities.front()));
            waitingEntities.pop();
        }

        if(waitingGroups.size() > 0) {
            while(!waitingGroups.empty()) {

                auto [entity, group] = waitingGroups.front();
                addToGroup(entity, group);
                waitingGroups.pop();
            }

            std::sort(
                begin(groupedEntities[Groups::drawable]),
                end(groupedEntities[Groups::drawable]),
                [](const auto a, const auto b) -> bool {
                    return a->getLayer() < b->getLayer();
                });
        }

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
        waitingEntities.emplace(std::move(uPtr));
        return *e;
    }
};

#endif /* MANAGER_H */
