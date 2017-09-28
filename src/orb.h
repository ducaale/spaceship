#ifndef ORB_H
#define ORB_H

#include <algorithm>

#include "game.h"
#include "components.h"
#include "observer.h"
#include "events.h"

#include "utility.h"

struct COrbArmBehaviour: Component, public Observer {
    CGun *cGun = nullptr;
    CTransform *cTransform = nullptr;
    CParent *cParent = nullptr;
    CSprite *cSprite = nullptr;
    Game *game = nullptr;

    enum Side : std::size_t {
        right,
        left
    };

    Side side;

    enum States : std::size_t {
        none,
        completed,
        firing,
        not_firing
    };

    States currentState = States::none;

    COrbArmBehaviour(Game* game) : game(game) {}

    void init() override {
        cGun = &entity->getComponent<CGun>();
        cTransform = &entity->getComponent<CTransform>();
        cParent = &entity->getComponent<CParent>();
        cSprite = &entity->getComponent<CSprite>();

        side = cTransform->y() < 0 ? Side::right : Side::left;
    }

    void nextAction(float elapsedTime) {
        counter += elapsedTime;

        if(counter > sleep && next) {
            currentState = States::none;

            // only one arm should notify the ai
            if(side == Side::left) return;

            next();
        }

    }

    void update(float elapsedTime) override {
        switch(currentState) {
            case States::firing:
                target(elapsedTime);
                fire();
                nextAction(elapsedTime);
                break;

            case States::not_firing:
                nextAction(elapsedTime);
                break;

            default:
                reposition(elapsedTime);
                break;
        }

    }

    void reposition(float elapsedTime) {
        float turn_speed = 15.f;
        float normal_position = side == Side::left ? -15 : 15;

        if(std::fabs(cTransform->angle - normal_position) > 1) {
            cTransform->angle += side == Side::right ? (turn_speed * elapsedTime) : (-turn_speed * elapsedTime);
        }
        else if(std::fabs(cTransform->angle - normal_position) < 1) {
            cTransform->angle += side == Side::left ? (turn_speed * elapsedTime) : (-turn_speed * elapsedTime);
        }
    }

    void target(float elapsedTime) {
        float turn_speed = 7.f;
        auto orbPos = game->manager.getByGroup(Groups::orb_body)->getComponent<CTransform>().position;
        auto playerPos = game->manager.getByGroup(Groups::player)->getComponent<CTransform>().position;

        float target_angle = 90 - (atan2(utility::magnitude(playerPos, orbPos), cTransform->y()) * 180 / PI);

        if(std::fabs(cTransform->angle - target_angle) > 1) {
            cTransform->angle += side == Side::left ? (turn_speed * elapsedTime) : (-turn_speed * elapsedTime);
        }
        else if(std::fabs(cTransform->angle - target_angle) < 1) {
            cTransform->angle += side == Side::right ? (turn_speed * elapsedTime) : (-turn_speed * elapsedTime);
        }
    }

    void fire() {
        sf::Transform t = cParent->getTransform();

        sf::Vector2f gunPos = cTransform->position;

        gunPos = cSprite->getTransform().getInverse().transformPoint(gunPos);
        gunPos.x -= 94.f;
        gunPos.y += (cGun->projShot % 2 ?  -5 : 5);
        gunPos = cSprite->getTransform().transformPoint(gunPos);

        sf::Vector2f globalPosition = t.transformPoint(gunPos);

        float globalAngle = atan2(t.getMatrix()[1], t.getMatrix()[0]) * 180 / PI;
        globalAngle += cTransform->angle;

        cGun->fire(globalPosition, globalAngle);
    }

    void onNotify(Events event, float sleep, std::function<void()> next) override {
        this->next = next;
        this->sleep = sleep;
        this->counter = 0;

        switch(event) {
            case Events::orb_start_firing:
                currentState = States::firing;
                break;
            case Events::orb_end_firing:
                currentState = States::not_firing;
                break;
            default:
                break;
        }
    }
};

struct CRLBehaviour: Component, public Observer {
    CTransform *cTransform = nullptr;
    CParent *cParent = nullptr;
    CGun *cGun = nullptr;

    float initialY, farthestY, speed;

    enum Side : std::size_t {
        right,
        left
    };

    Side side;

