#include "shape.h"

bool Shape::hasBrick(vec2 position) {
    for(auto& brick: bricks) {
        if(brick.getPosition() == position){
            return true;
        }
    }
    return false;
}

void Shape::setAllBricks(vector<vec2> new_positions) {
    // assume the size of two vectors are the same
    for(unsigned i = 0; i < bricks.size(); i++) {
        bricks[i].setPosition(new_positions[i]);
    }
}

vector<vec2> Shape::getBrickPositions() {
    vector<vec2> positions;
    for(auto& brick: bricks) {
        positions.push_back(brick.getPosition());
    }
    return positions;
}

void Shape::setColor(vec3 new_color) {
    color = new_color;
}

vec3 Shape::getColor() {
    return color;
}

vector<vec2> Shape::getNewPositionsRotation() {
    // show the new occupied positions 
    vector<vec2> new_positions;
    vec2 pivot_position = pivot->getPosition();
    vec2 brick_position;
    for(auto& brick: bricks) {
        brick_position = brick.getPosition();
        // apply rotatation formula
        vec2 new_position = vec2(
                pivot_position[0] + pivot_position[1] - brick_position[1],
                -pivot_position[0] + pivot_position[1] + brick_position[0]
            );
        new_positions.push_back(new_position);
    }
    return new_positions;
}

// TODO: how to deal with the strange floating number when the result is 0?
void Shape::rotate() {
    // not use rotation matrix to rotate.
    // since only rotate 90 degree, write the code directly
    vector<vec2> new_positions = getNewPositionsRotation();
    setAllBricks(new_positions);
}

vector<vec2> Shape::getNewPositionsMove(char direction) {
    vector<vec2> new_positions;
    vec2 translation_vec;
    switch(direction) {
        case 'u':
            translation_vec = vec2(0, 1);
            break;
        case 'd':
            translation_vec = vec2(0, -1);
            break;
        case 'l':
            translation_vec = vec2(-1, 0);
            break;
        case 'r':
            translation_vec = vec2(1, 0);
            break;
    }
    for(auto& brick: bricks) {
        vec2 new_position = vec2(brick.getPosition()+translation_vec);
        // add to new positions list
        new_positions.push_back(new_position);
    }
    return new_positions;
}

void Shape::move(char direction) {
    vector<vec2> new_positions = this->getNewPositionsMove(direction);
    setAllBricks(new_positions);
}

void Shape::print() {
    // for test purpose
    cout << "position of brick:" << endl;
    for(auto& brick: bricks) {
        cout << "\t" << brick.getPosition() << endl;
    }
    cout << "color : " << color << endl;
}
