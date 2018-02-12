#include "controller.h"

Ground ground;

vector<vec3> gl_grid_points;
vector<vec3> gl_brick_points;
vector<vec3> gl_brick_colors;

void initGrids() {
    // insert vertical grid points
    for(int i = 1; i < NUM_COLS; i++) {
        gl_grid_points.push_back(vec3(toGLCoordinate((float)i/NUM_COLS), -1.0, 0));
        gl_grid_points.push_back(vec3(toGLCoordinate((float)i/NUM_COLS), 1.0, 0));
    }
    // insert horizontal grid points
    for(int i = 1; i < NUM_ROWS; i++) {
        gl_grid_points.push_back(vec3(-1.0, toGLCoordinate((float)i/NUM_ROWS), 0));
        gl_grid_points.push_back(vec3(1.0, toGLCoordinate((float)i/NUM_ROWS), 0));
    }

}

void initColors() {
    // num of points: 6 (for each brick) * NUM_ROWS * NUM_COLS
    gl_brick_colors.resize(6 * NUM_COLS * NUM_ROWS);
    for(int i = 0; i < 6 * NUM_ROWS * NUM_COLS; i++) {
        gl_brick_colors[i] = vec3(background_color);
    }
}

void initBricks() {
    // num of points: 6 (for each brick) * NUM_ROWS * NUM_COLS
    gl_brick_points.resize(6 * NUM_COLS * NUM_ROWS);
    int rem = 0, quo = 0;
    for(int i = 0; i < NUM_ROWS * NUM_COLS; i++) {
        rem = i % NUM_COLS;
        quo = i / NUM_COLS;
        gl_brick_points[6*i] = vec3(toGLCoordinate((float)rem/NUM_COLS), toGLCoordinate((float)quo/NUM_ROWS), 0);
        gl_brick_points[6*i+1] = vec3(toGLCoordinate((float)(rem)/NUM_COLS), toGLCoordinate((float)(quo+1)/NUM_ROWS), 0);
        gl_brick_points[6*i+2] = vec3(toGLCoordinate((float)(rem+1)/NUM_COLS), toGLCoordinate((float)(quo)/NUM_ROWS), 0);
        gl_brick_points[6*i+3] = vec3(toGLCoordinate((float)(rem)/NUM_COLS), toGLCoordinate((float)(quo+1)/NUM_ROWS), 0);
        gl_brick_points[6*i+4] = vec3(toGLCoordinate((float)(rem+1)/NUM_COLS), toGLCoordinate((float)(quo)/NUM_ROWS), 0);
        gl_brick_points[6*i+5] = vec3(toGLCoordinate((float)(rem+1)/NUM_COLS), toGLCoordinate((float)(quo+1)/NUM_ROWS), 0);
    }
}
void onShapeFinish() {
    // do something when one shape is finished (touch the buttom of the ground)
    ground.delShape();
    // if game is not over
    // FIXME: change the init position of shape & use random color
    if(ground.newShape(vec2(NUM_COLS/2, NUM_ROWS-2), vec3(1, 0, 0))) {
        // TODO: if new shape can be generated

    } else {
        // TODO: if the game is over: 
        cout << "Game Over" << endl;
    }
}

// FIXME: bugs here!
void setBrickColor(vec2 position, vec3 color_vector) {
    int i = 6 * vec2ToInt(position);
    if(i < 6 * NUM_COLS * NUM_ROWS && i >= 0) {
        for(int j = 0; j < 6; j++) {
            gl_brick_colors[i+j] = color_vector;
        }
    } else {
        cout << "setBrickColor(): ignore invalid position\n";
    }
}

// update all colors
void updateColors() {
    for(int i = 0; i < NUM_COLS * NUM_ROWS; i++) {
        vec2 position = intToVec2(i);
        vec3 color = ground.getColors()[i] != NULL ? *(ground.getColors()[i]): background_color;
        // FIXME: delete debugging info
        if(color[0] != background_color[0]) {
            cout << "set " << position << " color: " << color << endl;
        }
        setBrickColor(position, color);
    }
}

void autoDropDown() {
    // should be triggered by timer
    if(ground.moveShape('d')) {
        // if not bottom
    } else {
        // bottom
        onShapeFinish();
    }
    updateColors();
}
void onKeyPressed(unsigned char key, int x, int y) {
    // if failed, just return and don't updateColors
    bool success = true;
    switch ( key ) {
    case 0x1b:
        // cout << "esc\n";
        exit( EXIT_SUCCESS );
        break;
    case 'w':
        success = ground.rotateShape();
        cout << "up\n";
        break;
    case 's':
        //do something here
        while(ground.moveShape('d')) {}
        onShapeFinish();
        cout << "down\n";
        break;
    case 'a':
        //do something here
        success = ground.moveShape('l');
        cout << "left\n";
        break;
    case 'd':
        //do something here
        success = ground.moveShape('r');
        cout << "right\n";
        break;
    }

    if(success){ 
        updateColors();
    }
}