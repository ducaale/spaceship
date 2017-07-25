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

struct CParent : Component {
    Entity *parent = nullptr;    
    CParent(Entity *entity) : parent(entity) {}
};

struct CSprite : Component {
    Game *game = nullptr;
    sf::Sprite sprite;
    CPosition *cPosition = nullptr;
    float width, height;
    CParent *cParent = nullptr;

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
            game->render(sprite, cParent->parent->getComponent<CSprite>().getTransform());
        } 
        else {
            game->render(sprite);
        }
    }
};

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

#endif /* end of include guard: COMPONENTS_H_QD5OJVYS */

