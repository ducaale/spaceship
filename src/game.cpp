#include <iostream>

#include "game.h"

#include "components.h"
#include "collision.h"
#include "groups.h"

#include "camera.h"

#include "player.h"
#include "orb.h"

void Game::loadResources() {
    sf::Texture *texture;

    texture = &resource["orb"];
    if(!texture->loadFromFile("../spritesheet/the_orb.png")) {
        std::cout << "unable to load file" << std::endl;
    }

    texture  = &resource["player"];
    if(!texture->loadFromFile("../spritesheet/player_ship.png")) {
        std::cout << "unable to load file" << std::endl;
    }
}

Game::Game() {

    loadResources();

    createPlayer(this);
    createOrb(this);

    camera = new Camera(manager);
}

void Game::run() {
    while(window.isOpen()) {
        elapsedTime = clock.restart();

        window.clear(sf::Color::Black);

        inputPhase();
        updatePhase();
        drawPhase();

    }
}

void Game::inputPhase() {
    while(window.pollEvent(event)) {
        if(event.type == sf::Event::Closed) {
            window.close();
        }
    }
}

void Game::updatePhase() {
    simulationTime = sf::seconds(0.f);
    for(int i = 0; simulationTime <= elapsedTime && i < 10; simulationTime += timeSlice, i++) {
        camera->update(timeSlice);
        manager.refresh();
        manager.update(timeSlice.asSeconds());
    }
}

void Game::drawPhase() {
    view.setCenter(camera->getCoordinate());
    window.setView(view);
    manager.draw();
    window.display();
}

void Game::render(const sf::Drawable& drawable, const sf::Transform& t) {
    window.draw(drawable, t);
}

int main() {
    Game().run();
    return 0;
}
