#include "controller.h"

bool gameStatus = false;

Ground ground;

vector<vec2> gl_grid_points;
vector<vec3> gl_grid_colors;
vector<vec2> gl_brick_points;
vector<GLint> gl_brick_elements;
vector<vec3> gl_brick_colors;
float gl_gameover_vertices[] = {
//  Position(2)   Color(3)          Texcoords(2)
    -1,  0.5f, // Top-left
    1,  0.5f, // Top-right
    1, -0.5f, // Bottom-right
    -1, -0.5f, // Bottom-left
};
float gl_gameover_colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 
    0.0f, 0.0f, 1.0f, 
    1.0f, 1.0f, 1.0f,
};
float gl_gameover_tex_positions[] = {
    0.0f, 1.0f, 
    1.0f, 1.0f,
    1.0f, 0.0f, 
    0.0f, 0.0f,
};
GLuint gl_gameover_elements[] = {
    0, 1, 2,
    2, 3, 0
};

void initGrids() {
    // insert vertical grid points
    for(int i = 1; i < NUM_COLS; i++) {
        gl_grid_points.emplace_back(vec2(toGLCoordinate((float)i/NUM_COLS), -1.0));
        gl_grid_points.emplace_back(vec2(toGLCoordinate((float)i/NUM_COLS), 1.0));
    }
    // insert horizontal grid points
    for(int i = 1; i < NUM_ROWS; i++) {
        gl_grid_points.emplace_back(vec2(-1.0, toGLCoordinate((float)i/NUM_ROWS)));
        gl_grid_points.emplace_back(vec2(1.0, toGLCoordinate((float)i/NUM_ROWS)));
    }
    // assign grid color
    for(int i = 0; i < 2 * (NUM_ROWS - 1) + 2 * (NUM_COLS - 1); i++) {
        gl_grid_colors.emplace_back(vec3(grid_color));
    }

}

void initBricks() {
    // num of points: 6 (for each brick) * NUM_ROWS * NUM_COLS
    gl_brick_points.resize(4 * NUM_COLS * NUM_ROWS);
    gl_brick_elements.resize(6 * NUM_COLS * NUM_ROWS);
    int rem = 0, quo = 0;
    for(int i = 0; i < NUM_ROWS * NUM_COLS; i++) {
        rem = i % NUM_COLS;
        quo = i / NUM_COLS;
        // (0, 0)
        gl_brick_points[4*i] = vec2(toGLCoordinate((float)rem/NUM_COLS), toGLCoordinate((float)quo/NUM_ROWS));
        // (0, 1)
        gl_brick_points[4*i+1] = vec2(toGLCoordinate((float)(rem)/NUM_COLS), toGLCoordinate((float)(quo+1)/NUM_ROWS));
        // (1, 0)
        gl_brick_points[4*i+2] = vec2(toGLCoordinate((float)(rem+1)/NUM_COLS), toGLCoordinate((float)(quo)/NUM_ROWS));
        // (1, 1)
        gl_brick_points[4*i+3] = vec2(toGLCoordinate((float)(rem+1)/NUM_COLS), toGLCoordinate((float)(quo+1)/NUM_ROWS));

        // specify the indices in gl_brick_points
        gl_brick_elements[6 * i] = 4 * i;
        gl_brick_elements[6 * i + 1] = 4 * i + 1;
        gl_brick_elements[6 * i + 2] = 4 * i + 2;
        gl_brick_elements[6 * i + 3] = 4 * i + 1;
        gl_brick_elements[6 * i + 4] = 4 * i + 2;
        gl_brick_elements[6 * i + 5] = 4 * i + 3;
    }
}

void initColors() {
    // num of points: 4(for each brick) * NUM_ROWS * NUM_COLS
    gl_brick_colors.resize(4 * NUM_COLS * NUM_ROWS);
    for(int i = 0; i < 4 * NUM_ROWS * NUM_COLS; i++) {
        gl_brick_colors[i] = vec3(background_color);
    }
}

