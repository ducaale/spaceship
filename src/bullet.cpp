#include "bullet.h"
#include <math.h>

Bullet::Bullet() {}

Bullet::Bullet(sf::Texture *texture, sf::Vector2f coordinate, sf::Vector2f direction, float speed) {

    bullet = sf::Sprite(*texture, {204,140,70,70});
    bullet.setScale(0.4f, 0.4f);
    bullet.setOrigin(45.f, 45.f);

    nFrames = 0;
    destroyed = false;

    intialCoordinate = coordinate;

    this->speed = speed;
    this->direction = direction;
    this->coordinate = coordinate;
}

void Bullet::update(sf::Time elapsedTime) {
    if(nFrames++ == 1) {
        bullet.setScale(0.2f, 0.2f);
        bullet.setTextureRect({364,140,70,70});
    }
    else {
        velocity = direction * speed * elapsedTime.asSeconds();
        coordinate += velocity;
    }

    if(distanceTraveled() > 600) destroyed = true;
}

void Bullet::draw(sf::RenderWindow *window) {
    bullet.setPosition(coordinate);
    window->draw(bullet);
}

float Bullet::distanceTraveled() {
   return sqrt( pow((coordinate.x - intialCoordinate.x), 2) + pow((coordinate.y - intialCoordinate.y), 2) );
}
