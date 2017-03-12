#ifndef GRAPHICSCOMPONENT_H
#define GRAPHICSCOMPONENT_H

namespace sf { class RenderWindow; }
class Camera;
class TheOrb;

class GraphicsComponent {
public:
    virtual ~GraphicsComponent() {}
    virtual void update(TheOrb &theOrb, sf::RenderWindow &window, Camera &camera) = 0;
};

#endif /* GRAPHICSCOMPONENT_H */
