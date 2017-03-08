#include "camera.h"
#include "utility.h"
#include "spaceship.h"
#include "theOrb.h"
#include <iostream>

Camera::Camera(Spaceship *spaceship, TheOrb *theOrb):
    coordinate(spaceship->getCoordinate()),
    offset(0.f, 0.f),
    spaceship(spaceship),
    theOrb(theOrb)
{}

void Camera::update(sf::Time elapsedTime) {

    if(utility::magnitude(spaceship->getCoordinate(), theOrb->getCoordinate()) < 800) {
        coordinate = utility::lerp(0.02, coordinate,
                (spaceship->getCoordinate() + theOrb->getCoordinate()) / 2.f );
    }
    else {
        offset = spaceship->getDirection() * 150.f;
        coordinate = utility::lerp(0.02, coordinate,
                spaceship->getCoordinate() + offset + (spaceship->getVelocity() * 80.f));
    }
}


sf::Vector2f Camera::getCoordinate() { return coordinate - sf::Vector2f(400,300); }
