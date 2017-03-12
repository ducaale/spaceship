#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "theOrbPhysicsComponent.h"
#include "theOrb.h"
#include "camera.h"
#include "spaceship.h"
#include "utility.h"

void TheOrbPhysicsComponent::update(TheOrb &theOrb, sf::Time elapsedTime) {

    float angle = atan2(theOrb.coordinate.y - theOrb.player.getCoordinate().y,
            theOrb.coordinate.x - theOrb.player.getCoordinate().x) * 180 / 3.141;
    theOrb.body.rotate((angle - theOrb.body.getRotation()));

    theOrb.arm1_coor = utility::rotatePoint(theOrb.arm1_coor, theOrb.coordinate, angle - theOrb.arm1.getRotation());
    theOrb.arm2_coor = utility::rotatePoint(theOrb.arm2_coor, theOrb.coordinate, angle - theOrb.arm2.getRotation());

    theOrb.arm1.rotate((angle - theOrb.arm1.getRotation()));
    theOrb.arm2.rotate((angle - theOrb.arm2.getRotation()));
}
