#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "theOrbGraphicsComponent.h"
#include "camera.h"
#include "theOrb.h"

void TheOrbGraphicsComponent::update(TheOrb &theOrb, sf::RenderWindow &window, Camera &camera) {

    theOrb.body.setPosition(theOrb.coordinate - camera.getCoordinate());
    theOrb.arm1.setPosition(theOrb.arm1_coor - camera.getCoordinate());
    theOrb.arm2.setPosition(theOrb.arm2_coor - camera.getCoordinate());

    window.draw(theOrb.arm1);
    window.draw(theOrb.arm2);
    window.draw(theOrb.body);
}
