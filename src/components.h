#ifndef COMPONENTS_H_QD5OJVYS
#define COMPONENTS_H_QD5OJVYS

#include <vector>
#include <map>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "game.h"
#include "component.h"
#include "entity.h"
#include "groups.h"

#include "AnimatedSprite.hpp"

#include "spaceship.h" //temporary
#include "utility.h"

struct CParent;
struct CSprite;

struct CTransform : Component {
    sf::Vector2f position;
    float angle;

    CTransform() = default;
    CTransform(const sf::Vector2f& position, float angle = 0) :
        position(position),
        angle(angle)
    {}

    float x() const { return position.x;  }
    float y() const { return position.y;  }

    sf::Vector2f getDirection() const {
        // in sfml y axis is upside down
        // but angle rotates clockwise
        return sf::Vector2f(-cos(angle * PI / 180), -sin(angle * PI / 180));
    }
};

struct CPhysics : Component {
    CTransform *cTransform = nullptr;
    sf::Vector2f velocity;
    float maxSpeed, speed, acceleration;

    CPhysics(float maxSpeed, float speed, float acceleration) :
        velocity{0, 0},
        maxSpeed(maxSpeed),
        speed(speed),
        acceleration(acceleration)
    {}

    void init() override {
        cTransform = &entity->getComponent<CTransform>();
    }

    void update(float elapsedTime) override {
        cTransform->position += velocity;
        velocity = cTransform->getDirection() * speed * elapsedTime;
    }

    void accelerate(float elapsedTime) {
        speed += acceleration * elapsedTime;
        if(speed > maxSpeed) {
            speed = maxSpeed;
        }
    }

    void deccelerate(float elapsedTime) {
        speed -= acceleration * elapsedTime;
        if(speed < 0) {
            speed = 0;
        }
    }
};

struct CParent : Component {
    Entity *parent = nullptr;
    CParent(Entity *entity) : parent(entity) {}

    sf::Transform getTransform();
};

struct CSprite : Component {
    Game *game = nullptr;
    CTransform *cTransform = nullptr;
    CParent *cParent = nullptr;

    sf::Sprite sprite;
    std::vector<sf::IntRect> rects;

    float width, height;

    CSprite() = default;
    CSprite(Game* game, const sf::Sprite& sprite) :
        game(game),
        sprite(sprite)
    {
        width = sprite.getGlobalBounds().width;
        height = sprite.getGlobalBounds().height;
        this->sprite.setOrigin(width/2, height/2);
    }

    void init() override {
        cTransform = &entity->getComponent<CTransform>();
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

    void draw() override {
        sprite.setPosition(cTransform->position);
        sprite.setRotation(cTransform->angle);
        if(cParent) {
            game->render(sprite, cParent->getTransform());
        }
        else {
            game->render(sprite);
        }
    }
};

struct CAnimatedSprite : Component {
    Game *game = nullptr;
    CTransform *cTransform = nullptr;
    CParent *cParent = nullptr;

    AnimatedSprite sprite;
    std::map<std::string, Animation> animations;
    std::string currentAnimation;

    float width, height;

    CAnimatedSprite() = default;
    CAnimatedSprite(Game* game, const AnimatedSprite& sprite, float width, float height) :
        game(game),
        sprite(sprite),
        width(width),
        height(height)
    {
        this->sprite.setOrigin(width/2, height/2);
    }

    void init() override {
        cTransform = &entity->getComponent<CTransform>();
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

    void play(const std::string animation) {
        sprite.play(animations[animation]);
    }

    void update(float elapsedTime) override {
        sprite.update(sf::seconds(elapsedTime));

        sprite.setPosition(cTransform->position);
        sprite.setRotation(cTransform->angle);
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
    if(parent->hasComponent<CSprite>()) {
        return parent->getComponent<CSprite>().getTransform();
    }
    else {
        return parent->getComponent<CAnimatedSprite>().getTransform();
    }
}


/*
 * https://gist.github.com/JISyed/6445974
 */
struct CTarget : Component {
    CTransform *cTransform = nullptr;

    Spaceship& target;
    float turn_speed, accuracy;
    float least_accurate_aim = 30.0f;

    CTarget(Spaceship& target, float turn_speed, float accuracy) :
        target(target),
        turn_speed(turn_speed),
        accuracy(1.0 - accuracy)
    {}

    void init() override {
        cTransform = &entity->getComponent<CTransform>();
    }

    void update(float elapsedTime) override {
        float target_angle = atan2(cTransform->y() - target.getCoordinate().y,
                cTransform->x() - target.getCoordinate().x) * 180 / PI;

        float facing_angle = cTransform->angle;

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
            cTransform->angle -= turn_speed;
        }
        else if(angle_diff < least_accurate_aim * -accuracy) {
            cTransform->angle += turn_speed;
        }

    }

};

struct CGun : Component {
    Game *game;
    sf::Sprite sprite;
    float rateOfFire, speed;
    float lastFired = 0.f;
    int projShot = 0;

