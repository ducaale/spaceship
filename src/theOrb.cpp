
#include "theOrb.h"
#include "utility.h"
#include "spaceship.h"

#include "graphicsComponent.h"
#include "physicsComponent.h"

TheOrb::TheOrb(PhysicsComponent* physics, GraphicsComponent* graphics, Spaceship &player) :
    physics(physics),
    graphics(graphics),
    player(player)
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
    this->physics->update(*this, elapsedTime);
}

void TheOrb::draw(sf::RenderWindow &window, Camera &camera) {
    this->graphics->update(*this, window, camera);
}

sf::Vector2f TheOrb::getCoordinate() { return coordinate; }
