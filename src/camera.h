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

    sf::View view{{0, 0,800, 600}};
    sf::View HUDview{{0, 0,800, 600}};

private:
    Manager& manager;

    sf::RenderWindow& window;

    sf::Vector2f coordinate;
    sf::Vector2f scale;
    sf::Vector2f offset;
};

#endif /* CAMERA_H */
