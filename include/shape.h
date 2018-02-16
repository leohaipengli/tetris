#ifndef _SHAPE_H_
#define _SHAPE_H_

#include <iostream>
#include <vector>
#include "brick.h"

using namespace std;


class Shape {
    // don't consider collision
    // detect collision in ground
protected:
    vec3 color;
    vector<Brick> bricks;
    Brick *pivot;

public:
    Shape() {}
    Shape(vec3 color): color(color) {}

    bool hasBrick(vec2 position);
    
    void setAllBricks(vector<vec2> new_positions);
    vector<vec2> getBrickPositions();
    void setColor(vec3 new_color);
    vec3 getColor();
    vector<vec2> getNewPositionsRotation();
    void rotate();
    vector<vec2> getNewPositionsMove(char direction);
    void move(char direction);
    void print();
};

class TShape: public Shape {
public:
    TShape(vec2 pivot_position, vec3 color): Shape(color) {
        bricks.emplace_back(pivot_position, color);
        bricks.emplace_back(pivot_position + vec2(-1, 0), color);
        bricks.emplace_back(pivot_position + vec2(1, 0), color);
        bricks.emplace_back(pivot_position + vec2(0, -1), color);
        pivot = &bricks[0];
    }
};

class JShape: public Shape {
public:
    JShape(vec2 pivot_position, vec3 color): Shape(color) {
        bricks.emplace_back(pivot_position, color);
        bricks.emplace_back(pivot_position + vec2(-1, 0), color);
        bricks.emplace_back(pivot_position + vec2(1, 0), color);
        bricks.emplace_back(pivot_position + vec2(1, -1), color);
        pivot = &bricks[0];
    }
};

class LShape: public Shape {
public:
    LShape(vec2 pivot_position, vec3 color): Shape(color) {
        bricks.emplace_back(pivot_position, color);
        bricks.emplace_back(pivot_position + vec2(-1, 0), color);
        bricks.emplace_back(pivot_position + vec2(1, 0), color);
        bricks.emplace_back(pivot_position + vec2(-1, -1), color);
        pivot = &bricks[0];
    }
};

class ZShape: public Shape {
public:
    ZShape(vec2 pivot_position, vec3 color): Shape(color) {
        bricks.emplace_back(pivot_position, color);
        bricks.emplace_back(pivot_position + vec2(-1, 0), color);
        bricks.emplace_back(pivot_position + vec2(1, -1), color);
        bricks.emplace_back(pivot_position + vec2(0, -1), color);
        pivot = &bricks[0];
    }
};

class IShape: public Shape {
public:
    IShape(vec2 pivot_position, vec3 color): Shape(color) {
        bricks.emplace_back(pivot_position, color);
        bricks.emplace_back(pivot_position + vec2(-2, 0), color);
        bricks.emplace_back(pivot_position + vec2(-1, 0), color);
        bricks.emplace_back(pivot_position + vec2(1, 0), color);
        pivot = &bricks[0];
    }
};

class OShape: public Shape {
public:
    OShape(vec2 pivot_position, vec3 color): Shape(color) {
        bricks.emplace_back(pivot_position, color);
        bricks.emplace_back(pivot_position + vec2(-1, 0), color);
        bricks.emplace_back(pivot_position + vec2(-1, -1), color);
        bricks.emplace_back(pivot_position + vec2(0, -1), color);
        pivot = &bricks[0];
    }
};

#endif