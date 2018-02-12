#include "ground.h"
bool Ground::isOutOfGround(vec2 position) {
    // FIXME: bug
    int intPosition = vec2ToInt(position);
    if(intPosition < 0 || intPosition >= NUM_COLS * NUM_ROWS) {
        return true;
    } else {
        return false;
    }
}
bool Ground::hasBrick(vec2 position) {
    return brick_colors[vec2ToInt(position)] != NULL;
}
bool Ground::isAvailable(vec2 position) {
    // return false if
    //   1. position out of range
    //   2. position has brick
    return !isOutOfGround(position) && !hasBrick(position);
}

void Ground::deleteBricks(vector<vec2> positions) {
    // delete color from multiple positions (delete bricks)
    for(auto& position: positions) {
        if(isOutOfGround(position)) {
            cout << "warning: try to delete brick in position " << position << " , which is out of the border of the ground" << endl;
        } else {
            brick_colors[vec2ToInt(position)] = NULL;
        }
    }
}

void Ground::setBricks(vector<vec2> positions, vec3 color) {
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
bool Ground::moveShape(char direction) {
    // return success
    // move currentShape
    vector<vec2> new_positions = currentShape->getNewPositionsMove(direction);
    for(auto& new_position: new_positions) {
        if(!currentShape->hasBrick(new_position) && !this->isAvailable(new_position)) {
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

bool Ground::rotateShape() {
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

void Ground::initBrickColors() {
    // init brick_colors with all NULL (no brick exists)
    brick_colors.resize(NUM_ROWS * NUM_COLS, NULL);
}

bool Ground::newShape(vec2 pivot_position, vec3 color) {
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

void Ground::delShape() {
    // delete the shape but keep its bricks record in ground
    if(currentShape != NULL) {
        delete currentShape;
        currentShape = NULL;
    }
}

vector<vec3*> Ground::getColors() {
    return brick_colors;
}