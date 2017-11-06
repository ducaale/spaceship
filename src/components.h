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

struct CTimerKiller : Component {

    float counter = 0;
    float end;

    CTimerKiller(float end) : end(end) {}

    void update(float elapsedTime) override {
        counter += elapsedTime;

        if(counter > end) entity->destroy();
    }
};

struct CExplosion : Component {

    Game *game = nullptr;

    CExplosion(Game *game) : game(game) {}

    void explode() {

        auto position = entity->getComponent<CTransform>().position;

        auto& explosion_entity = game->manager.addEntity();
        explosion_entity.addComponent<CTransform>(position);
        explosion_entity.addComponent<CTimerKiller>(0.45f);
        auto& cSprite = explosion_entity.addComponent<CAnimatedSprite>(game, AnimatedSprite(sf::seconds(0.1), false, false), 32, 32);
        cSprite.setScale(2.5f, 2.5f);

        Animation explosion;
        explosion.setSpriteSheet(game->resource["guns"]);
        for(int k = 0; k < 2; k++) explosion.addFrame(sf::IntRect(0,192,64,64));
        for(int k = 0; k < 4; k++) explosion.addFrame(sf::IntRect(64,192,64,64));

        cSprite.animations["explosion"] = explosion;
        cSprite.setAnimation("explosion");

        explosion_entity.addGroup(Groups::drawable);
        explosion_entity.setLayer(2);
    }
};

struct CHealth : Component {
    int health;
    float lastHit = 0;
    float invincibleFrames = 3.f;

    bool isInvincible = false;

    CHealth(int health) : health(health) {}

    void update(float elapsedTime) override {
        if(isInvincible) {
            lastHit += elapsedTime;

            if(lastHit > invincibleFrames) {
                isInvincible = false;
                lastHit = 0;
            }
        }
    }

    int isAlive() { return health > 0; }

    void loseHealth(int amount) {
        if(isInvincible || !isAlive()) return;

        health = amount > health ? 0 : health - amount;

        if(!isAlive()) onDeath();

        isInvincible = true;

        if(!isAlive()) entity->delGroup(Groups::drawable);
    }

