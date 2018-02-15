#include<iostream>
#include<vector>

#include "include/controller.h"

#define DEBUG

using namespace std;

// VAO
GLuint vao_bricks, vao_grids;


GLuint program;

void myInit(void) {
    srand(time(NULL));
    initGrids();
    initBricks();
    initColors();
    // create the first shape
    onShapeFinish();
    updateColors();
    glGenVertexArrays( 1, &vao_grids );
    glBindVertexArray( vao_grids );


    // draw the grid lines
    // Create and initialize a buffer object
    GLuint buffer1;
    glGenBuffers( 1, &buffer1 );
    glBindBuffer( GL_ARRAY_BUFFER, buffer1 );
    
    //glBufferData( GL_ARRAY_BUFFER, sizeof(points1), points1, GL_STATIC_DRAW );
    glBufferData( GL_ARRAY_BUFFER, sizeof(vec3) * gl_grid_colors.size() + gl_grid_points.size() * sizeof(vec2), &gl_grid_points.front(), GL_STREAM_DRAW );

    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vec2) * gl_grid_points.size(), &gl_grid_points.front());
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(vec2) * gl_grid_points.size(), sizeof(vec3) * gl_grid_colors.size(), &gl_grid_colors.front());

    // Load shaders and use the resulting shader program
    program = InitShader( "vshader.glsl", "fshader.glsl" );
    glUseProgram( program );

    // Initialize the vertex position attribute from the vertex shader
    GLuint loc1 = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( loc1 );
    glVertexAttribPointer( loc1, 2, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );

    GLuint vColor1 = glGetAttribLocation( program, "vColor" );
    glEnableVertexAttribArray( vColor1 );
    glVertexAttribPointer( vColor1, 3, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(sizeof(vec2) * gl_grid_points.size()));

    glClearColor(background_color[0], background_color[1], background_color[2], 1.0); // background

}

//----------------------------------------------------------------------------

void updateBricks() {
    // Create and bind a vertex array object
    glGenVertexArrays( 1, &vao_bricks );
    glBindVertexArray( vao_bricks );

    // Create and bind a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );

    glBufferData( GL_ARRAY_BUFFER, sizeof(vec3) * (gl_brick_colors.size() + gl_brick_points.size()), &gl_brick_points.front(), GL_STATIC_DRAW );

    // Next, we load the real data in parts.  We need to specify the
    //   correct byte offset for placing the color data after the point
    //   data in the buffer.  Conveniently, the byte offset we need is
    //   the same as the size (in bytes) of the points array, which is
    //   returned from "sizeof(points)".
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vec3) * gl_brick_points.size(), &gl_brick_points.front());
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(vec3) * gl_brick_points.size(), sizeof(vec3) * gl_brick_colors.size(), &gl_brick_colors.front());


    // Create element buffers
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * gl_brick_elements.size(), &gl_brick_elements.front(), GL_STATIC_DRAW);
    
    // Load shaders and use the resulting shader program if we need new shaders
    // program = InitShader( "vshader.glsl", "fshader.glsl" );
    // glUseProgram( program );

    // Initialize the vertex position attribute from the vertex shader
    // Keep in mind that the second parameter is associated with the length of the array sent to opengl (1D,2D,3D or 4D).
    // The last parameter is the offset where the data is stored on the buffer.
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 2, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );


    // Likewise, initialize the vertex color attribute.  Once again, we
    //    need to specify the starting offset (in bytes) for the color
    //    data.  Just like loading the array, we use "sizeof(points)"
    //    to determine the correct value.
    GLuint vColor = glGetAttribLocation( program, "vColor" );
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 3, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(sizeof(vec3) * gl_brick_points.size()));

}
void display(void) {
    cout << "display() is called" << endl;

    updateBricks();

    glClear( GL_COLOR_BUFFER_BIT );     // clear the window
    

    //Draw triangles
    //Here we are binding back the first vertex array object. Now we can acess all the buffers associated to it and render accordingly
    glBindVertexArray( vao_bricks );
    // glDrawArrays( GL_TRIANGLES, 0, gl_brick_points.size());
    glDrawElements(GL_TRIANGLES, gl_brick_elements.size(), GL_UNSIGNED_INT, 0);

    //Draw lines using the second vertex array object. On your tetris code, you probabily want to draw the lines first, then the triangles.
    //If you want to change the thickness of the lines, this is how:  glLineWidth(5.0);    
    // FIXME:
    glBindVertexArray( vao_grids );
    glDrawArrays( GL_LINES, 0, gl_grid_points.size());

    //Causes all issued commands to be executed as quickly as they are accepted by the actual rendering engine
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
    glutSpecialFunc( onSpecialKeyPressed );

    glutTimerFunc(700.0, autoDropDown, 0);

    glutMainLoop();
    return 0;
}
