#ifndef THEORB_H
#define THEORB_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class Spaceship;
class Camera;

class GraphicsComponent;
class PhysicsComponent;

class TheOrb {
public:
    TheOrb(PhysicsComponent* physics, GraphicsComponent* graphics, Spaceship &player);

    void update(sf::Time &elapsedTime);
    void draw(sf::RenderWindow &window, Camera &camera);

    sf::Vector2f getCoordinate();

    sf::Sprite body;
    sf::Sprite arm1;
    sf::Sprite arm2;

    Spaceship &player;

    sf::Vector2f arm1_coor, arm2_coor;
    sf::Vector2f coordinate;

private:
    sf::Texture texture;

    //std::vector<sf::Rect> hitpoints;

    sf::Vector2f direction;
    sf::Vector2f velocity;


    GraphicsComponent *graphics;
    PhysicsComponent *physics;
};

#endif /* THEORB_H */
