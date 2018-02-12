#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "ground.h"
#include <iostream>
#include <vector>

extern Ground ground;

extern vector<vec3> gl_grid_points;
extern vector<vec3> gl_brick_points;
extern vector<vec3> gl_brick_colors;


// connect the game logic and the graphic logic
// give command to the ground, and convert the ground to color data where can be directly sent to vbos

void initGrids();
void initColors();
void initBricks();
void onShapeFinish();
void setBrickColor(vec2 position, vec3 color_vector);
void updateColors();
void autoDropDown();
void onKeyPressed(unsigned char key, int x, int y);


#endif