    enum States : std::size_t {
        opening,
        closing,
        open,
        close
    };

    States currentState = States::close;

    bool completed = false;

    CRLBehaviour(float initialY, float farthestY, float speed) :
        initialY(initialY), farthestY(farthestY), speed(speed) {}

    void init() override {
        cTransform = &entity->getComponent<CTransform>();
        cParent = &entity->getComponent<CParent>();
        cGun = &entity->getComponent<CGun>();

        side = cTransform->y() < 0 ? Side::right : Side::left;
    }

    void update(float elapsedTime) override {
        switch(currentState) {
            case States::opening:
                openGate(elapsedTime);
                break;
            case States::closing:
                closeGate(elapsedTime);
                break;
            case States::open:
                fire();
                break;
            default:
                break;
        }

        if(completed) {
            nextAction(elapsedTime);
        }

    }

    void openGate(float elapsedTime) {
        auto newPosition = (cTransform->position.y += speed * elapsedTime);
        cTransform->position.y = utility::clamp(newPosition, std::min(initialY, farthestY), std::max(initialY, farthestY));
        if(cTransform->position.y == farthestY) {
            currentState = States::open;
            completed = true;
        }
    }

    void closeGate(float elapsedTime) {
        auto newPosition = (cTransform->position.y -= speed * elapsedTime);
        cTransform->position.y = utility::clamp(newPosition, std::min(initialY, farthestY), std::max(initialY, farthestY));
        if(cTransform->position.y == initialY) {
            currentState = States::close;
            completed = true;
        }
    }

    void fire() {
        sf::Transform t = cParent->getTransform();
        sf::Vector2f gunPos = cTransform->position;

        if(farthestY > 0) {
            gunPos.y += (cGun->projShot % 2 ?  5 : 15);
        } else {
            gunPos.y += (cGun->projShot % 2 ?  -5 : -15);
        }

        gunPos.x -= 16.f;
        sf::Vector2f globalPosition = t.transformPoint(gunPos);
        float globalAngle = atan2(t.getMatrix()[1], t.getMatrix()[0]) * 180 / PI;

        cGun->fire(globalPosition, globalAngle, Groups::player);
    }

    void nextAction(float elapsedTime) {
        if(side == Side::left) return;

        counter += elapsedTime;
        if(counter > sleep && next) {
            completed = false;
            next();
        }
    }

    void onNotify(Events event, float sleep, std::function<void()> next) override {
        this->next = next;
        this->sleep = sleep;
        this->counter = 0;

        switch(event) {
            case Events::orb_start_lunching_rockets:
                currentState = States::opening;
                break;
            case Events::orb_end_lunching_rockets:
                currentState = States::closing;
                break;
            default:
                break;
        }
    }
};

struct COrbBehaviour : Component, public Observer {
    CLaserGun *cLaserGun = nullptr;
    CAnimatedSprite *cSprite = nullptr;
    CTarget *cTarget = nullptr;

    enum States : std::size_t {
        none,
        completed,
        normal_to_close,
        close_to_open,
        open_laser,
        close_laser,
        open_to_close,
        close_to_normal,
        start_targeting,
        end_targeting
    };

    States currentState = States::none;

    void init() override {
        cLaserGun = &entity->getComponent<CLaserGun>();
        cSprite = &entity->getComponent<CAnimatedSprite>();
        cTarget = &entity->getComponent<CTarget>();
    }

    void nextAction(float elapsedTime) {
        counter += elapsedTime;

        if(counter > sleep && next) {
            currentState = States::none;
            next();
        }

    }

    void update(float elapsedTime) override {

        switch(currentState) {

            case States::normal_to_close:
                cSprite->sprite.stop();
                cSprite->play("normal_to_close", [&]() { currentState = States::close_to_open; });
                currentState = States::none;
                break;

            case States::close_to_open:
                cSprite->sprite.stop();
                cSprite->play("close_to_open", [&]() { currentState = States::open_laser; });
                currentState = States::none;
                break;

            case States::open_laser:
                cLaserGun->openLaser();
                currentState = States::completed;
                break;

            case States::close_laser:
                cLaserGun->closeLaser();
                currentState = States::open_to_close;
                break;

            case States::open_to_close:
                cSprite->sprite.stop();
                cSprite->play("open_to_close", [&]() { currentState = States::close_to_normal; });
                currentState = States::none;
                break;

            case States::close_to_normal:
                cSprite->sprite.stop();
                cSprite->play("close_to_normal");
                currentState = States::completed;
                break;

            case States::completed:
                nextAction(elapsedTime);
                break;

            case States::start_targeting:
                enableTargeting();
                currentState = States::completed;
                break;

            case States::end_targeting:
                disableTargeting();
                currentState = States::completed;
                break;

            default:
                break;
        }
    }

