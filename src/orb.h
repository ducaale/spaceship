#ifndef ORB_H
#define ORB_H

#include <algorithm>

#include "game.h"
#include "components.h"

#include "utility.h"

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
            gunPos.x -= 94.f;
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

    bool opening = false;
    bool closing = false;
    bool open = false;

    float initialY, farthestY, speed;

    CRLBehaviour(float initialY, float farthestY, float speed) :
        initialY(initialY), farthestY(farthestY), speed(speed) {}

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

        if(opening) {
            auto newPosition = (cTransform->position.y += speed * elapsedTime);
            cTransform->position.y = utility::clamp(newPosition, std::min(initialY, farthestY), std::max(initialY, farthestY));
            if(cTransform->position.y == farthestY) {
                opening = false;
                open = true;
            }
        }
        else if(closing) {
            auto newPosition = (cTransform->position.y -= speed * elapsedTime);
            cTransform->position.y = utility::clamp(newPosition, std::min(initialY, farthestY), std::max(initialY, farthestY));
            if(cTransform->position.y == initialY) {
                closing = false;
                open = false;
            }
        }
    }
};

struct COrbBehaviour : Component {
    CLaserGun *cLaserGun = nullptr;
    CAnimatedSprite *cSprite = nullptr;
    CTarget *cTarget = nullptr;

    enum States : std::size_t {
        none,
        normal_to_close,
        close_to_open,
        open_laser,
        close_laser,
        open_to_close,
        close_to_normal
    };

    States currentState = States::none;

    void init() override {
        cLaserGun = &entity->getComponent<CLaserGun>();
        cSprite = &entity->getComponent<CAnimatedSprite>();
        cTarget = &entity->getComponent<CTarget>();
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
                currentState = States::none;
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
                currentState = States::none;
                break;
            default:
                break;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::O)) {
            // open laser
            currentState = States::normal_to_close;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
            // close laser
            currentState = States::close_laser;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::I)) {
            cTarget->targetOn = false;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::U)) {
            cTarget->targetOn = true;
        }
    }
};

Entity& createRightArm(Game *game, Entity& parent) {
    auto& entity = game->manager.addEntity();

    float scaleX = 1.5f, scaleY = 1.5f;

    entity.addComponent<CTransform>(sf::Vector2f(0.f,-170.f));
    entity.addComponent<CParent>(&parent);

    auto& cSprite = entity.addComponent<CSprite>(game, sf::Sprite(game->resource["orb"], {384,128,128,32}));
    cSprite.setScale(scaleX, scaleY);

    entity.addComponent<CGun>(game, sf::Sprite(game->resource["orb"], {0,256,32,16}), 4.f, 300.f, Groups::enemy_bullet);
    entity.addComponent<COrbArmBehaviour>();

    entity.addGroup(Groups::drawable);
    entity.addGroup(Groups::collidable);
    entity.addGroup(Groups::enemy);

    return entity;
}

Entity& createLeftArm(Game *game, Entity& parent) {
    auto& entity = game->manager.addEntity();

    float scaleX = 1.5f, scaleY = 1.5f;

    entity.addComponent<CTransform>(sf::Vector2f(0.f,170.f));
    entity.addComponent<CParent>(&parent);

    auto& cSprite = entity.addComponent<CSprite>(game, sf::Sprite(game->resource["orb"], {256,128,128,32}));
    cSprite.setScale(scaleX, scaleY);

    entity.addComponent<CGun>(game, sf::Sprite(game->resource["orb"], {0,256,32,16}), 4.f, 300.f, Groups::enemy_bullet);
    entity.addComponent<COrbArmBehaviour>();

    entity.addGroup(Groups::drawable);
    entity.addGroup(Groups::collidable);
    entity.addGroup(Groups::enemy);

    return entity;
}

Entity& createRightRL(Game *game, Entity& parent) {
    auto& entity = game->manager.addEntity();

    float scaleX = 1.5f, scaleY = 1.5f;

    entity.addComponent<CTransform>(sf::Vector2f(0.f,-48.f));
    entity.addComponent<CParent>(&parent);

    auto& cSprite = entity.addComponent<CSprite>(game, sf::Sprite(game->resource["orb"], {0,128,32,64}));
    cSprite.setScale(scaleX, scaleY);

    entity.addComponent<CGun>(game, sf::Sprite(game->resource["orb"], {0,256,32,16}), 1.f, 200.f, Groups::enemy_bullet);
    entity.addComponent<CRLBehaviour>(-48.f, -102.f, -80.f);

    entity.addGroup(Groups::drawable);
    entity.addGroup(Groups::collidable);
    entity.addGroup(Groups::enemy);
    entity.setLayer(-1);

    return entity;
}

Entity& createLeftRL(Game *game, Entity& parent) {
    auto& entity = game->manager.addEntity();

    float scaleX = 1.5f, scaleY = 1.5f;

    entity.addComponent<CTransform>(sf::Vector2f(-1.5f,49.f));
    entity.addComponent<CParent>(&parent);

    auto& cSprite = entity.addComponent<CSprite>(game, sf::Sprite(game->resource["orb"], {128,128,32,64}));
    cSprite.setScale(scaleX, scaleY);

    entity.addComponent<CGun>(game, sf::Sprite(game->resource["orb"], {0,256,32,16}), 1.f, 200.f, Groups::enemy_bullet);
    entity.addComponent<CRLBehaviour>(49.f, 103.f, 80.f);

    entity.addGroup(Groups::drawable);
    entity.addGroup(Groups::collidable);
    entity.addGroup(Groups::enemy);
    entity.setLayer(-1);

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

    entity.addComponent<CTarget>(game, Groups::player, 17.f, 0.9f);
    entity.addComponent<CLaserGun>(game, sf::Sprite(game->resource["orb"], {0,224,512,32}));
    entity.addComponent<COrbBehaviour>();

    entity.addGroup(Groups::drawable);
    entity.addGroup(Groups::collidable);
    entity.addGroup(Groups::enemy);

    createLeftArm(game, entity);
    createRightArm(game, entity);
    createRightRL(game, entity);
    createLeftRL(game, entity);
}


#endif /* ORB_H */
