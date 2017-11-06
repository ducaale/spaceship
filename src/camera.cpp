#include <algorithm>

#include "camera.h"
#include "utility.h"

#include "components.h"
#include "groups.h"

Camera::Camera(Manager& manager, sf::RenderWindow& w):
    manager(manager),
    window(w)
{}

void Camera::update(float elapsedTime) {

    auto player = manager.getByGroup(Groups::player);
    auto& enemies = manager.getEntitiesByGroup(Groups::enemy);

    auto& player_coor = player->getComponent<CTransform>().position;
    auto player_direction = player->getComponent<CTransform>().getDirection();
    auto player_vel = player->getComponent<CPhysics>().getSpeedPercentage();
    auto& enemy_coor = enemies[0]->getComponent<CTransform>().position;

    if(utility::distance(player_coor, enemy_coor) > 1500) {
        offset = (player_direction * 150.f * (player_vel * 2.5f));
        coordinate = utility::lerp(0.02, coordinate, player_coor + offset);
        scale = utility::lerp(0.02, scale, {800.f, 600.f});
    } else {
        coordinate = utility::lerp(0.02, coordinate, (player_coor + enemy_coor) * (1/2.f));
        scale = utility::lerp(0.02, scale, {800.f * 2.0, 600.f * 2.0});
    }

    view.setCenter(coordinate);
    view.setSize(scale);
    window.setView(view);
}
