#ifndef THEORBGRAPHICSCOMPONENT_H
#define THEORBGRAPHICSCOMPONENT_H

#include "graphicsComponent.h"

class TheOrbGraphicsComponent : public GraphicsComponent
{
public:
    virtual void update(TheOrb &theOrb, sf::RenderWindow &window, Camera &camera);
};

#endif /* THEORBGRAPHICSCOMPONENT_H */
