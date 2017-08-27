#ifndef CAMERA_H
#define CAMERA_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "entity.h"
#include "components.h"
#include "manager.h"

class Camera {
public:
    Camera(Manager& manager);
    void update(sf::Time elapsedTime);
    sf::Vector2f getCoordinate();
private:
    sf::Vector2f coordinate;
    sf::Vector2f offset;

    Entity* player = nullptr;

};

#endif /* CAMERA_H */
