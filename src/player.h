#ifndef PLAYER_H
#define PLAYER_H

#include <fstream>

#include "game.h"
#include "components.h"

#include "json.hpp"

using json = nlohmann::json;

struct CThruster : Component {

    Game* game = nullptr;
    CAnimatedSprite* cSprite = nullptr;
    CHealth* cHealth = nullptr;
    bool boosterOn = false;

    CThruster(Game* game) : game(game) {}

    void init() override {
        cSprite = &entity->getComponent<CAnimatedSprite>();
        cHealth = &game->manager.getByGroup(Groups::player)->getComponent<CHealth>();
    }

    void update(float elapsedTime) override {

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || boosterOn) {
            entity->addGroup(Groups::drawable);

            if(boosterOn) {
                cSprite->setScale(0.35f, 0.45f);
            }
            else {
                cSprite->setScale(0.15f, 0.25f);
            }
        }
        else {
            entity->delGroup(Groups::drawable);
        }

        if(!cHealth->isAlive()) entity->delGroup(Groups::drawable);
    }
};

struct CPlayerBoost : Component {
    Game *game = nullptr;
    CPhysics *cPhysics = nullptr;
    float normal_acceleration;
    float normal_max_speed;

    bool boosterOn = false;
    float boostDuration = 1.f;
    float boostRecharge = 3.f;
    float lastBoost = boostRecharge;

    CPlayerBoost(Game *game) : game(game) {}

    void init() override {
        cPhysics = &entity->getComponent<CPhysics>();
        normal_acceleration = cPhysics->acceleration;
        normal_max_speed = cPhysics->maxSpeed;
    }

    void update(float elapsedTime) {
        lastBoost += elapsedTime;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
            if(lastBoost > boostRecharge) {
                lastBoost = 0;
                boosterOn = true;
            }
        }

        if(lastBoost > boostDuration) {
            boosterOn = false;
        }

        if(boosterOn) {
            game->manager.getByGroup(Groups::thruster)->getComponent<CThruster>().boosterOn = true;
            cPhysics->acceleration = 400.f;
            cPhysics->maxSpeed = 400.f;
        }
        else {
            game->manager.getByGroup(Groups::thruster)->getComponent<CThruster>().boosterOn = false;
            cPhysics->acceleration = normal_acceleration;
            cPhysics->maxSpeed = normal_max_speed;
        }
    }
};

struct CPlayerMovement : Component {

    CTransform *cTransform = nullptr;
    CPhysics *cPhysics = nullptr;
    CSprite *cSprite = nullptr;
    CGun *cGun = nullptr;
    CHealth *cHealth = nullptr;
    CPlayerBoost *cPlayerBoost = nullptr;

    float turn_speed = 0;

    CPlayerMovement(float turn_speed) : turn_speed(turn_speed) {}

    void init() override {
        cTransform = &entity->getComponent<CTransform>();
        cPhysics = &entity->getComponent<CPhysics>();
        cSprite = &entity->getComponent<CSprite>();
        cGun = &entity->getComponent<CGun>();
        cHealth = &entity->getComponent<CHealth>();
        cPlayerBoost = &entity->getComponent<CPlayerBoost>();
    }

    void changeView() {
        float angle = cSprite->sprite.getRotation();

        if(angle < 45 || angle > 315) {
            cSprite->changeFrame("left");
        }
        else if(angle > 135 && angle < 225) {
            cSprite->changeFrame("right");
        }
        else {
            cSprite->changeFrame("up_or_down");
        }
    }

    void update(float elapsedTime) override {

        if(cHealth->isAlive()) {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
                fire();
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                cTransform->angle -= (turn_speed * elapsedTime );
                changeView();
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                cTransform->angle += (turn_speed * elapsedTime );
                changeView();
            }

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || cPlayerBoost->boosterOn) {
                cPhysics->accelerate(elapsedTime);
            }
        }

        cPhysics->deccelerate(elapsedTime);
    }

    void fire() {
        sf::Transform t = cSprite->getTransform(false, true);
        sf::Vector2f gunPos(0.f, 0.f);

        gunPos.y += (cGun->projShot % 2 ?  +5 : -5);

        gunPos.x -= 150.f;
        sf::Vector2f globalPosition = t.transformPoint(gunPos);

        float globalAngle = atan2(t.getMatrix()[1], t.getMatrix()[0]) * 180 / PI;

        cGun->fire(globalPosition, globalAngle);
    }
};

