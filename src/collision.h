/*
 * reference url: https://www.gamedev.net/articles/programming/general-and-gameplay-programming/2d-rotated-rectangle-collision-r2604
 */

#ifndef COLLISION_H
#define COLLISION_H

#include "entity.h"
#include "component.h"
#include "components.h"

#include <array>

void getVertices(Entity& e, std::array<sf::Vector2f, 4>& entity_vertices) {
    sf::Transform t;
    float width, height;
    if(e.hasComponent<CSprite>()) {
        t = e.getComponent<CSprite>().getTransform(true, true);
        width = e.getComponent<CSprite>().width();
        height = e.getComponent<CSprite>().height();
    }
    else {
        t = e.getComponent<CAnimatedSprite>().getTransform(true, true);
        width = e.getComponent<CAnimatedSprite>().width();
        height = e.getComponent<CAnimatedSprite>().height();
    }
    if(e.hasComponent<CParent>()) {
        t = e.getComponent<CParent>().getTransform() * t;
    }

    entity_vertices[0] = t.transformPoint(0.f, 0.f);
    entity_vertices[1] = t.transformPoint(width, 0.f);
    entity_vertices[2] = t.transformPoint(width, height);
    entity_vertices[3] = t.transformPoint(0.f, height);
}

void getAxis(std::array<sf::Vector2f, 4>& vertices1, std::array<sf::Vector2f, 4>& vertices2, std::array<sf::Vector2f, 4>& axis) {
    //entity1 axis
    axis[0] = vertices1[1] - vertices1[0];
    axis[1] = vertices1[1] - vertices1[2];

    //entity2 axis
    axis[2] = vertices2[1] - vertices2[0];
    axis[3] = vertices2[1] - vertices2[2];
}

/*
 * project vertices to axis
 * returns min and max point scalar value
 */
std::pair<int, int> projVectToAxis(const sf::Vector2f& entity_axis, std::array<sf::Vector2f, 4>& entity_vertices) {

    int min = utility::dotProduct(entity_vertices[0], entity_axis);
    int max = utility::dotProduct(entity_vertices[0], entity_axis);
    int scalar_value;
    for(int i = 1; i < 4; i++) {
        scalar_value = utility::dotProduct(entity_vertices[i], entity_axis);
        if(scalar_value > max) max = scalar_value;
        if(scalar_value < min) min = scalar_value;
    }

    return std::make_pair(min, max);
}

bool overlap(float min1, float max1, float min2, float max2) {
    if(max1 < min2 || max2 < min1)
        return false;

    return true;
}


/**
 * SAT collision test for rectangles
 */
bool test_collision(Entity& e1, Entity& e2) {

    std::array<sf::Vector2f, 4> entity1_vertices, entity2_vertices;
    getVertices(e1, entity1_vertices);
    getVertices(e2, entity2_vertices);

    std::array<sf::Vector2f, 4> entity_axis;
    getAxis(entity1_vertices, entity2_vertices, entity_axis);
    
    for(auto& axis : entity_axis) {
        auto [min1, max1] = projVectToAxis(axis, entity1_vertices);
        auto [min2, max2] = projVectToAxis(axis, entity2_vertices);
        if(!overlap(min1, max1, min2, max2)) return false;
    }

    return true;
}

#endif /* COLLISION_H */
