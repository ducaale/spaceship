#ifndef BULLET_H
#define BULLET_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class Camera;

class Bullet {
public:
    Bullet();
    Bullet(sf::Texture *texture, sf::Vector2f coordinate, sf::Vector2f direction, float speed);
    void update(sf::Time frameTime);
    void draw(sf::RenderWindow *window, Camera *camera);

    float distanceTraveled();
    bool isDestroyed() const { return destroyed; }

private:
    sf::Sprite bullet;

    sf::Vector2f intialCoordinate;

    sf::Vector2f direction;
    sf::Vector2f coordinate;
    sf::Vector2f velocity;

    int nFrames;

    float speed;
    bool destroyed;
};

#endif /* BULLET_H */
