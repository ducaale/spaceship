#include <iostream>

#include "game.h"

#include "camera.h"
#include "collision.h"
#include "components.h"
#include "collision.h"
#include "groups.h"

#include "player.h"
#include "orb.h"
#include "rocks.h"

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

    texture  = &resource["guns"];
    if(!texture->loadFromFile("../spritesheet/guns.png")) {
        std::cout << "unable to load file" << std::endl;
    }

    texture  = &resource["rocks"];
    if(!texture->loadFromFile("../spritesheet/rocks.png")) {
        std::cout << "unable to load file" << std::endl;
    }

    if(!font.loadFromFile("../fonts/ARCADECLASSIC.ttf")) {
        std::cout << "unable to load font" << std::endl;
    }
}

Game::Game() {

    window.setFramerateLimit(60);

    loadResources();

    createPlayer(this);
    createOrb(this);
    createRocks(this);

    ai.init();

    camera = new Camera(manager, window);

    collision = new Collision(manager);
    collision->noCollision = {
        {Groups::enemy, Groups::enemy},
        {Groups::enemy, Groups::enemy_bullet},
        {Groups::enemy_bullet, Groups::enemy_bullet},
        {Groups::player, Groups::player_bullet},
        {Groups::player_bullet, Groups::player_bullet}
    };
}

void Game::run() {
    while(window.isOpen()) {
        elapsedTime = clock.restart();

        window.clear({28, 34, 61});

        inputPhase();
        updatePhase();
        drawPhase();

        f++;
        if((f % 10) == 0) {
            window.setTitle(std::to_string(std::round(1.f / elapsedTime.asSeconds())));
        }
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
    for(int i = 0; simulationTime <= elapsedTime && i < 5; simulationTime += timeSlice, i++) {
        manager.refresh();
        camera->update(timeSlice.asSeconds());
        manager.update(timeSlice.asSeconds());
        collision->checkCollision();
    }
}

void Game::drawPhase() {
    manager.draw();
    window.display();
}

void Game::render(const sf::Drawable& drawable, const sf::Transform& t) {
    window.draw(drawable, t);
}

void Game::renderHUD(const sf::Drawable& drawable) {
    window.setView(camera->HUDview);

    window.draw(drawable);

    window.setView(camera->view);
}

int main() {
    Game().run();
    return 0;
}