    CGun(Game *game, sf::Sprite sprite, float rateOfFire, float speed) :
        game(game),
        sprite(sprite),
        rateOfFire(rateOfFire),  // bullets per second
        speed(speed)
    {}

    void update(float elapsedTime) override {
        lastFired += elapsedTime;
    }

    void fire(sf::Vector2f position, float angle, Spaceship* target=nullptr) {
        if(lastFired > 1/rateOfFire) {
            auto& entity = game->manager.addEntity();
            entity.addComponent<CTransform>(position, angle);
            entity.addComponent<CPhysics>(speed, speed, 0.f);
            entity.addComponent<CSprite>(game, sprite);

            if(target) {
                entity.addComponent<CTarget>(*target, 0.5f, 0.8f);
            }

            entity.addGroup(Groups::drawable);

            lastFired = 0.f;
            ++projShot;
        }
    }
};

struct CLaserGun : Component {
    Game *game;
    sf::Sprite sprite;
    bool laserOn = false;

    CLaserGun(Game *game, sf::Sprite sprite) :
        game(game),
        sprite(sprite)
    {}

    void update(float elapsedTime) override {
    }

    void openLaser() {
        if(!laserOn) {
            auto& firstPart = game->manager.addEntity();
            firstPart.addComponent<CTransform>(sf::Vector2f(-256 - 64, 0));
            firstPart.addComponent<CParent>(this->entity);
            firstPart.addComponent<CSprite>(game, sprite);
            firstPart.addGroup(Groups::drawable);

            auto& secondPart = game->manager.addEntity();
            secondPart.addComponent<CTransform>(sf::Vector2f((-256 - 64) * 2, 0));
            secondPart.addComponent<CParent>(this->entity);
            secondPart.addComponent<CSprite>(game, sprite);
            secondPart.addGroup(Groups::drawable);

            laserOn = true;
        }
    }
};

struct CEnemyInput : Component {
    CAnimatedSprite *cSprite = nullptr;

    void init() override {
        cSprite = &entity->getComponent<CAnimatedSprite>();
    }

    void update(float elapsedTime) override {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
            cSprite->sprite.stop();
            cSprite->play("closeEyeAnimation");
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::O)) {
            cSprite->sprite.stop();
            cSprite->play("openEyeAnimation");
        }
    }
};

struct COrbArmBehaviour: Component {
    CGun *cGun = nullptr;
    CTransform *cTransform = nullptr;
    CParent *cParent = nullptr;

    void init() override {
        cGun = &entity->getComponent<CGun>();
        cTransform = &entity->getComponent<CTransform>();
        cParent = &entity->getComponent<CParent>();
    }

    void update(float elapsedTime) override {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::K)) {

            sf::Transform t = cParent->getTransform();
            sf::Vector2f gunPos = cTransform->position;
            gunPos.x -= 64.f;
            gunPos.y += (cGun->projShot % 2 ?  -5 : 5);
            sf::Vector2f globalPosition = t.transformPoint(gunPos);
            float globalAngle = atan2(t.getMatrix()[1], t.getMatrix()[0]) * 180 / PI;

            cGun->fire(globalPosition, globalAngle);
        }
    }
};

struct CRLBehaviour: Component {
    CTransform *cTransform = nullptr;
    CParent *cParent = nullptr;
    CGun *cGun = nullptr;
    Spaceship& target;

    bool opening = false;
    bool closing = false;
    bool open = false;

    CRLBehaviour(Spaceship& target) : target(target) {}

    void init() override {
        cTransform = &entity->getComponent<CTransform>();
        cParent = &entity->getComponent<CParent>();
        cGun = &entity->getComponent<CGun>();
    }

    void update(float elapsedTime) override {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
            opening = true;
            closing = false;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
            closing = true;
            opening = false;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::N)) {
            sf::Transform t = cParent->getTransform();
            sf::Vector2f gunPos = cTransform->position;

            gunPos.y += (cGun->projShot % 2 ?  -5 : -15);

            gunPos.x -= 16.f;
            sf::Vector2f globalPosition = t.transformPoint(gunPos);
            float globalAngle = atan2(t.getMatrix()[1], t.getMatrix()[0]) * 180 / PI;

            cGun->fire(globalPosition, globalAngle, &target);
        }

        if(opening) {
            cTransform->position.y += -30.f * elapsedTime;
            if(cTransform->position.y < -65.f) {
                cTransform->position.y = -65.f;
                opening = false;
                open = true;
            }
        }
        else if(closing) {
            cTransform->position.y += 30.f * elapsedTime;
            if(cTransform->position.y > -32.f) {
                cTransform->position.y = -32.f;
                closing = false;
                open = false;
            }
        }
    }
};

struct COrbBehaviour : Component {
    CLaserGun *cLaserGun = nullptr;

    void init() override {
        cLaserGun = &entity->getComponent<CLaserGun>();
    }

    void update(float elapsedTime) override {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::M)) {
            cLaserGun->openLaser();
        }
    }
};


#endif /* end of include guard: COMPONENTS_H_QD5OJVYS */

