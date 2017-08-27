#include "camera.h"
#include "utility.h"

#include "groups.h"

Camera::Camera(Manager& manager):
    offset(0.f, 0.f)
{
    player = manager.getByGroup(Groups::player);
}

void Camera::update(sf::Time elapsedTime) {

    offset = player->getComponent<CTransform>().getDirection() * 150.f;
    coordinate = utility::lerp(0.02, coordinate,
            player->getComponent<CTransform>().position + offset + (player->getComponent<CPhysics>().velocity * 80.f));
}


sf::Vector2f Camera::getCoordinate() { return coordinate; }
