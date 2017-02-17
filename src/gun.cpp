#include "gun.h"
#include "bullet.h"

#include <iostream>

Gun::Gun() {};

Gun::Gun(sf::Texture &texture, float rateOfFire, float speed) :
    texture(&texture),
    rateOfFire(rateOfFire),
    speed(speed)
{}

void Gun::fire(sf::Time elapsedTime, sf::Vector2f coordinate, sf::Vector2f direction) {
    lastFired += elapsedTime;

    if(lastFired.asSeconds() > 0.1f) {
        bullets.emplace_back(texture, coordinate, direction, speed);
        lastFired = sf::Time();
    }
}

void Gun::update(sf::Time elapsedTime) {
    bullets.erase(remove_if(begin(bullets), end(bullets),
                [](const Bullet& bullet) { return bullet.isDestroyed(); }),
                end(bullets));

    for(auto& bullet : bullets) {
        bullet.update(elapsedTime);
    }
}

void Gun::draw(sf::RenderWindow *window, Camera *camera) {
    for(auto& bullet : bullets)
        bullet.draw(window, camera);
}
