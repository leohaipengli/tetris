#include<iostream>
#include<vector>

#include "include/controller.h"

#define DEBUG

using namespace std;

// VAO
GLuint vao;

void myInit(void) {
    srand(time(NULL));
    initGrids();
    initBricks();
    initColors();
    // create the first shape
    onShapeFinish();
    updateColors();

    glClearColor(background_color[0], background_color[1], background_color[2], 1.0); // background

}

//----------------------------------------------------------------------------

void display(void) {
    // Create a vertex array object
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );
    // This will identify our grid buffer
    GLuint grid_buffer;
    // Generate 1 buffer, put the resulting identifier in grid_buffer
    glGenBuffers(1, &grid_buffer);
    // make the buffer 'active' by binding
    glBindBuffer(GL_ARRAY_BUFFER, grid_buffer);
    // send active buffer data
    glBufferData(GL_ARRAY_BUFFER, gl_brick_points.size() * sizeof(vec3), &gl_brick_points.front(), GL_STATIC_DRAW);

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
    glBufferData(GL_ARRAY_BUFFER, gl_brick_colors.size() * sizeof(vec3), &gl_brick_colors.front(), GL_STATIC_DRAW);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

    glClear( GL_COLOR_BUFFER_BIT );     // clear the window

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

    glDrawArrays(GL_TRIANGLES, 0, gl_brick_points.size()); // Starting from vertex 0; 3 vertices total -> 1 triangle
    // glDrawArrays( GL_POINTS, 0, NumPoints );    // draw the points
    glFlush();
}

//----------------------------------------------------------------------------

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
    glutKeyboardFunc( onKeyPressed );

    glutMainLoop();
    return 0;
}
