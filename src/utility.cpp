#include "utility.h"


namespace utility {

    sf::Vector2f normalize(const sf::Vector2f &source) {
        float length = sqrt((source.x * source.x) + (source.y * source.y));

        if(length != 0)
            return sf::Vector2f(source.x / length, source.y / length);
        else
            return source;
    }

    float dotProduct(const sf::Vector2f& v1, const sf::Vector2f& v2) {
        return (v1.x * v2.x) + (v1.y * v2.y);
    }

    float magnitude(sf::Vector2f point, sf::Vector2f origin = sf::Vector2f(0.f,0.f)) {
       return sqrt( pow((point.x - origin.x), 2) + pow((point.y - origin.y), 2) );
    }

    float distance(const sf::Vector2f& p1, const sf::Vector2f& p2) {
        return sqrt( pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2) );
    }

    sf::Vector2f rotatePoint(sf::Vector2f point, sf::Vector2f origin, float angle) {
        sf::Vector2f displacement = point - origin;
        angle = -angle * PI / 180.0;        //sfml uses clockwise rotation

        point.x = displacement.x * cos(angle) + displacement.y * sin(angle);
        point.y = displacement.y * cos(angle) - displacement.x * sin(angle);
        return point + origin;
    }

    sf::Vector2f lerp(float t, const sf::Vector2f &a, const sf::Vector2f &b) {
        return (1-t)*a + t*b;
    }

    float lerp(float t, float a, float b) {
        return (1-t)*a + t*b;
    }

    float clamp(float n, float lower, float upper) {
        return std::max(lower, std::min(n, upper));
    }
}
