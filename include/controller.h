#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "ground.h"

// connect the game logic and the graphic logic
// give command to the ground, and convert the ground to color data where can be directly sent to vbos

class Controller {

    Ground ground;

public:
    Controller() {
        srand(time(NULL));
    }


    vec3 getBrickColor(vec2 position) {
        // assume the position is valid
        // return the color (vec3) of one brick
        int i = 6 * vec2ToInt(position);
        // assume the 6 points of a brick has only one color
        // return the color of the first 
        return this->gl_brick_colors[i];
    }


    // TODO: delete?
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