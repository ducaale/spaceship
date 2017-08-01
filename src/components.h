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

/*
 * https://gist.github.com/JISyed/6445974
 */
struct CTarget : Component {
    CSprite *cSprite = nullptr;
    CPosition *cPosition = nullptr;

    Spaceship& target;
    float turn_speed, accuracy;
    float least_accurate_aim = 30.0f;

    CTarget(Spaceship& target, float turn_speed, float accuracy) :
        target(target),
        turn_speed(turn_speed),
        accuracy(1.0 - accuracy)
    {}

    void init() override {
        cSprite = &entity->getComponent<CSprite>();
        cPosition = &entity->getComponent<CPosition>();
    }

    void update(float elapsedTime) override {
        float target_angle = atan2(cPosition->y() - target.getCoordinate().y,
                cPosition->x() - target.getCoordinate().x) * 180 / PI;

        float facing_angle = cSprite->sprite.getRotation();

        if(target_angle < 0) {
            target_angle += 360;
        }


        float angle_diff =  facing_angle - target_angle;

        if(std::abs(angle_diff) > 180) {
            if(facing_angle > target_angle) {
                angle_diff = - (360 - facing_angle + target_angle);
            }
            else {
                angle_diff = 360 - target_angle + facing_angle;
            }
        }

        if(angle_diff > least_accurate_aim * accuracy) {
            cSprite->sprite.rotate(-turn_speed);
        }
        else if(angle_diff < least_accurate_aim * -accuracy) {
            cSprite->sprite.rotate(turn_speed);
        }

    }

};

#endif /* end of include guard: COMPONENTS_H_QD5OJVYS */

