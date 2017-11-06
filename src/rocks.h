#ifndef ROCKS_H
#define ROCKS_H

#include "game.h"
#include "components.h"

void createRocks(Game *game) {

    std::vector<sf::Vector2f> positions = {
        {241.5f, -817.7f}, {-277.f, -735.f}, {-111.f, -629.f}, {762.f, 749.f},
        {246.5f, 236.7f}, {-431.f, 13.f}, {348.f, -542.f}, {33.f, 556.f},
        {282.5f, 49.7f}, {-463.f, -113.f}
    };

    for(auto position : positions) {
        auto& entity = game->manager.addEntity();

        entity.addComponent<CTransform>(position);

        auto& cSprite = entity.addComponent<CSprite>(game, sf::Sprite(game->resource["rocks"], {0,0,64,64}));
        cSprite.setScale(1.5f, 1.5f);

        entity.addGroup(Groups::drawable);
        entity.setLayer(-5);
    }
}

#endif /* end of include guard: ROCKS_H */
