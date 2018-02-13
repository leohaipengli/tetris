#ifndef _GROUND_H_
#define _GROUND_H_

#include <vector>

#include  "shapefactory.h"
#include "config.h"
#include "utils.h"

using namespace std;

class Ground {
private:
    Shape *currentShape;
    // size: col*row
    // TODO: convert 1D to 2D in order to recognize border correctly
    vector<vector<vec3*>> brick_colors;
public:

    Ground() {
        initBrickColors();
    }
    ~Ground() {
        delete currentShape;
        for(auto& col: brick_colors) {
            for(auto& color: col){
                delete color;
            }
        }
    }
    // TODO: more game logic: eliminate a row 
    bool isOutOfGround(vec2 position);
    bool hasBrick(vec2 position);
    bool isAvailable(vec2 position);
    void deleteBricks(vector<vec2> positions);
    void setBricks(vector<vec2> positions, vec3 color);
    bool moveShape(char direction);
    bool rotateShape();
    void initBrickColors();
    bool newShape(vec2 pivot_position, vec3 color);
    void delShape();
    vec3* getColor(int x, int y);
};

#endif