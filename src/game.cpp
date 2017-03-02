#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "spaceship.h"
#include "theOrb.h"
#include "camera.h"


int main() {
    sf::RenderWindow window({800,600}, "Spaceship");

    Spaceship spaceship(window);
    TheOrb theOrb(window, spaceship);
    Camera camera(&spaceship);

    sf::Texture texture;
    texture.loadFromFile("../background/nebula.png");
    sf::Sprite background(texture);
    background.setScale(1/1.28f, 1/1.28f);

    sf::Clock clock;
    sf::Time elapsedTime;
    sf::Time simulationTime;
    sf::Time timeSlice = sf::milliseconds(16.f);

    sf::Event event;
    while(window.isOpen()) {
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
            }
        }

        elapsedTime = clock.restart();

        window.clear({0,24,72});

        simulationTime = sf::seconds(0.f);
        for(; simulationTime <= elapsedTime; simulationTime += timeSlice) {
            spaceship.update(timeSlice);
            theOrb.update(timeSlice);
            camera.update(timeSlice);
        }

        window.draw(background);
        theOrb.draw(camera);
        spaceship.draw(camera);
        window.display();

    }

    return 0;
}
