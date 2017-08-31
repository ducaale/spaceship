#ifndef CAMERA_H
#define CAMERA_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "entity.h"
#include "manager.h"

class Camera {
public:
    Camera(Manager& manager, sf::RenderWindow& w);
    void update(float elapsedTime);

private:
    sf::RenderWindow& window;
    sf::View view{{0, 0,800, 600}};

    sf::Vector2f coordinate;
    sf::Vector2f offset;

    Entity* player = nullptr;
};

#endif /* CAMERA_H */