    void onNotify(Events event, float sleep, std::function<void()> next) override {
        this->next = next;
        this->sleep = sleep;
        this->counter = 0;

        switch(event) {
            case Events::orb_open_laser:
                openLaser();
                break;
            case Events::orb_close_laser:
                closeLaser();
                break;
            case Events::orb_start_targeting:
                currentState = States::start_targeting;
                break;
            case Events::orb_end_targeting:
                currentState = States::end_targeting;
                break;
            default:
                break;
        }
    }

    void openLaser() {
        currentState = States::normal_to_close;
    }

    void closeLaser() {
        currentState = States::close_laser;
    }

    void enableTargeting() {
        cTarget->enableTargeting();
    }

    void  disableTargeting() {
        cTarget->disableTargeting();
    }
};

Entity& createRightArm(Game *game, Entity& parent) {
    auto& entity = game->manager.addEntity();

    float scaleX = 1.5f, scaleY = 1.5f;

    entity.addComponent<CTransform>(sf::Vector2f(0.f,-200.f), 15);
    entity.addComponent<CParent>(&parent);

    auto& cSprite = entity.addComponent<CSprite>(game, sf::Sprite(game->resource["orb"], {384,128,128,32}));
    cSprite.setScale(scaleX, scaleY);

    auto& cFlash = entity.addComponent<CFlash>();

    auto& cCollision = entity.addComponent<CCollision>();
    cCollision.onCollision = [&cFlash](Entity& e) { cFlash.flash(); };

    entity.addComponent<CGun>(game, sf::Sprite(game->resource["guns"], {0,0,32,16}), 4.f, 300.f, Groups::enemy_bullet);
    auto& orbBehaviour = entity.addComponent<COrbArmBehaviour>(game);

    entity.addGroup(Groups::drawable);
    entity.addGroup(Groups::collidable);
    entity.addGroup(Groups::orb);
    entity.addGroup(Groups::enemy);

    game->ai.addObserver(&orbBehaviour);

    return entity;
}

Entity& createLeftArm(Game *game, Entity& parent) {
    auto& entity = game->manager.addEntity();

    float scaleX = 1.5f, scaleY = 1.5f;

    entity.addComponent<CTransform>(sf::Vector2f(0.f,200.f), -15);
    entity.addComponent<CParent>(&parent);

    auto& cSprite = entity.addComponent<CSprite>(game, sf::Sprite(game->resource["orb"], {256,128,128,32}));
    cSprite.setScale(scaleX, scaleY);

    auto& cFlash = entity.addComponent<CFlash>();

    auto& cCollision = entity.addComponent<CCollision>();
    cCollision.onCollision = [&cFlash](Entity& e) { cFlash.flash(); };

    entity.addComponent<CGun>(game, sf::Sprite(game->resource["guns"], {0,0,32,16}), 4.f, 300.f, Groups::enemy_bullet);
    auto& orbBehaviour = entity.addComponent<COrbArmBehaviour>(game);

    entity.addGroup(Groups::drawable);
    entity.addGroup(Groups::collidable);
    entity.addGroup(Groups::orb);
    entity.addGroup(Groups::enemy);

    game->ai.addObserver(&orbBehaviour);

    return entity;
}

Entity& createRightRL(Game *game, Entity& parent) {
    auto& entity = game->manager.addEntity();

    float scaleX = 1.5f, scaleY = 1.5f;

    entity.addComponent<CTransform>(sf::Vector2f(0.f,-48.f));
    entity.addComponent<CParent>(&parent);

    auto& cSprite = entity.addComponent<CSprite>(game, sf::Sprite(game->resource["orb"], {0,128,32,64}));
    cSprite.setScale(scaleX, scaleY);

    entity.addComponent<CGun>(game, sf::Sprite(game->resource["guns"], {0,128,32,16}), 1.f, 200.f, Groups::enemy_bullet);
    auto& behaviour = entity.addComponent<CRLBehaviour>(-48.f, -102.f, -80.f);

    entity.addGroup(Groups::drawable);
    entity.addGroup(Groups::collidable);
    entity.addGroup(Groups::orb);
    entity.addGroup(Groups::enemy);
    entity.setLayer(-1);

    game->ai.addObserver(&behaviour);

    return entity;
}