struct CHUD : Component {
    Game *game = nullptr;
    CHealth *cHealth = nullptr;
    sf::Text text;

    CHUD(Game *game) : game(game) {
        text.setFont(game->font);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);
        text.setPosition(50, 20);
    }

    void init() override {
        cHealth = &entity->getComponent<CHealth>();
    }

    void update(float elapsedTime) override {
        text.setString(std::to_string(cHealth->health));
    }

    void draw() override {
        game->renderHUD(text);
    }
};

void createThruster(Game *game, Entity& parent) {
    auto& entity = game->manager.addEntity();

    entity.addComponent<CTransform>(sf::Vector2f(30.f, 0.f));
    entity.addComponent<CParent>(&parent);

    float width = 160.f;
    float height = 70.f;

    float scaleX = 0.15f;
    float scaleY = 0.25f;

    entity.addComponent<CAnimatedSprite>(game, AnimatedSprite(sf::seconds(0.09)), width/2, height/2);

    Animation thrusterAnimation;
    thrusterAnimation.setSpriteSheet(game->resource["player"]);
    thrusterAnimation.addFrame({320,70,160,70});
    thrusterAnimation.addFrame({160,70,160,70});

    auto& cSprite = entity.getComponent<CAnimatedSprite>();

    cSprite.setScale(scaleX, scaleY);
    cSprite.setOrigin(0, height/2);

    cSprite.animations["thrusterAnimation"] = thrusterAnimation;

    cSprite.setAnimation("thrusterAnimation");

    entity.addComponent<CThruster>(game);

    entity.addGroup(Groups::thruster);
}

void createPlayer(Game *game) {

    //load values from json file
    std::ifstream file("values.json");
    json values;
    file >> values;

    sf::Vector2f position = {values["player"]["initial_position"]["x"], values["player"]["initial_position"]["y"]};
    float angle = values["player"]["initial_angle"];
    float max_speed = values["player"]["max_speed"];
    float acceleration = values["player"]["acceleration"];
    float turn_speed = values["player"]["turn_speed"];
    float gun_speed = values["player"]["gun"]["speed"];
    float bullets_per_second = values["player"]["gun"]["bullets_per_second"];


    auto& entity = game->manager.addEntity();

    entity.addComponent<CTransform>(position, angle);
    entity.addComponent<CHealth>(20);
    entity.addComponent<CSprite>(game, sf::Sprite(game->resource["player"], {0,0,160,70}));
    entity.addComponent<CPhysics>(max_speed, 0.f, acceleration);
    entity.addComponent<CPlayerBoost>(game);
    entity.addComponent<CGun>(game, sf::Sprite(game->resource["guns"], {0,0,32,16}), bullets_per_second, gun_speed, Groups::player_bullet);

    auto& cSprite = entity.getComponent<CSprite>();
    cSprite.frames["right"] = std::make_tuple(sf::IntRect(0,0,160,70), sf::Vector2f(80.f, 38.f), sf::Vector2f(0.4f, 0.4f));
    cSprite.frames["up_or_down"] = std::make_tuple(sf::IntRect(160,0,160,70), sf::Vector2f(80.f, 35.f), sf::Vector2f(0.4f, 0.4f));
    cSprite.frames["left"] = std::make_tuple(sf::IntRect(320,0,160,70), sf::Vector2f(80.f, 32.f), sf::Vector2f(0.4f, 0.4f));
    cSprite.changeFrame("right");

    entity.addComponent<CExplosion>(game);
    entity.addComponent<CPlayerMovement>(turn_speed);

    entity.addComponent<CHUD>(game);

    entity.addGroup(Groups::drawable);
    entity.addGroup(Groups::player);
    entity.addGroup(Groups::collidable);

    game->manager.refresh();

    createThruster(game, entity);
}


#endif /* end of include guard: PLAYER_H */
