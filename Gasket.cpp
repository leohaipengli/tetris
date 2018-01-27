// Two-Dimensional Sierpinski Gasket       
// Generated using randomly selected vertices and bisection

#include "include/Angel.h"


//----------------------------------------------------------------------------

void myInit(void) {
    // vec2 points[NumPoints];

    // // Specifiy the vertices for a triangle
    // vec2 vertices[3] = {
    //     vec2( -1.0, -1.0 ), vec2( 0.0, 1.0 ), vec2( 1.0, -1.0 )
    // };

    // // Select an arbitrary initial point inside of the triangle
    // points[0] = vec2( 0.25, 0.50 );

    // // compute and store N-1 new points
    // for ( int i = 1; i < NumPoints; ++i ) {
    //     int j = rand() % 3;   // pick a vertex at random

    //     // Compute the point halfway between the selected vertex
    //     //   and the previous point
    //     points[i] = ( points[i - 1] + vertices[j] ) / 2.0;
    // }

    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    static const GLfloat g_vertex_buffer_data[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f,  1.0f, 0.0f,
    };

    // This will identify our vertex buffer
    GLuint vertexbuffer;
    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

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
    glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
    // glDrawArrays( GL_POINTS, 0, NumPoints );    // draw the points
    glFlush();
}

//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
    case 033:
        exit( EXIT_SUCCESS );
        break;
    }
}

//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA );
    glutInitWindowSize( 400, 800 );

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
