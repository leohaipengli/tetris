#include<iostream>
#include<vector>
#include "include/Angel.h"

#define DEBUG

using namespace std;

// define the number & size of the grid:
const int NUM_ROWS = 20;
const int NUM_COLS = 10;
const int GRID_SIZE = 40;
const vec3 background_color = vec3(0, 0, 0);

vector<vec3> grid_points;
vector<vec3> brick_points;
vector<vec3> brick_colors;
inline GLfloat toGLCoordinate(float in) {
    // in: float 0 ~ 1
    // out: -1 ~ 1
    return (GLfloat)in * 2 - 1;
}

void generateColor() {

    // num of points: 6 (for each brick) * NUM_ROWS * NUM_COLS
    brick_colors.resize(6 * NUM_COLS * NUM_ROWS);
    for(int i = 0; i < 6 * NUM_ROWS * NUM_COLS; i++) {
        brick_colors[i] = vec3((float) rand() / (RAND_MAX), (float) rand() / (RAND_MAX), (float) rand() / (RAND_MAX)); 
    }
}
void generateBrick() {
    // num of points: 6 (for each brick) * NUM_ROWS * NUM_COLS
    brick_points.resize(6 * NUM_COLS * NUM_ROWS);
    int rem = 0, quo = 0;
    for(int i = 0; i < NUM_ROWS * NUM_COLS; i++) {
        rem = i % NUM_COLS;
        quo = i / NUM_COLS;
        brick_points[6*i] = vec3(toGLCoordinate((float)rem/NUM_COLS), toGLCoordinate((float)quo/NUM_ROWS), 0);
        brick_points[6*i+1] = vec3(toGLCoordinate((float)(rem)/NUM_COLS), toGLCoordinate((float)(quo+1)/NUM_ROWS), 0);
        brick_points[6*i+2] = vec3(toGLCoordinate((float)(rem+1)/NUM_COLS), toGLCoordinate((float)(quo)/NUM_ROWS), 0);
        brick_points[6*i+3] = vec3(toGLCoordinate((float)(rem)/NUM_COLS), toGLCoordinate((float)(quo+1)/NUM_ROWS), 0);
        brick_points[6*i+4] = vec3(toGLCoordinate((float)(rem+1)/NUM_COLS), toGLCoordinate((float)(quo)/NUM_ROWS), 0);
        brick_points[6*i+5] = vec3(toGLCoordinate((float)(rem+1)/NUM_COLS), toGLCoordinate((float)(quo+1)/NUM_ROWS), 0);
    }
}
void myInit(void) {

    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

#ifdef DEBUG
    srand(time(NULL));
    generateBrick();
    generateColor();
#else
    // insert vertical grid points
    for(int i = 1; i < NUM_COLS; i++) {
        grid_points.push_back(vec3(toGLCoordinate((float)i/NUM_COLS), -1.0, 0));
        grid_points.push_back(vec3(toGLCoordinate((float)i/NUM_COLS), 1.0, 0));
    }
    // insert horizontal grid points
    for(int i = 1; i < NUM_ROWS; i++) {
        grid_points.push_back(vec3(-1.0, toGLCoordinate((float)i/NUM_ROWS), 0));
        grid_points.push_back(vec3(1.0, toGLCoordinate((float)i/NUM_ROWS), 0));
    }
#endif



#ifdef DEBUG
    // for(auto grid_point = brick_colors.begin(); grid_point != brick_colors.end(); grid_point++) {
    //     cout << *grid_point << endl;
    // }
#endif
    // This will identify our grid buffer
    GLuint grid_buffer;
    // Generate 1 buffer, put the resulting identifier in grid_buffer
    glGenBuffers(1, &grid_buffer);
    // make the buffer 'active' by binding
    glBindBuffer(GL_ARRAY_BUFFER, grid_buffer);
    // send active buffer data
    glBufferData(GL_ARRAY_BUFFER, brick_points.size() * sizeof(vec3), &brick_points.front(), GL_STATIC_DRAW);

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
    glUseProgram( program );

    // Initialize the vertex position attribute from the vertex shader
    GLuint loc = glGetAttribLocation( program, "vPosition" );
    // glEnableVertexAttribArray( loc );
    // glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0,
    //                        BUFFER_OFFSET(0) );

    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );
    // color buffer data
    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, brick_colors.size() * sizeof(vec3), &brick_colors.front(), GL_STATIC_DRAW);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glVertexAttribPointer(
        1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    glClearColor(background_color[0], background_color[1], background_color[2], 1.0); // background
}

//----------------------------------------------------------------------------

void display(void) {
    glClear( GL_COLOR_BUFFER_BIT );     // clear the window
    glDrawArrays(GL_TRIANGLES, 0, brick_points.size()); // Starting from vertex 0; 3 vertices total -> 1 triangle
    // glDrawArrays( GL_POINTS, 0, NumPoints );    // draw the points
    glFlush();
}

//----------------------------------------------------------------------------

void keyboard(unsigned char key, int x, int y) {
    switch ( key ) {
    case 033:
        exit( EXIT_SUCCESS );
        break;
    }
}

//----------------------------------------------------------------------------

int main(int argc, char **argv) {
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA );
    glutInitWindowSize( NUM_COLS * GRID_SIZE, NUM_ROWS * GRID_SIZE );

    // If you are using freeglut, the next two lines will check if 
    // the code is truly 3.2. Otherwise, comment them out
    glutInitContextVersion( 3, 2 );
    glutInitContextProfile( GLUT_CORE_PROFILE );

    glutCreateWindow( "Tetris" );

    // Iff you get a segmentation error at line 34, please uncomment the line below
    glewExperimental = GL_TRUE; 
    glewInit();

    myInit();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );

    glutMainLoop();
    return 0;
}
