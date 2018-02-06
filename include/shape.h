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

    // TODO: how to deal with the strange floating number when the result is 0?
    void rotate() {
        // not use rotation matrix to rotate.
        // since only rotate 90 degree, write the code directly
        vec2 pivot_position = pivot->getPosition();
        vec2 brick_position;
        for(auto& brick: bricks) {
            brick_position = brick.getPosition();
            // apply rotatation formula
            brick.setPosition(
                vec2(
                    pivot_position[0] + pivot_position[1] - brick_position[1],
                    -pivot_position[0] + pivot_position[1] + brick_position[0]
                )
            );
        }
    }

    void move(char direction) {
        vec2 translation_vec;
        switch(direction) {
            case 'u':
                translation_vec = vec2(0, 1);
                break;
            case 'd':
                translation_vec = vec2(0, -1);
                break;
            case 'l':
                translation_vec = vec2(-1, 0);
                break;
            case 'r':
                translation_vec = vec2(1, 0);
                break;
        }

        for(auto& brick: bricks) {
            brick.move(translation_vec);
        }
    }

    vector<Brick> getBricks() {
        return bricks;
    }

    void setColor(vec3 new_color) {
        color = new_color;
    }

    void print() {
        // for test purpose
        cout << "position of brick:" << endl;
        for(auto& brick: bricks) {
            cout << "\t" << brick.getPosition() << endl;
        }
        cout << "color : " << color << endl;
    }

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

// create a random shape
template<typename T>
Shape *Create(vec2 pivot_position, vec3 color) {
    return new T(pivot_position, color);
}

typedef Shape* (*CreateFn)(vec2, vec3);
CreateFn createArray[] = {
    &Create<TShape>,
    &Create<OShape>,
    &Create<IShape>,
    &Create<ZShape>,
    &Create<LShape>,
    &Create<JShape>,
};

const size_t fncount = sizeof(createArray)/sizeof(*createArray);
Shape *Create(vec2 pivot_position, vec3 color)
{
   return createArray[rand() % fncount](pivot_position, color); //forward the call
}


#endif