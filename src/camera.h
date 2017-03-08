#ifndef CAMERA_H
#define CAMERA_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class Spaceship;
class TheOrb;

class Camera {
public:
    Camera(Spaceship *spaceship, TheOrb *theOrb);
    void update(sf::Time elapsedTime);
    sf::Vector2f getCoordinate();
private:
    sf::Vector2f coordinate;
    sf::Vector2f offset;
    Spaceship *spaceship;
    TheOrb *theOrb;

};

#endif /* CAMERA_H */
