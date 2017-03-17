#ifndef COMPONENTS_H_QD5OJVYS
#define COMPONENTS_H_QD5OJVYS

#include <vector>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "game.h"
#include "component.h"
#include "entity.h"

struct CPosition : Component {
    sf::Vector2f position;

    CPosition() = default;
    CPosition(const sf::Vector2f& position) : position(position) {}

    float x() const { return position.x;  }
    float y() const { return position.y;  }
};

struct CSprite : Component {
    Game *game = nullptr;
    sf::Sprite sprite;
    CPosition* cPosition = nullptr;

    CSprite() = default;
    CSprite(Game* game, const sf::Sprite& sprite) :
        sprite(sprite),
        game(game)
    {}

    void init() override {
        cPosition = &entity->getComponent<CPosition>();
    }

    void update(float elapsedTime) override {
        sprite.setPosition(cPosition->position - game->camera->getCoordinate());
    }

    void draw() override {
        game->render(sprite);
    }
};

#endif /* end of include guard: COMPONENTS_H_QD5OJVYS */

