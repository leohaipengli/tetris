#ifndef _SHAPE_H_
#define _SHAPE_H_

#include <vector>
#include "vec.h"
#include "mat.h"
#include "config.h"
#include "brick.h"

using namespace std;

class Shape {
    // don't consider collision
    // detect collision in ground
private:
    vector<Brick> bricks;
    Brick* pivot;
    // TODO:
    // translation & rotation matrix ??
public:
    Shape() {}
    void rotate() {
        // check the formula of rotation

    }

    void move(char direction) {
        // check the formula of translation

    }

    vector<Brick> getBricks() {
        return bricks;
    }

};

class TShape: public Shape {
public:
    TShape(vec2 pivot_position) {}

};

#endif