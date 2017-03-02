#include <Vector2.hpp>

#include <math.h>
#include <algorithms>

sf::Vector2 normalize(const sf::Vector2 &source) {
    float length = sqrt((source.x * source.x) + (source.y * source.y));

    if(length != 0)
        return sf::Vector2f(source.x / length, source.y / length);
    else
        return source;
}

sf::Vector2f lerp(float t, sf::Vector2f &a, sf::Vector2f &b) {
    return (1-t)*a + t*b;
}

float clamp(float n, float lower, float upper) {
    return std::max(lower, std::min(n, upper));
}
