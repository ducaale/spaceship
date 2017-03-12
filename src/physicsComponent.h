#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

namespace sf { class Time; }
class TheOrb;

class PhysicsComponent {
public:
    virtual ~PhysicsComponent() {};
    virtual void update(TheOrb &theOrb, sf::Time elapsedTime) = 0;
};

#endif /* PHYSICSCOMPONENT_H */
