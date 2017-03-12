#ifndef THEORBPHYSICSCOMPONENT_H
#define THEORBPHYSICSCOMPONENT_H

#include "physicsComponent.h"

class TheOrbPhysicsComponent : public PhysicsComponent
{

public:
    virtual void update(TheOrb &theOrb, sf::Time elapsedTime);
};

#endif /* THEORBPHYSICSCOMPONENT_H */
