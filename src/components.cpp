#include "components.h"

sf::Transform CParent::getTransform(bool origin, bool scale) {
    if(parent->hasComponent<CSprite>()) {
        return parent->getComponent<CSprite>().getTransform(origin, scale);
    }
    else {
        return parent->getComponent<CAnimatedSprite>().getTransform(origin, scale);
    }
}

