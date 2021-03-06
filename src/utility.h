#ifndef UTILITY_H
#define UTILITY_H

#include <SFML/System.hpp>

#include <math.h>
#include <algorithm>

constexpr double PI = 3.14159265;

namespace utility {

    sf::Vector2f normalize(const sf::Vector2f& source);

    float dotProduct(const sf::Vector2f& v1, const sf::Vector2f& v2);

    float magnitude(sf::Vector2f point, sf::Vector2f origin = sf::Vector2f(0.f,0.f));

    float distance(const sf::Vector2f& p1, const sf::Vector2f& p2);

    sf::Vector2f rotatePoint(sf::Vector2f point, sf::Vector2f origin, float angle);

    sf::Vector2f lerp(float t, const sf::Vector2f &a, const sf::Vector2f &b);

    float lerp(float t, float a, float b);

    float smoothStep(float t);

    float smootherStep(float t);

    float clamp(float n, float lower, float upper);
}

#endif /* UTILITY_H */
