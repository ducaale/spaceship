#ifndef COMPONENTS_H_QD5OJVYS
#define COMPONENTS_H_QD5OJVYS

#include <vector>
#include <map>
#include <functional>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "game.h"
#include "component.h"
#include "entity.h"
#include "groups.h"

#include "AnimatedSprite.hpp"

#include "utility.h"

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
        if(acceleration > 0) {
            cTransform->position += velocity * elapsedTime;
        }
        else {
            cTransform->position += speed * cTransform->getDirection() * elapsedTime;
        }
    }

    void accelerate(float elapsedTime) {
        velocity += cTransform->getDirection() * acceleration * elapsedTime;
    }

    void deccelerate(float elapsedTime) {
        velocity = velocity * 0.99f;
    }

    float getSpeed() {
        return utility::magnitude(velocity);
    }

    float getSpeedPercentage() {
        return getSpeed() / maxSpeed;
    }
};

struct CParent : Component {
    Entity *parent = nullptr;
    CParent(Entity *entity) : parent(entity) {}

    sf::Transform getTransform(bool origin=false, bool scale=false);
};

template <class T>
struct BaseSprite : Component {
    Game *game = nullptr;
    CTransform *cTransform = nullptr;
    CParent *cParent = nullptr;

    T sprite;

    float centerX, centerY;
    float scaleX, scaleY;

    BaseSprite(Game* game, const T& sprite, float centerX, float centerY) :
        game(game),
        sprite(sprite),
        centerX(centerX),
        centerY(centerY),
        scaleX(1.f),
        scaleY(1.f)
    {
        this->sprite.setOrigin(centerX, centerY);
        this->sprite.setScale(scaleX, scaleY);
    }

    BaseSprite(Game* game, const sf::Sprite& sprite) :
        game(game),
        sprite(sprite),
        scaleX(1.f),
        scaleY(1.f)
    {
        centerX = sprite.getGlobalBounds().width / 2.f;
        centerY = sprite.getGlobalBounds().height / 2.f;

        this->sprite.setOrigin(centerX, centerY);
        this->sprite.setScale(scaleX, scaleY);
    }

    void init() override {
        cTransform = &entity->getComponent<CTransform>();
        if(entity->hasComponent<CParent>()) {
            cParent = &entity->getComponent<CParent>();
        }
    }


    void setScale(float scaleX, float scaleY) {
        sprite.setScale(scaleX, scaleY);

        this->scaleX = scaleX;
        this->scaleY = scaleY;
    }

    void setOrigin(float centerX, float centerY) {
        this->centerX = centerX;
        this->centerY = centerY;

        this->sprite.setOrigin(centerX, centerY);
    }

    float width() {
        return sprite.getLocalBounds().width;
    }

    float height() {
        return sprite.getLocalBounds().height;
    }

