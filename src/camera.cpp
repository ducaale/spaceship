#include "camera.h"
#include "utility.h"

#include "components.h"
#include "groups.h"

Camera::Camera(Manager& manager, sf::RenderWindow& w):
    window(w)
{
    player = manager.getByGroup(Groups::player);
}

void Camera::update(float elapsedTime) {

    offset = player->getComponent<CTransform>().getDirection() * 150.f;
    coordinate = utility::lerp(0.02, coordinate,
            player->getComponent<CTransform>().position + offset + (player->getComponent<CPhysics>().velocity * 80.f));

    view.setCenter(coordinate);
    window.setView(view);
}
