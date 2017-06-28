#include "spaceship.h"
#include "camera.h"

#include <iostream>

#define PI 3.14159265

Spaceship::Spaceship(sf::RenderWindow &w):
    window(w),
    direction{1, 0},
    coordinate{0,0},
    velocity{0, 0},
    maxSpeed(300.f),
    speed(0.f),
    acceleration(300),
    isThrusterOn(false)
{
    texture.loadFromFile("../spritesheet/player_ship.png");
    body = sf::Sprite(texture, {0,0,160,70});
    body.setScale(0.4f, 0.4f);
    body.setOrigin(80.f, 38.f);

    gun = Gun(texture, 5, 400.f);

    thrusterAnimation.setSpriteSheet(texture);
    thrusterAnimation.addFrame({0,70,160,70});
    thrusterAnimation.addFrame({160,70,160,70});

    thruster = AnimatedSprite(sf::seconds(0.09), true, false);
    thruster.setAnimation(thrusterAnimation);

    thruster.setScale(0.2f, 0.2f);
    thruster.setPosition(body.getPosition());
    thruster.setOrigin(80.f, 36.f);
}

void Spaceship::update(sf::Time elapsedTime) {
    handleInput(elapsedTime);
    coordinate += velocity;
    velocity = direction * speed * elapsedTime.asSeconds();

    thruster.update(elapsedTime);
    gun.update(elapsedTime);
}

void Spaceship::draw(Camera &camera){

    body.setPosition(coordinate - camera.getCoordinate());

    thruster.setPosition(getEnginePos());
    thruster.setRotation(body.getRotation());

    window.draw(body);
    gun.draw(&window, &camera);

    if(isThrusterOn)
        window.draw(thruster);
}

void Spaceship::handleInput(sf::Time elapsedTime) {
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
        gun.fire(elapsedTime, getGunPos(), getDirection());
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        steerLeft();
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        steerRight();
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        accelerate(elapsedTime);
    }
    else {
        deccelerate(elapsedTime);
    }
}

void Spaceship::accelerate(sf::Time elapsedTime) {
    enableThrusters();
    if(speed < maxSpeed) {
        speed += acceleration * elapsedTime.asSeconds();
    }
}
void Spaceship::deccelerate(sf::Time elapsedTime) {
    disableThrusters();
    if(speed > 0.f)
        speed -= acceleration * elapsedTime.asSeconds();
    else
        speed = 0.f;
}

void Spaceship::steerLeft() {
    body.rotate(-5);
    direction.x = cos(body.getRotation() * PI / 180);
    direction.y = sin(body.getRotation() * PI / 180);
    changeView();
}

void Spaceship::steerRight() {
    body.rotate(5);
    direction.x = cos(body.getRotation() * PI / 180);
    direction.y = sin(body.getRotation() * PI / 180);
    changeView();
}

void Spaceship::enableThrusters() {
    isThrusterOn = true;
    thruster.play(thrusterAnimation);
}

void Spaceship::disableThrusters() {
    isThrusterOn = false;
    thruster.stop();
}

void Spaceship::changeView() {
    float angle = body.getRotation();  // NOTE: angle increases clockwise

    if(angle < 45 || angle > 315) {
        body.setTextureRect({0,0,160,70});
        body.setOrigin(80.f, 38.f);
    }
    else if(angle > 135 && angle < 225) {
        body.setTextureRect({320,0,160,70});
        body.setOrigin(80.f, 32.f);
    }
    else {
        body.setTextureRect({160,0,160,70});
        body.setOrigin(80.f, 35.f);
    }
}

sf::Vector2f Spaceship::getEnginePos() {
    if(body.getTextureRect() == sf::IntRect{160,0,160,70}) {
        return body.getPosition() - ( getDirection() * 44.f );
    }
    else {
        sf::Transform translation;
        translation.rotate(90);
        sf::Vector2f newPoint = translation.transformPoint(getDirection()) * 2.f;
        return body.getPosition() - (newPoint + ( getDirection() * 44.f ));
    }
}

sf::Vector2f Spaceship::getGunPos() {
    float shipLength = (body.getTextureRect() == sf::IntRect{160, 0, 160, 70}) ? 26.f : 32.f;

    return getCoordinate() + (getDirection() * shipLength);
}

sf::Vector2f Spaceship::getCoordinate() { return coordinate; }
sf::Vector2f Spaceship::getDirection() { return direction; }
sf::Vector2f Spaceship::getVelocity() { return velocity; }