    sf::Transform getTransform(bool origin=false, bool scale=false) {

        sf::Transform transform;

        if(origin && scale) {
            transform = sprite.getTransform();
            return transform;
        }

        else if(!origin && scale) {
            sprite.setOrigin(0.f, 0.f);
            transform = sprite.getTransform();
            sprite.setOrigin(centerX, centerY);
        }

        else if(origin && !scale) {
            sprite.setScale(1.f, 1.f);
            transform = sprite.getTransform();
            sprite.setScale(scaleX, scaleY);
        }

        else {
            sprite.setOrigin(0.f, 0.f);
            sprite.setScale(1.f, 1.f);

            transform = sprite.getTransform();

            sprite.setOrigin(centerX, centerY);
            sprite.setScale(scaleX, scaleY);
        }

        return transform;
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

struct CSprite : BaseSprite<sf::Sprite> {

    // rect, origin, scale
    std::map<std::string, std::tuple<sf::IntRect, sf::Vector2f, sf::Vector2f>> frames;

    CSprite(Game* game, const sf::Sprite& sprite, float centerX, float centerY) :
        BaseSprite<sf::Sprite>(game, sprite, centerX, centerY)
    {}

    CSprite(Game* game, const sf::Sprite& sprite) :
        BaseSprite<sf::Sprite>(game, sprite)
    {}

    void changeFrame(const std::string frame) {

        auto [ rect, origin, scale ] = frames[frame];
        sprite.setTextureRect(rect);
        sprite.setOrigin(origin);
        sprite.setScale(scale);

        scaleX = scale.x;
        scaleY = scale.y;

        centerX = origin.x;
        centerY = origin.y;
    }

    void update(float elapsedTime) override {
        sprite.setPosition(cTransform->position);
        sprite.setRotation(cTransform->angle);
    }
};

struct CAnimatedSprite  : BaseSprite<AnimatedSprite> {

    std::map<std::string, Animation> animations;

    CAnimatedSprite(Game* game, const AnimatedSprite& sprite, float centerX, float centerY) :
        BaseSprite<AnimatedSprite>(game, sprite, centerX, centerY)
    {}

    void play(const std::string animation) {
        sprite.play(animations[animation]);
    }

    void play(const std::string animation, std::function<void()> onAnimationDone) {
        sprite.play(animations[animation], onAnimationDone);
    }

    void setAnimation(const std::string animation) {
        sprite.setAnimation(animations[animation]);
    }

    void update(float elapsedTime) override {
        sprite.setPosition(cTransform->position);
        sprite.setRotation(cTransform->angle);

        sprite.update(sf::seconds(elapsedTime));
    }
};

/*
 * https://gist.github.com/JISyed/6445974
 */
struct CTarget : Component {
    CTransform *cTransform = nullptr;
    Entity *target = nullptr;
    Game *game = nullptr;

    float turn_speed, accuracy;
    float least_accurate_aim = 30.0f;

    bool targetOn = true;

    CTarget(Game *game, Group target_name, float turn_speed, float accuracy) :
        game(game),
        turn_speed(turn_speed),
        accuracy(1.0 - accuracy)
    {
        target = game->manager.getByGroup(target_name);
    }

    void init() override {
        cTransform = &entity->getComponent<CTransform>();
    }

    void disableTargeting() {
        targetOn = false;
    }

    void enableTargeting() {
        targetOn = true;
    }

    void update(float elapsedTime) override {
        if(!targetOn) return;

        float target_angle = atan2(cTransform->y() - target->getComponent<CTransform>().y(),
                cTransform->x() - target->getComponent<CTransform>().x()) * 180 / PI;

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
            cTransform->angle -= turn_speed * elapsedTime;
        }
        else if(angle_diff < least_accurate_aim * -accuracy) {
            cTransform->angle += turn_speed * elapsedTime;
        }

    }

};

struct CTimerKiller : Component {

    float counter = 0;
    float end;

    CTimerKiller(float end) : end(end) {}

    void update(float elapsedTime) override {
        counter += elapsedTime;

        if(counter > end) entity->destroy();
    }
};

struct CGradualTransparency : Component {
    CSprite *cSprite;

    void init() override {
        cSprite = &entity->getComponent<CSprite>();
    }

    void update(float elapsedTime) override {
        auto color = cSprite->sprite.getColor();
        color.a = utility::clamp(color.a - 0.001 * elapsedTime, 0, 255);
        cSprite->sprite.setColor(color);
    }
};

struct CPathTrail : Component {
    Game *game;
    sf::Sprite sprite;
    CTransform *cTransform = nullptr;

    CPathTrail(Game *game, sf::Sprite sprite) :
        game(game),
        sprite(sprite)
    {}

    void init() override {
        cTransform = &entity->getComponent<CTransform>();
    }

