#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "spaceship.h"
#include "camera.h"


int main() {
    sf::RenderWindow window({800,600}, "Spaceship");

    Spaceship spaceship(window);
    Camera camera(&spaceship);

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
            camera.update(timeSlice);
        }

        spaceship.draw(camera);
        window.display();

    }

    return 0;
}
