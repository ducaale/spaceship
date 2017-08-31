#ifndef GAME_H
#define GAME_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "manager.h"
#include <map>

class Camera;

class Game {
public:
    Game();

    void run();

    void updatePhase();
    void inputPhase();
    void drawPhase();

    void render(const sf::Drawable& drawable, const sf::Transform& t = sf::Transform::Identity);

    void loadResources();

    Camera *camera = nullptr;
    Manager manager;

    std::map<std::string, sf::Texture> resource;

private:
    sf::RenderWindow window{{800,600}, "Spaceship"};
    sf::View view{{0, 0,800, 600}};

    sf::Clock clock;
    sf::Time elapsedTime;
    sf::Time simulationTime;
    sf::Time timeSlice = sf::milliseconds(16.f);

    sf::Event event;
};

#endif /* GAME_H */