    void onDeath() {
        if(entity->hasComponent<CExplosion>()) {
            entity->getComponent<CExplosion>().explode();
        }
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
    float least_accurate_aim = 5.f;

    bool targetOn = true;

    float target_angle;

    CTarget(Game *game, Group target_name, float turn_speed, float accuracy) :
        game(game),
        turn_speed(turn_speed),
        accuracy(1.0 - accuracy)
    {
        target = game->manager.getByGroup(target_name);
    }

    void init() override {
        cTransform = &entity->getComponent<CTransform>();

        target_angle = atan2(cTransform->y() - target->getComponent<CTransform>().y(),
                cTransform->x() - target->getComponent<CTransform>().x()) * 180 / PI;
    }

    void disableTargeting() {
        targetOn = false;
    }

    void enableTargeting() {
        targetOn = true;
    }

    void update(float elapsedTime) override {
        if(targetOn) {
            target_angle = atan2(cTransform->y() - target->getComponent<CTransform>().y(),
                    cTransform->x() - target->getComponent<CTransform>().x()) * 180 / PI;
        }


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

struct CCircleCollider : Component {
    float radius;
    CTransform *cTransform = nullptr;

    CCircleCollider(float radius) : radius(radius) {}

    void init() override {
        cTransform = &entity->getComponent<CTransform>();
    }

    sf::Vector2f center() {
        return cTransform->position;
    }
};

struct CRocketBehaviour : Component {
    Game *game = nullptr;
    CTransform *cTransform = nullptr;
    CCollision *cCollision = nullptr;
    Entity* player = nullptr;

    CRocketBehaviour(Game* game) : game(game) {}

    void init() override {
        cTransform = &entity->getComponent<CTransform>();
        cCollision = &entity->getComponent<CCollision>();
        player = game->manager.getByGroup(Groups::player);
    }

    void update(float elapsedTime) override {
        auto playerPos = player->getComponent<CTransform>().position;
        auto currentPos = cTransform->position;

        if(utility::magnitude(currentPos, playerPos) < 110) {
            cCollision->onCollision(*player);
        }
    }
};

struct CGun : Component {
    Game *game;
    sf::Sprite sprite;
    float rateOfFire, speed;
    float lastFired = 0.f;
    int projShot = 0;
    Group group;

    float scaleX = 1;
    float scaleY = 1;

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

    void setScale(float scaleX, float scaleY) {
        this->scaleX = scaleX;
        this->scaleY = scaleY;
    }

    void fire(sf::Vector2f position, float angle, Group target_name=0) {
        if(lastFired > 1/rateOfFire) {
            auto& entity = game->manager.addEntity();
            auto& cTransform = entity.addComponent<CTransform>(position, angle);
            auto& cPhysics = entity.addComponent<CPhysics>(speed, speed, 0.f);

            auto& cSprite = entity.addComponent<CSprite>(game, sprite);
            cSprite.setScale(scaleX, scaleY);

            auto& cTimerKiller = entity.addComponent<CTimerKiller>(5);

            auto& cCollision = entity.addComponent<CCollision>();
            cCollision.onCollision = [this, &entity, &cTransform, target_name] (Entity& e) {
                this->onCollision(cTransform.position, cTransform.angle, target_name);
                entity.destroy();

                if(other.hasComponent<CHealth>()) {
                    if(target_name) {
                        other.getComponent<CHealth>().loseHealth(2);
                    }
                    else {
                        other.getComponent<CHealth>().loseHealth(1);
                    }
                }
            };

            if(target_name) {
                entity.addComponent<CTarget>(game, target_name, 15.f, 1.f);
                entity.addComponent<CRocketBehaviour>(game);
                //entity.addComponent<CPathTrail>(game, sf::Sprite(game->resource["guns"], {0,130,8,8}));

                cPhysics.speed /= 2.f;
                cTimerKiller.end *= 3;
            }

            entity.addGroup(Groups::drawable);
            entity.addGroup(Groups::collidable);
            entity.addGroup(Groups::bullet);
            entity.addGroup(group);

            lastFired = 0.f;
            ++projShot;
        }
    }

    void onCollision(sf::Vector2f position, float angle, Group target_name) {
        auto& entity = game->manager.addEntity();

        entity.addComponent<CTransform>(position, angle);
        entity.addComponent<CTimerKiller>(0.45f);

        if(target_name) {
            auto& cSprite = entity.addComponent<CAnimatedSprite>(game, AnimatedSprite(sf::seconds(0.1), false, false), 32, 32);
            cSprite.setScale(3.5f, 3.5f);

            Animation rocket_impact;
            rocket_impact.setSpriteSheet(game->resource["guns"]);
            for(int k = 0; k < 2; k++) rocket_impact.addFrame(sf::IntRect(0,192,64,64));
            for(int k = 0; k < 4; k++) rocket_impact.addFrame(sf::IntRect(64,192,64,64));

            cSprite.animations["rocket_impact"] = rocket_impact;
            cSprite.setAnimation("rocket_impact");
        }
        else {
            auto& cSprite = entity.addComponent<CAnimatedSprite>(game, AnimatedSprite(sf::seconds(0.1), false, false), 16, 16);
            cSprite.setScale(1.5f, 1.5f);

            Animation bullet_impact;
            bullet_impact.setSpriteSheet(game->resource["guns"]);
            for(int k = 0; k < 4; k++) bullet_impact.addFrame(sf::IntRect(64 * k,64,32,32));

            cSprite.animations["bullet_impact"] = bullet_impact;
            cSprite.setAnimation("bullet_impact");
        }


        entity.setLayer(2);
        entity.addGroup(Groups::drawable);
    }
};

struct CLaserGun : Component {
    Game *game;
    sf::Sprite sprite;

    Entity *laser = nullptr;

    float width = 1.3f;
    float length = 4.5f;

    float lastOpened = 0;

    enum States : std::size_t {
        opening,
        on,
        closing,
        off
    };

    States state = States::off;

    CLaserGun(Game *game, sf::Sprite sprite) :
        game(game),
        sprite(sprite)
    {}

    void update(float elapsedTime) override {
        if(state == States::closing) {
            lastOpened = 0;

            auto& laserSprite = laser->getComponent<CSprite>();

            laserSprite.setScale(laserSprite.scaleX, laserSprite.scaleY - (3 * elapsedTime));

            if(laserSprite.scaleY < 0) {
                state = States::off;
                laser->destroy();
            }
        }
        else if(state == States::on) {
            lastOpened += elapsedTime;

            auto& laserSprite = laser->getComponent<CSprite>();

            auto theta = lastOpened / 0.04f;
            auto deltaWidth = 0.8 * sin(theta);
            deltaWidth = utility::clamp(deltaWidth, 0, deltaWidth);
            laserSprite.setScale(length, width + deltaWidth);
        }
    }

    void openLaser() {
        if(state == States::off) {
            laser = &game->manager.addEntity();
            laser->addComponent<CTransform>(sf::Vector2f(-90, 0));
            laser->addComponent<CParent>(this->entity);

            auto& laserSprite = laser->addComponent<CSprite>(game, sprite);
            laserSprite.setOrigin(laserSprite.width(), laserSprite.height()/2);
            laserSprite.setScale(length, width);


            auto& cCollision = laser->addComponent<CCollision>();
            cCollision.onCollision = [] (Entity& other) {
                if(other.hasComponent<CHealth>()) {
                    other.getComponent<CHealth>().loseHealth(5);
                }
            };

            laser->addGroup(Groups::drawable);
            laser->addGroup(Groups::collidable);
            laser->addGroup(Groups::enemy_laser);

            state = States::on;
        }
    }

    void closeLaser() {
        if(state == States::on) {
            state = States::closing;
        }
    }
};

struct CFlash : Component {
    float counter = 0;
    float end = 0.2f;

    bool flashing = false;

    void update(float elapsedTime) override {
        counter += elapsedTime;

        if(flashing) {
            if(entity->hasComponent<CSprite>()) {
                entity->getComponent<CSprite>().sprite.setColor(sf::Color::Yellow);
            }
            if(entity->hasComponent<CAnimatedSprite>()) {
                entity->getComponent<CAnimatedSprite>().sprite.setColor(sf::Color::Yellow);
            }
        }

        if(counter > end) {
            flashing = false;
            if(entity->hasComponent<CSprite>()) {
                entity->getComponent<CSprite>().sprite.setColor(sf::Color::White);
            }
            if(entity->hasComponent<CAnimatedSprite>()) {
                entity->getComponent<CAnimatedSprite>().sprite.setColor(sf::Color::White);
            }
        }
    }

    void flash() {
        flashing = true;
        counter = 0;
    }
};

#endif /* end of include guard: COMPONENTS_H_QD5OJVYS */

