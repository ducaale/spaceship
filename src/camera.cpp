#include "camera.h"
#include "spaceship.h"

#include <iostream>
#include <algorithm>

Camera::Camera(Spaceship *spaceship):
    coordinate(spaceship->getCoordinate()),
    offset(0.f, 0.f),
    spaceship(spaceship),
    factor(2.f)
{}

void Camera::update(sf::Time elapsedTime) {
    if(offset != spaceship->getDirection() * 200.f) {
        offset += spaceship->getDirection() * 20.f;
    }
    offset *= 0.9f;

    coordinate = spaceship->getCoordinate() + offset + spaceship->getVelocity();
    coordinate -= sf::Vector2f(400,300);   // center camera
}

float Camera::clamp(float n, float lower, float upper) {
    return std::max(lower, std::min(n, upper));
}

sf::Vector2f Camera::getCoordinate() { return coordinate; }
