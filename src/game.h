#ifndef GAME_H
#define GAME_H

#include <map>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "manager.h"

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

    Manager manager;

    std::map<std::string, sf::Texture> resource;

private:
    sf::RenderWindow window{{800,600}, "Spaceship"};

    Camera *camera = nullptr;

    sf::Clock clock;
    sf::Time elapsedTime;
    sf::Time simulationTime;
    sf::Time timeSlice = sf::milliseconds(16.f);

    sf::Event event;
};

#endif /* GAME_H */
