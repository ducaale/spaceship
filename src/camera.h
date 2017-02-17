#ifndef CAMERA_H
#define CAMERA_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class Spaceship;

class Camera {
public:
    Camera(Spaceship *spaceship);
    void update(sf::Time elapsedTime);
    sf::Vector2f getCoordinate();
    float clamp(float n, float lower, float upper);
private:
    sf::Vector2f coordinate;
    sf::Vector2f offset;
    Spaceship *spaceship;

    const float factor;
};

#endif /* CAMERA_H */
