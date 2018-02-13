#ifndef _UTILS_H_
#define _UTILS_H_

#include "vec.h"
#include "mat.h"
#include "config.h"

inline int vec2ToInt(vec2 coordinate) {
    return coordinate[0] + NUM_ROWS * coordinate[1];
}

inline vec2 intToVec2(int coordinate) {
    return vec2(coordinate % NUM_ROWS, coordinate / NUM_ROWS);
}

inline GLfloat toGLCoordinate(float in) {
    // in: float 0 ~ 1
    // out: -1 ~ 1
    return (GLfloat)in * 2 - 1;
}

#endif