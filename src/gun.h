#ifndef GUN_H
#define GUN_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "bullet.h"
#include "camera.h"

class Gun {
public:
    Gun();
    Gun(sf::Texture &texture, float rateOfFire, float speed);
    void fire(sf::Time elapsedTime, sf::Vector2f coordinate, sf::Vector2f direction);

    void update(sf::Time elapsedTime);
    void draw(sf::RenderWindow *window, Camera *camera);

private:
    sf::Texture *texture;

    float rateOfFire, speed;
    sf::Time lastFired;
    std::vector<Bullet> bullets;
};

#endif /* GUN_H */