vec3 randomColor() {
    return vec3(
        ((double) rand() / 2 / (RAND_MAX)) + 0.5,
        ((double) rand() / 2 / (RAND_MAX)) + 0.5,
        ((double) rand() / 2 / (RAND_MAX)) + 0.5
    );
}
void onShapeFinish() {
    // do something when one shape is finished (touch the buttom of the ground)
    ground.delShape();
    // detect elimination
    int bottomEliminatedRow = NUM_ROWS;
    for(int j = NUM_ROWS - 1; j >= 0; j--) {
        if(ground.isRowFull(j)){
            ground.delRow(j);
            bottomEliminatedRow = j;
        }
    }
    ground.dropAllBricksAboveFromRow(bottomEliminatedRow);
    // if game is not over
    // FIXME: change the init position of shape & use random color
    if(ground.newShape(vec2(NUM_COLS/2, NUM_ROWS-2), randomColor())) {
        // TODO: if new shape can be generated

    } else {
        // TODO: if the game is over: 
        cout << "Game Over" << endl;
        stopGame();
    }
}

void setBrickColor(vec2 position, vec3 color_vector) {
    int i = 4 * vec2ToInt(position);
    if(i < 4 * NUM_COLS * NUM_ROWS && i >= 0) {
        for(int j = 0; j < 4; j++) {
            gl_brick_colors[i+j] = color_vector;
        }
    } else {
        cout << "setBrickColor(): ignore invalid position\n";
    }
}

void printColors() {
    for(int j = NUM_ROWS - 1; j >= 0; j--) {
        for(int i = 0; i < NUM_COLS; i++) {
            int index = 4 * vec2ToInt(vec2(i, j));
            cout << (gl_brick_colors[index] == background_color ? 0 : 1);
        }
        cout << endl;
    }
    cout << "/////////////////////////////////////\n";
}

// update all colors
void updateColors() {
    for(int i = 0; i < NUM_COLS; i++) {
        for(int j = 0; j < NUM_ROWS; j++) {
            vec2 position = vec2(i, j);
            vec3 color = ground.getColor(i, j) != NULL ? *(ground.getColor(i, j)): background_color;
        // FIXME: delete debugging info
#ifdef DEBUG
            if(color[0] != background_color[0]) {
                cout << "set " << position << " color: " << color << endl;
            }
#endif
            setBrickColor(position, color);
        }
    }
}

void autoDropDown(int foo) {
    glutTimerFunc(700.0, autoDropDown, 0);
    if(!gameStatus) {
        return;
    }
    // should be triggered by timer
    if(ground.moveShape('d')) {
        // if not bottom
    } else {
        // bottom
        onShapeFinish();
    }
    updateColors();
        //Always remember to update your canvas
    glutPostRedisplay();

    //then we can set another identical event in 1000 miliseconds in the future, that is how we keep the triangle rotating
}
void onSpecialKeyPressed(int key, int x, int y) {
    if(!gameStatus) {
        return;
    }

    bool success = true;

    switch ( key ) {
    case GLUT_KEY_UP:
        success = ground.rotateShape();
        cout << "up\n";
        break;
    case GLUT_KEY_DOWN:
        //do something here
        while(ground.moveShape('d')) {}
        onShapeFinish();
        cout << "down\n";
        break;
    case GLUT_KEY_LEFT:
        //do something here
        success = ground.moveShape('l');
        cout << "left\n";
        break;
    case GLUT_KEY_RIGHT:
        //do something here
        success = ground.moveShape('r');
        cout << "right\n";
        break;
    }

    if(success){ 
        // update controller
        updateColors();
#ifdef DEBUG
        printColors();
#endif 
        // update gl window
        glutPostRedisplay();
    }
}
void onKeyPressed(unsigned char key, int x, int y) {
    // if failed, just return and don't updateColors
    switch ( key ) {
    case 0x1b:
        // cout << "esc\n";
        exit( EXIT_SUCCESS );
        break;
    case 'r':
        restartGame();
        break;
    }
}

void startGame() {
    if(gameStatus) {
        return;
    }
    gameStatus = true;
    srand(time(NULL));
    ground.clear();
    initGrids();
    initBricks();
    initColors();
    // create the first shape
    onShapeFinish();
    updateColors();

}
void stopGame() {
    if(!gameStatus) {
        return;
    }
    gameStatus = false;
    // refresh the screen to display gameover
    glutPostRedisplay();
}

void restartGame() {
    stopGame();
    startGame();
}