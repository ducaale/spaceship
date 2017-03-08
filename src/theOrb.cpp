#include "theOrb.h"
#include "utility.h"
#include "spaceship.h"
#include <iostream>

TheOrb::TheOrb(sf::RenderWindow &w, Spaceship &player) :
    player(player),
    window(w)
{
    coordinate = sf::Vector2f(900.f,0.f);
    texture.loadFromFile("../spritesheet/the_orb.png");
    body = sf::Sprite(texture, {256,0,128,128});
    body.setOrigin(64.f, 64.f);
    body.setScale(1.5f, 1.5f);

    arm1_coor = coordinate + sf::Vector2f(0,180);
    arm1 = sf::Sprite(texture, {0,384,128,32});
    arm1.setOrigin(64.f,16.f);
    arm1.setScale(1.5f, 1.5f);

    arm2_coor = coordinate + sf::Vector2f(0,-180);
    arm2 = sf::Sprite(texture, {128,384,128,32});
    arm2.setOrigin(64.f,16.f);
    arm2.setScale(1.5f, 1.5f);
}

void TheOrb::update(sf::Time &elapsedTime) {
    float angle = atan2(coordinate.y - player.getCoordinate().y,
            coordinate.x - player.getCoordinate().x) * 180 / 3.141;
    body.rotate((angle - body.getRotation()));

    arm1_coor = utility::rotatePoint(arm1_coor, coordinate, angle - arm1.getRotation());
    arm2_coor = utility::rotatePoint(arm2_coor, coordinate, angle - arm2.getRotation());

    arm1.rotate((angle - arm1.getRotation()));
    arm2.rotate((angle - arm2.getRotation()));
}

void TheOrb::draw(Camera &camera) {
    body.setPosition(coordinate - camera.getCoordinate());
    arm1.setPosition(arm1_coor - camera.getCoordinate());
    arm2.setPosition(arm2_coor - camera.getCoordinate());

    window.draw(arm1);
    window.draw(arm2);
    window.draw(body);
}

sf::Vector2f TheOrb::getCoordinate() { return coordinate; }
