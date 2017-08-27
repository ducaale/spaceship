#include "components.h"

sf::Transform CParent::getTransform() {
    if(parent->hasComponent<CSprite>()) {
        return parent->getComponent<CSprite>().getTransform();
    }
    else {
        return parent->getComponent<CAnimatedSprite>().getTransform();
    }
}

