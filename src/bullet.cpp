#include "bullet.h"
#include "camera.h"
#include <math.h>

Bullet::Bullet() {}

Bullet::Bullet(sf::Texture *texture, sf::Vector2f coordinate, sf::Vector2f direction, float speed) {

    bullet = sf::Sprite(*texture, {204,140,70,70});
    bullet.setScale(0.3f, 0.3f);
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

void Bullet::draw(sf::RenderWindow *window, Camera *camera) {
    bullet.setPosition(coordinate - camera->getCoordinate());
    window->draw(bullet);
}

float Bullet::distanceTraveled() {
   return sqrt( pow((coordinate.x - intialCoordinate.x), 2) + pow((coordinate.y - intialCoordinate.y), 2) );
}
