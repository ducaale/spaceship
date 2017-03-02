#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "AnimatedSprite.hpp"
#include "gun.h"

class Camera;
//class Gun;

class Spaceship {
public:
    Spaceship(sf::RenderWindow &window);

    void update(sf::Time elapsedTime);
    void draw(Camera &camera);

    sf::Vector2f getCoordinate();
    sf::Vector2f getDirection();
    sf::Vector2f getVelocity();
    sf::Vector2f getGunPos();
    sf::Vector2f getEnginePos();

private:
    void enableThrusters();
    void disableThrusters();

    void accelerate(sf::Time elapsedTime);
    void deccelerate(sf::Time elapsedTime);
    void steerLeft();
    void steerRight();

    void changeView();

    sf::RenderWindow &window;

    sf::Texture texture;
    sf::Sprite body;

    AnimatedSprite thruster;
    Animation thrusterAnimation;
    bool isThrusterOn;

    Gun gun;

    sf::Vector2f direction;
    sf::Vector2f coordinate;
    sf::Vector2f velocity;

    float maxSpeed, speed, acceleration;

};

#endif /* SPACESHIP_H */
