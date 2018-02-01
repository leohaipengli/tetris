#ifndef _GROUND_H_
#define _GROUND_H_

#include <vector>

#include "vec.h"
#include "mat.h"
#include "config.h"
#include "utils.h"
// #include "brick.h"
using namespace std;

class Ground {
public:
    vector<vec3> grid_points;
    vector<vec3> brick_points;
    vector<vec3> brick_colors;
    Ground() {
        srand(time(NULL));
        this->generateBrick();
        this->generateColor();
    }


    void generateColor() {
        // num of points: 6 (for each brick) * NUM_ROWS * NUM_COLS
        this->brick_colors.resize(6 * NUM_COLS * NUM_ROWS);
        for(int i = 0; i < 6 * NUM_ROWS * NUM_COLS; i++) {
            this->brick_colors[i] = vec3((float) rand() / (RAND_MAX), (float) rand() / (RAND_MAX), (float) rand() / (RAND_MAX)) / 5; 
        }
    }

    void generateBrick() {
        // num of points: 6 (for each brick) * NUM_ROWS * NUM_COLS
        this->brick_points.resize(6 * NUM_COLS * NUM_ROWS);
        int rem = 0, quo = 0;
        for(int i = 0; i < NUM_ROWS * NUM_COLS; i++) {
            rem = i % NUM_COLS;
            quo = i / NUM_COLS;
            this->brick_points[6*i] = vec3(toGLCoordinate((float)rem/NUM_COLS), toGLCoordinate((float)quo/NUM_ROWS), 0);
            this->brick_points[6*i+1] = vec3(toGLCoordinate((float)(rem)/NUM_COLS), toGLCoordinate((float)(quo+1)/NUM_ROWS), 0);
            this->brick_points[6*i+2] = vec3(toGLCoordinate((float)(rem+1)/NUM_COLS), toGLCoordinate((float)(quo)/NUM_ROWS), 0);
            this->brick_points[6*i+3] = vec3(toGLCoordinate((float)(rem)/NUM_COLS), toGLCoordinate((float)(quo+1)/NUM_ROWS), 0);
            this->brick_points[6*i+4] = vec3(toGLCoordinate((float)(rem+1)/NUM_COLS), toGLCoordinate((float)(quo)/NUM_ROWS), 0);
            this->brick_points[6*i+5] = vec3(toGLCoordinate((float)(rem+1)/NUM_COLS), toGLCoordinate((float)(quo+1)/NUM_ROWS), 0);
        }
    }

    vec3 getBrickColor(vec2 position) {
        // assume the position is valid
        // return the color (vec3) of one brick
        int i = 6 * vec2ToInt(position);
        // assume the 6 points of a brick has only one color
        // return the color of the first 
        return this->brick_colors[i];
    }

    void setBrickColor(vec2 position, vec3 color_vector) {
        int i = 6 * vec2ToInt(position);
        if(i < NUM_COLS * NUM_ROWS && i >= 0) {
            for(int j = 0; j < 6; j++) {
                brick_colors[i+j] = color_vector;
            }

        } else {
            cout << "setBrickColor(): ignore invalid position\n";
        }
    }

    void moveBrickColor(vec2 position, char direction) {
        // assume the position is always valid
        // the position checking is done by other functions
        vec3 color = getBrickColor(position);
        vec2 new_position;
        switch(direction) {
            case 'u':
                new_position = vec2(position[0] + 1, position[1]);
                break;
            case 'd':
                new_position = vec2(position[0] - 1, position[1]);
                break;
            case 'l':
                new_position = vec2(position[0], position[1] - 1);
                break;
            case 'r':
                new_position = vec2(position[0], position[1] + 1);
                break;
        }
        // cout << "old position: " << position << endl;
        // cout << "new position: " << new_position << endl;
        setBrickColor(position, background_color);
        setBrickColor(new_position, color);
    }

};
#endif