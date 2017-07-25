#include "game.h"
#include "spaceship.h"
#include "camera.h"
#include "components.h"
#include <iostream>

Entity& Game::createEnemy() {
    auto& texture = resource["orb"];
    if(!texture.loadFromFile("../spritesheet/the_orb.png")) {
        std::cout << "unable to load file" << std::endl;
    }
    auto& entity = manager.addEntity();
    entity.addComponent<CPosition>(sf::Vector2f(200.f,200.f));
    entity.addComponent<CSprite>(this, sf::Sprite(resource["orb"], {0,0,128,128}));
    entity.addComponent<CRotate>();

    createLeftArm(entity);
    createRightArm(entity);

    return entity;
}

Entity& Game::createLeftArm(Entity& parent) {
    auto& entity = manager.addEntity();
    entity.addComponent<CPosition>(sf::Vector2f(0.f,-100.f));
    entity.addComponent<CParent>(&parent);
    entity.addComponent<CSprite>(this, sf::Sprite(resource["orb"], {128,384,128,32}));

    return entity;
}

Entity& Game::createRightArm(Entity& parent) {
    auto& entity = manager.addEntity();
    entity.addComponent<CPosition>(sf::Vector2f(0.f,100.f));
    entity.addComponent<CParent>(&parent);
    entity.addComponent<CSprite>(this, sf::Sprite(resource["orb"], {0,384,128,32}));

    return entity;
}

Game::Game() {
    spaceship = new Spaceship(window);
    camera = new Camera(spaceship);

    createEnemy();
    //sf::Texture texture;
    //texture.loadFromFile("../background/nebula.png");
    //sf::Sprite background(texture);
    //background.setScale(1/1.28f, 1/1.28f);
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
    for(; simulationTime <= elapsedTime; simulationTime += timeSlice) {
        spaceship->update(timeSlice);
        camera->update(timeSlice);
        manager.update(timeSlice.asSeconds());
    }
}

void Game::drawPhase() {
    //window.draw(background);
    spaceship->draw(*camera);
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
