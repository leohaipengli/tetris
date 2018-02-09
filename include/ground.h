#ifndef _GROUND_H_
#define _GROUND_H_

#include <vector>

#include "shape.h"
#include "vec.h"
#include "mat.h"
#include "config.h"
#include "utils.h"
// #include "brick.h"
using namespace std;

class Ground {
private:
    Shape *currentShape;
    // size: col*row
    // TODO: convert 1D to 2D in order to recognize border correctly
    vector<vec3*> brick_colors;
public:

    Ground() {
        initBrickColors();
    }
    ~Ground() {
        for(auto& color: brick_colors) {
            delete color;
        }
    }
    // TODO: more game logic: eliminate a row 
    bool isOutOfGround(vec2 position) {
        // FIXME: bug
        int intPosition = vec2ToInt(position);
        if(intPosition < 0 || intPosition >= NUM_COLS * NUM_ROWS) {
            return true;
        } else {
            return false;
        }
    }
    bool hasBrick(vec2 position) {
        return brick_colors[vec2ToInt(position)] != NULL;
    }
    bool isAvailable(vec2 position) {
        // return false if
        //   1. position out of range
        //   2. position has brick
        return !isOutOfGround(position) && !hasBrick(position);
    }

    void deleteBricks(vector<vec2> positions) {
        // delete color from multiple positions (delete bricks)
        for(auto& position: positions) {
            if(isOutOfGround(position)) {
                cout << "warning: try to delete brick in position " << position << " , which is out of the border of the ground" << endl;
            } else {
                brick_colors[vec2ToInt(position)] = NULL;
            }
        }
    }

    void setBricks(vector<vec2> positions, vec3 color) {
        // set color to multiple positions 
        for(auto& position: positions) {
            if(isOutOfGround(position)) {
                // tentative solution for out of range: show warning, not draw
                cout << "warning: position " << position << " out of the border of the ground" << endl;
            } else {
                brick_colors[vec2ToInt(position)] = new vec3(color);
            }
        }
    }
    // TODO: when the position is out of ground, don't show in the ground , but keep in the shape
    bool moveShape(char direction) {
        // return success
        // move currentShape
        vector<vec2> new_positions = currentShape->getNewPositionsMove(direction);
        for(auto& new_position: new_positions) {
            if(!currentShape->hasBrick(new_position) && !this->isAva)) {
                // fail to move
                return false;
            }
        }
        deleteBricks(currentShape->getBrickPositions());
        // allow to move
        currentShape->setAllBricks(new_positions);
        setBricks(new_positions, currentShape->getColor());
        return true;
    }
    
    bool rotateShape() {
        vector<vec2> new_positions = currentShape->getNewPositionsRotation();
        for(auto& new_position: new_positions) {
            if(!currentShape->hasBrick(new_position) && !this->isAvailable(new_position)) {
                // fail to rotate
                return false;
            }
        }
        // TODO: consider combine the two duplacated code pieces into one?
        deleteBricks(currentShape->getBrickPositions());
        // allow to move
        currentShape->setAllBricks(new_positions);
        setBricks(new_positions, currentShape->getColor());
        return true;
    }

    void initBrickColors() {
        // init brick_colors with all NULL (no brick exists)
        brick_colors.resize(NUM_ROWS * NUM_COLS, NULL);
    }

    bool newShape(vec2 pivot_position, vec3 color) {
        // return true if new shape can be generated
        // else GAME OVER
        currentShape = Create(pivot_position, color);
        auto positions = currentShape->getBrickPositions();
        for(auto& position: positions) {
            if(!isAvailable(position)) {
                // return false if not (game over)
                delShape();
                return false;
            }
        }
        // valid creatation: setBricks in ground
        setBricks(positions, currentShape->getColor());
        return true;
    }

    void delShape() {
        // delete the shape but keep its bricks record in ground
        if(currentShape != NULL) {
            delete currentShape;
            currentShape = NULL;
        }
    }

    vector<vec3*> getColors() {
        return brick_colors;
    }

};
#endif