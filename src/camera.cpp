#include "camera.h"
#include "spaceship.h"
#include "utility.h"

Camera::Camera(Spaceship *spaceship):
    coordinate(spaceship->getCoordinate()),
    offset(0.f, 0.f),
    spaceship(spaceship),
    factor(2.f)
{}

void Camera::update(sf::Time elapsedTime) {

    offset = spaceship->getDirection() * 150.f;
    coordinate = utility::lerp(0.02, coordinate, 
            spaceship->getCoordinate() + offset + (spaceship->getVelocity() * 80.f));
}


sf::Vector2f Camera::getCoordinate() { return coordinate - sf::Vector2f(400,300); }
