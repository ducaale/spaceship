#include <Vector2.hpp>
#include <math.h>

Vector2 normalize(const Vector2 &source) {
    float length = sqrt((source.x * source.x) + (source.y * source.y));

    if(length != 0)
        return Vector2(source.x / length, source.y / length);
    else
        return source;
}
