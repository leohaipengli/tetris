#include "controller.h"

bool gameStatus = true;

Ground ground;

vector<vec2> gl_grid_points;
vector<vec3> gl_grid_colors;
vector<vec2> gl_brick_points;
vector<GLint> gl_brick_elements;
vector<vec3> gl_brick_colors;
vector<vec2> gl_gameover_points;

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
GLuint loadBMP_custom(const char *imagepath) {
    // Data read from the header of the BMP file
    unsigned char header[54]; // Each BMP file begins by a 54-bytes header
    unsigned int dataPos;     // Position in the file where the actual data begins
    unsigned int width, height;
    unsigned int imageSize;   // = width*height*3
    // Actual RGB data
    unsigned char * data;
    // Open the file
    FILE * file = fopen(imagepath,"rb");
    if (!file){printf("Image could not be opened\n"); return 0;}
    if ( fread(header, 1, 54, file)!=54 ){ // If not 54 bytes read : problem
        printf("Not a correct BMP file\n");
        return false;
    }
    if ( header[0]!='B' || header[1]!='M' ){
        printf("Not a correct BMP file\n");
            return 0;
    }
    // Read ints from the byte array
dataPos    = *(int*)&(header[0x0A]);
imageSize  = *(int*)&(header[0x22]);
width      = *(int*)&(header[0x12]);
height     = *(int*)&(header[0x16]);
// Some BMP files are misformatted, guess missing information
if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
if (dataPos==0)      dataPos=54; // The BMP header is done that way
// Create a buffer
data = new unsigned char [imageSize];

// Read the actual data from the file into the buffer
fread(data,1,imageSize,file);

//Everything is in memory now, the file can be closed
fclose(file);

// Create one OpenGL texture
GLuint textureID;
glGenTextures(1, &textureID);

// "Bind" the newly created texture : all future texture functions will modify this texture
glBindTexture(GL_TEXTURE_2D, textureID);

// Give the image to OpenGL
glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void initGameover() {
    // points of the rectangle
    gl_gameover_points.resize(4);
    gl_gameover_points[0] = vec2(-0.5, -0.5);
    gl_gameover_points[1] = vec2(0.5, -0.5);
    gl_gameover_points[2] = vec2(-0.5, 0.5);
    gl_gameover_points[3] = vec2(0.5, 0.5);
    // gameover bmp file
    // GLuint loadBMP_custom("src/gameover.bmp");
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
    }
}

// FIXME: bugs here!
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

// FIXME: debug: print 1 if not background color
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
    glutTimerFunc(700.0, autoDropDown, 0);
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
    }
}

void startGame() {
    if(gameStatus) {
        return;
    }

}
void stopGame() {
    if(!gameStatus) {
        return;
    }

}
void restartGame() {
    stopGame();
    startGame();
}