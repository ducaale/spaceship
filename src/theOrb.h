#ifndef THEORB_H
#define THEORB_H

#include "spaceship.h"

class TheOrb {
public:
    TheOrb(sf::RenderWindow &window, Spaceship &player);

    void update(sf::Time &elapsedTime);
    void draw(Camera &camera);

private:
    sf::Sprite body;
    sf::Sprite arm1;
    sf::Sprite arm2;

    Spaceship &player;

    sf::RenderWindow &window;

    sf::Texture texture;

    //std::vector<sf::Rect> hitpoints;

    sf::Vector2f direction;
    sf::Vector2f coordinate;
    sf::Vector2f velocity;
};

#endif /* THEORB_H */
