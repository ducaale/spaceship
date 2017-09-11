#include <iostream>

#include "game.h"

#include "camera.h"
#include "components.h"
#include "collision.h"
#include "groups.h"

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

    ai.init();

    camera = new Camera(manager, window);
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
        manager.refresh();
        camera->update(timeSlice.asSeconds());
        manager.update(timeSlice.asSeconds());
        checkCollision();
    }
}

void Game::drawPhase() {
    manager.draw();
    window.display();
}

void Game::render(const sf::Drawable& drawable, const sf::Transform& t) {
    window.draw(drawable, t);
}

void Game::checkCollision() {
    auto& collidables = manager.getEntitiesByGroup(Groups::collidable);

    for(std::size_t i = 0; i < collidables.size() - 1; i++) {
        for(std::size_t j = i + 1; j < collidables.size(); j++) {

            if(collidables[i]->hasGroup(Groups::enemy_bullet) && collidables[j]->hasGroup(Groups::enemy)) continue;
            if(collidables[i]->hasGroup(Groups::enemy) && collidables[j]->hasGroup(Groups::enemy_bullet)) continue;

            if(collidables[i]->hasGroup(Groups::player_bullet) && collidables[j]->hasGroup(Groups::player)) continue;
            if(collidables[i]->hasGroup(Groups::player) && collidables[j]->hasGroup(Groups::player_bullet)) continue;

            if(test_collision(*collidables[i], *collidables[j])) {
                if(collidables[i]->hasGroup(Groups::bullet)) collidables[i]->destroy();
                if(collidables[j]->hasGroup(Groups::bullet)) collidables[j]->destroy();
            }
        }
    }
}

int main() {
    Game().run();
    return 0;
}