    void update(float elapsedTime) override {
        auto& entity = game->manager.addEntity();

        entity.addComponent<CTransform>(cTransform->position, cTransform->angle);

        auto& cSprite = entity.addComponent<CSprite>(game, sprite);
        cSprite.sprite.setColor(sf::Color(128, 128, 128, 50));

        entity.addComponent<CGradualTransparency>();
        entity.addComponent<CTimerKiller>(6.f);
        entity.setLayer(-2);
        entity.addGroup(Groups::drawable);
    }
};

struct CCollision : Component {
    std::function<void(Entity& e)> onCollision;
};

struct CGun : Component {
    Game *game;
    sf::Sprite sprite;
    float rateOfFire, speed;
    float lastFired = 0.f;
    int projShot = 0;
    Group group;

    CGun(Game *game, sf::Sprite sprite, float rateOfFire, float speed, Group group) :
        game(game),
        sprite(sprite),
        rateOfFire(rateOfFire),  // bullets per second
        speed(speed),
        group(group)
    {}

    void update(float elapsedTime) override {
        lastFired += elapsedTime;
    }

    void fire(sf::Vector2f position, float angle, Group target_name=0) {
        if(lastFired > 1/rateOfFire) {
            auto& entity = game->manager.addEntity();
            auto& cTransform = entity.addComponent<CTransform>(position, angle);
            entity.addComponent<CPhysics>(speed, speed, 0.f);
            entity.addComponent<CSprite>(game, sprite);
            entity.addComponent<CTimerKiller>(5);

            if(target_name) {
                entity.addComponent<CTarget>(game, target_name, 15.f, 0.8f);
                entity.addComponent<CPathTrail>(game, sf::Sprite(game->resource["orb"], {0,230,8,8}));
            }

            auto& cCollision = entity.addComponent<CCollision>();
            cCollision.onCollision = [this, &entity, &cTransform] (Entity& e) {
                this->onCollision(cTransform.position, cTransform.angle);
                entity.destroy();
            };

            entity.addGroup(Groups::drawable);
            entity.addGroup(Groups::collidable);
            entity.addGroup(Groups::bullet);
            entity.addGroup(group);

            lastFired = 0.f;
            ++projShot;
        }
    }

    void onCollision(sf::Vector2f position, float angle) {
        auto& entity = game->manager.addEntity();

        entity.addComponent<CTransform>(position, angle);
        entity.addComponent<CTimerKiller>(0.45f);
        auto& cSprite = entity.addComponent<CAnimatedSprite>(game, AnimatedSprite(sf::seconds(0.1), false, false), 16, 16);
        cSprite.setScale(1.5f, 1.5f);

        Animation bullet_impact;
        bullet_impact.setSpriteSheet(game->resource["guns"]);
        for(int k = 0; k < 4; k++) bullet_impact.addFrame(sf::IntRect(64 * k,64,32,32));

        cSprite.animations["bullet_impact"] = bullet_impact;
        cSprite.setAnimation("bullet_impact");

        entity.addGroup(Groups::drawable);
    }
};


struct CLaserGun : Component {
    Game *game;
    sf::Sprite sprite;
    bool laserOn = false;

    Entity *laser = nullptr;

    CLaserGun(Game *game, sf::Sprite sprite) :
        game(game),
        sprite(sprite)
    {}

    void update(float elapsedTime) override {
    }

    void openLaser() {
        if(!laserOn) {
            laser = &game->manager.addEntity();
            laser->addComponent<CTransform>(sf::Vector2f(-90, 0));
            laser->addComponent<CParent>(this->entity);

            auto& laserSprite = laser->addComponent<CSprite>(game, sprite);
            laserSprite.setOrigin(laserSprite.width(), laserSprite.height()/2);
            laserSprite.setScale(2.5f, 1.3f);

            laser->addGroup(Groups::drawable);

            laserOn = true;
        }
    }

    void closeLaser() {
        if(laserOn) {
            laser->destroy();

            laserOn = false;
        }
    }
};

#endif /* end of include guard: COMPONENTS_H_QD5OJVYS */

