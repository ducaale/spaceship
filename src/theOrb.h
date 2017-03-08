#ifndef THEORB_H
#define THEORB_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class Spaceship;
class Camera;

class TheOrb {
public:
    TheOrb(sf::RenderWindow &window, Spaceship &player);

    void update(sf::Time &elapsedTime);
    void draw(Camera &camera);

    sf::Vector2f getCoordinate();
private:
    sf::Sprite body;
    sf::Sprite arm1;
    sf::Sprite arm2;

    sf::Transform rotation;
    sf::Transform translation;

    sf::Vector2f arm1_coor, arm2_coor;

    Spaceship &player;

    sf::RenderWindow &window;

    sf::Texture texture;

    //std::vector<sf::Rect> hitpoints;

    sf::Vector2f direction;
    sf::Vector2f coordinate;
    sf::Vector2f velocity;
};

#endif /* THEORB_H */
