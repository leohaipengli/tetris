#ifndef _BRICK_H_
#define _BRICK_H_

#include "Angel.h"
#include "vec.h"
#include "mat.h"
#include "config.h"

class Brick {

    vec2 position;
    vec3 color;

public:
    Brick(int row, int col) {
        Brick(vec2(row, col), background_color);
    }

    Brick(int row, int col, vec3 color) {
        Brick(vec2(row, col), color);
    }


    Brick(vec2 position) {
        Brick(position, background_color);
    }

    Brick(vec2 position, vec3 color): position(position), color(color) {}

    vec2 getPosition();
    void setPosition(vec2 new_position);
    void move(vec2 transform_vec);
    void move(char direction);
    vec3 getColor();

};

#endif
