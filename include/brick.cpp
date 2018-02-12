#include "brick.h"

vec2 Brick::getPosition() {
    return position;
}

void Brick::setPosition(vec2 new_position) {
    position = new_position;
}

void Brick::move(vec2 transform_vec) {
    position += transform_vec;
}

void Brick::move(char direction) {
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

vec3 Brick::getColor() {
    return color;
}