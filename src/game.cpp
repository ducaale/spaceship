#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "spaceship.h"
#include "camera.h"

//constexpr float ftStep = 1.f;
//constexpr float ftSlice = 1.f;

int main() {
    sf::RenderWindow window({800,600}, "Spaceship");
    window.setFramerateLimit(50);

    Spaceship spaceship(window);
    Camera camera(&spaceship);

    sf::Clock clock;
    sf::Time elapsedTime;
    //float currentSlice = 0.f;

    sf::Event event;
    while(window.isOpen()) {
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
            }
        }

        elapsedTime = clock.restart();

        window.clear({0,24,72});

        spaceship.update(elapsedTime);
        camera.update(elapsedTime);

        spaceship.draw(camera);
        window.display();

    }

    return 0;
}