Entity& createLeftRL(Game *game, Entity& parent) {
    auto& entity = game->manager.addEntity();

    float scaleX = 1.5f, scaleY = 1.5f;

    entity.addComponent<CTransform>(sf::Vector2f(-1.5f,49.f));
    entity.addComponent<CParent>(&parent);

    auto& cSprite = entity.addComponent<CSprite>(game, sf::Sprite(game->resource["orb"], {128,128,32,64}));
    cSprite.setScale(scaleX, scaleY);

    entity.addComponent<CGun>(game, sf::Sprite(game->resource["guns"], {0,128,32,16}), 1.f, 200.f, Groups::enemy_bullet);
    auto& behaviour = entity.addComponent<CRLBehaviour>(49.f, 103.f, 80.f);

    entity.addGroup(Groups::drawable);
    entity.addGroup(Groups::collidable);
    entity.addGroup(Groups::orb);
    entity.addGroup(Groups::enemy);
    entity.setLayer(-1);

    game->ai.addObserver(&behaviour);

    return entity;
}

void createOrb(Game *game) {
    auto& entity = game->manager.addEntity();

    float width = 128.f, height = 128.f;
    float scaleX = 1.5f, scaleY = 1.5f;

    entity.addComponent<CTransform>(sf::Vector2f(200.f,200.f));
    entity.addComponent<CAnimatedSprite>(game, AnimatedSprite(sf::seconds(0.4), true, false), width/2, height/2);

    Animation close_to_open, open_to_close, close_to_normal, normal_to_close;

    close_to_open.setSpriteSheet(game->resource["orb"]);
    open_to_close.setSpriteSheet(game->resource["orb"]);
    close_to_normal.setSpriteSheet(game->resource["orb"]);
    normal_to_close.setSpriteSheet(game->resource["orb"]);

    for(int i = 0; i < 4; i++)  close_to_open.addFrame(sf::IntRect(width * i, 0, width, height));
    for(int i = 3; i > -1; i--) open_to_close.addFrame(sf::IntRect(width * i, 0, width, height));
    for(int i = 0; i < 3; i++) open_to_close.addFrame(sf::IntRect(width * 0, 0, width, height));
    for(int i = 0; i < 3; i++)  close_to_normal.addFrame(sf::IntRect(width * i, 0, width, height));
    for(int i = 2; i > -1; i--) normal_to_close.addFrame(sf::IntRect(width * i, 0, width, height));

    auto& sprite = entity.getComponent<CAnimatedSprite>();
    sprite.setScale(scaleX, scaleY);

    sprite.animations["close_to_open"] = close_to_open;
    sprite.animations["open_to_close"] = open_to_close;
    sprite.animations["close_to_normal"] = close_to_normal;
    sprite.animations["normal_to_close"] = normal_to_close;
    sprite.setAnimation("normal_to_close");

    auto& cFlash = entity.addComponent<CFlash>();

    auto& cCollision = entity.addComponent<CCollision>();
    cCollision.onCollision = [&cFlash](Entity& e) { cFlash.flash(); };

    entity.addComponent<CTarget>(game, Groups::player, 35.f, 0.99f);
    entity.addComponent<CLaserGun>(game, sf::Sprite(game->resource["guns"], {0,128,512,32}));
    auto& orbBehaviour = entity.addComponent<COrbBehaviour>();

    entity.addGroup(Groups::drawable);
    entity.addGroup(Groups::collidable);
    entity.addGroup(Groups::orb);
    entity.addGroup(Groups::orb_body);
    entity.addGroup(Groups::enemy);

    game->ai.addObserver(&orbBehaviour);

    createLeftArm(game, entity);
    createRightArm(game, entity);
    createRightRL(game, entity);
    createLeftRL(game, entity);
}


#endif /* ORB_H */
