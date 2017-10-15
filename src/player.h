#ifndef PLAYER_H
#define PLAYER_H

#include <fstream>

#include "game.h"
#include "components.h"

#include "json.hpp"

using json = nlohmann::json;

struct CPlayerMovement : Component {

    CTransform *cTransform = nullptr;
    CPhysics *cPhysics = nullptr;
    CSprite *cSprite = nullptr;
    CGun *cGun = nullptr;

    float turn_speed = 0;

    CPlayerMovement(float turn_speed) : turn_speed(turn_speed) {}

    void init() override {
        cTransform = &entity->getComponent<CTransform>();
        cPhysics = &entity->getComponent<CPhysics>();
        cSprite = &entity->getComponent<CSprite>();
        cGun = &entity->getComponent<CGun>();
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

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            cPhysics->accelerate(elapsedTime);
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

struct CThruster : Component {

    CAnimatedSprite* cSprite = nullptr;

    void init() override {
        cSprite = &entity->getComponent<CAnimatedSprite>();
    }

    void update(float elapsedTime) override {

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            entity->addGroup(Groups::drawable);
            //cSprite->play("thrusterAnimation");
        }
        else {
            entity->delGroup(Groups::drawable);
        }
    }
};

void createThruster(Game *game, Entity& parent) {
    auto& entity = game->manager.addEntity();

    entity.addComponent<CTransform>(sf::Vector2f(40.f, 0.f));
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

    cSprite.animations["thrusterAnimation"] = thrusterAnimation;

    cSprite.setAnimation("thrusterAnimation");

    entity.addComponent<CThruster>();

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
    entity.addComponent<CSprite>(game, sf::Sprite(game->resource["player"], {0,0,160,70}));
    entity.addComponent<CPhysics>(max_speed, 0.f, acceleration);
    entity.addComponent<CGun>(game, sf::Sprite(game->resource["guns"], {0,0,32,16}), bullets_per_second, gun_speed, Groups::player_bullet);

    auto& cSprite = entity.getComponent<CSprite>();
    cSprite.frames["right"] = std::make_tuple(sf::IntRect(0,0,160,70), sf::Vector2f(80.f, 38.f), sf::Vector2f(0.4f, 0.4f));
    cSprite.frames["up_or_down"] = std::make_tuple(sf::IntRect(160,0,160,70), sf::Vector2f(80.f, 35.f), sf::Vector2f(0.4f, 0.4f));
    cSprite.frames["left"] = std::make_tuple(sf::IntRect(320,0,160,70), sf::Vector2f(80.f, 32.f), sf::Vector2f(0.4f, 0.4f));
    cSprite.changeFrame("right");

    entity.addComponent<CPlayerMovement>(turn_speed);

    entity.addGroup(Groups::drawable);
    entity.addGroup(Groups::player);
    entity.addGroup(Groups::collidable);

    createThruster(game, entity);

    game->manager.refresh();
}


#endif /* end of include guard: PLAYER_H */
