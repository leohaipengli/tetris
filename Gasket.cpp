#include<iostream>
#include<vector>
#include "include/Angel.h"

// #define DEBUG

using namespace std;

// define the number & size of the grid:
const int NUM_ROWS = 20;
const int NUM_COLS = 10;
const int GRID_SIZE = 40;

vector<vec3> grid_points;

inline GLfloat toGLCoordinate(float in) {
    // in: float 0 ~ 1
    // out: -1 ~ 1
    return (GLfloat)in * 2 - 1;
}

void myInit(void) {

    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );


    vec2 tps[4];

    // insert vertical grid points
#ifdef DEBUG
    tps[0] = vec2(0, 0);
    tps[1] = vec2(0.1, 0.1);
    tps[2] = vec2(0.2, 0.2);
    tps[3] = vec2(0.3, 0.3);
    grid_points.push_back(vec3(-0.5f, -0.5f, 0));
    grid_points.push_back(vec3(1.0f, -1.0f, 0));
    grid_points.push_back(vec3(0.0f,  1.0f, 0));
    grid_points.push_back(vec3(1.0f,  0.0f, 0));
#else
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


    // vec3 points[NumPoints];

#ifdef DEBUG

    // ???
    // for(vec2 grid_point: grid_points) {
    //     cout << *grid_point << endl;
    // }
    for(vector<vec3>::iterator grid_point = grid_points.begin(); grid_point != grid_points.end(); grid_point++) {
        cout << *grid_point << endl;
    }
#endif

    // This will identify our vertex buffer
    GLuint vertexbuffer;
    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // Give our vertices to OpenGL.
    // glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, grid_points.size() * sizeof(vec3), &grid_points.front(), GL_STATIC_DRAW);

    // // Create and initialize a buffer object
    // GLuint buffer;
    // glGenBuffers( 1, &buffer );
    // glBindBuffer( GL_ARRAY_BUFFER, buffer );
    // glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
    glUseProgram( program );

    // Initialize the vertex position attribute from the vertex shader
    GLuint loc = glGetAttribLocation( program, "vPosition" );
    // glEnableVertexAttribArray( loc );
    // glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0,
    //                        BUFFER_OFFSET(0) );

    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( loc, 3, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );

    glClearColor( 0, 0, 0, 1.0 ); // white background
}

//----------------------------------------------------------------------------

void display(void) {
    glClear( GL_COLOR_BUFFER_BIT );     // clear the window
    glDrawArrays(GL_LINES, 0, grid_points.size()); // Starting from vertex 0; 3 vertices total -> 1 triangle
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
