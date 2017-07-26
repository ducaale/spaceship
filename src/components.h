#ifndef COMPONENTS_H_QD5OJVYS
#define COMPONENTS_H_QD5OJVYS

#include <vector>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "game.h"
#include "component.h"
#include "entity.h"

#include "spaceship.h" //temporary
#include "utility.h"

struct CParent;
struct CSprite;

struct CPosition : Component {
    sf::Vector2f position;

    CPosition() = default;
    CPosition(const sf::Vector2f& position) : position(position) {}

    float x() const { return position.x;  }
    float y() const { return position.y;  }
};

struct CParent : Component {
    Entity *parent = nullptr;
    CParent(Entity *entity) : parent(entity) {}

    sf::Transform getTransform();
};

struct CSprite : Component {
    Game *game = nullptr;
    CPosition *cPosition = nullptr;
    CParent *cParent = nullptr;

    sf::Sprite sprite;
    float width, height;

    CSprite() = default;
    CSprite(Game* game, const sf::Sprite& sprite) :
        sprite(sprite),
        game(game)
    {
        width = sprite.getGlobalBounds().width;
        height = sprite.getGlobalBounds().height;
        this->sprite.setOrigin(width/2, height/2);
    }

    void init() override {
        cPosition = &entity->getComponent<CPosition>();
        if(entity->hasComponent<CParent>()) {
            cParent = &entity->getComponent<CParent>();
        }
    }

    sf::Transform getTransform() {
        sprite.setOrigin(0.f, 0.f);
        sf::Transform t = sprite.getTransform();
        sprite.setOrigin(width/2.0f, height/2.0f);

        return t;
    }

    void update(float elapsedTime) override {
        sprite.setPosition(cPosition->position);
    }

    void draw() override {
        if(cParent) {
            game->render(sprite, cParent->getTransform());
        }
        else {
            game->render(sprite);
        }
    }
};

sf::Transform CParent::getTransform() {
    return parent->getComponent<CSprite>().getTransform();
}

struct CRotate : Component {
    CSprite *cSprite = nullptr;
    CPosition *cPosition = nullptr;

    void init() override {
       cSprite = &entity->getComponent<CSprite>();
       cPosition = &entity->getComponent<CPosition>();
    }

    void update(float elapsedTime) override {
        cSprite->sprite.rotate(0.2);
    }
};

struct CTarget : Component {
    CSprite *cSprite = nullptr;
    CPosition *cPosition = nullptr;
    Spaceship& target;

    CTarget(Spaceship& target) : target(target) {}

    void init() override {
        cSprite = &entity->getComponent<CSprite>();
        cPosition = &entity->getComponent<CPosition>();
    }

    void update(float elapsedTime) override {
        float angle = atan2(cPosition->y() - target.getCoordinate().y,
                cPosition->x() - target.getCoordinate().x) * 180 / PI;

        cSprite->sprite.rotate((angle - cSprite->sprite.getRotation()));
    }

};

#endif /* end of include guard: COMPONENTS_H_QD5OJVYS */

