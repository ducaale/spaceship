#include "theOrb.h"
#include <cmath>

TheOrb::TheOrb(sf::RenderWindow &w, Spaceship &player) :
    player(player),
    window(w)
{
    coordinate = sf::Vector2f(900.f,0.f);
    texture.loadFromFile("../spritesheet/the_orb.png");
    body = sf::Sprite(texture, {256,0,128,128});
    body.setOrigin(64.f, 64.f);
    body.setScale(1.5f, 1.5f);
}

void TheOrb::update(sf::Time &elapsedTime) {
    float angle = atan2(coordinate.y - player.getCoordinate().y,
            coordinate.x - player.getCoordinate().x) * 180 / 3.141;
    body.rotate((angle - body.getRotation()));
}

void TheOrb::draw(Camera &camera) {
    body.setPosition(coordinate - camera.getCoordinate());
    window.draw(body);
}
