#ifndef _BRICK_H_
#define _BRICK_H_

#include "Angel.h"
// #include "vec.h"
// #include "mat.h"
#include "config.h"

class Brick {

    // TODO: need to change position from vec2 to vec3 (inorder to fit the transform matrix & coordinates in opengl ) ??
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

    vec2 getPosition() {
        return position;
    }

    void setPosition(vec2 new_position) {
        position = new_position;
    }

    void move(vec2 transform_vec) {
        position += transform_vec;
    }

    void move(char direction) {
        // simply set the position
        // since it has no knowledge of other bricks
        switch(direction) {
            case 'u':
                position[0]++;
                break;
            case 'd':
                position[0]--;
                break;
            case 'l':
                position[1]--;
                break;
            case 'r':
                position[1]++;
                break;

        }
    }

    vec3 getColor() {
        return color;
    }
};

#endif
