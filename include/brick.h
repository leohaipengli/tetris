#ifndef _BRICK_H_
#define _BRICK_H_

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

    void move(char direction) {
        // simply set the position
        // since it has no knowledge of other bricks
    }

    vec3 getColor() {
        return this.color;
    }
}

#endif
