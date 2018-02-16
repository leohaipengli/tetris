#include<iostream>
#include<vector>

#include "include/controller.h"

#define DEBUG

using namespace std;

// VAO
GLuint vao_bricks, vao_grids, vao_gameover;


GLuint program, program_gameover;
void deleteBMPData(unsigned char* data);
unsigned char* loadBMPData(const char *imagepath, unsigned int& width, unsigned int& height);

void initGameover(void) {

    // Create Vertex Array Object
    glGenVertexArrays(1, &vao_gameover);
    glBindVertexArray(vao_gameover);

    // Create a Vertex Buffer Object and copy the vertex data to it
    GLuint vbo_gameover;
    glGenBuffers(1, &vbo_gameover);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_gameover);

    // buffer data
    glBufferData( GL_ARRAY_BUFFER, sizeof(gl_gameover_vertices) + sizeof(gl_gameover_colors) + sizeof(gl_gameover_tex_positions),
        gl_gameover_vertices, GL_STREAM_DRAW );

    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(gl_gameover_vertices), gl_gameover_vertices);
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(gl_gameover_vertices), sizeof(gl_gameover_colors), gl_gameover_colors);
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(gl_gameover_vertices) + sizeof(gl_gameover_colors), sizeof(gl_gameover_tex_positions),
        gl_gameover_tex_positions);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(gl_gameover_elements), gl_gameover_elements, GL_STATIC_DRAW);
    

    program_gameover = InitShader("texturevshader.glsl", "texturefshader.glsl");
    glUseProgram(program_gameover);
    // specify the position of vertices, colors and textures in the buffer
    GLuint vPosition = glGetAttribLocation( program_gameover, "vPosition" );
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer( vPosition, 2, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );
    GLuint vColor = glGetAttribLocation( program_gameover, "vColor" );
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(sizeof(gl_gameover_vertices)) );
    GLuint vTexPosition = glGetAttribLocation( program_gameover, "vTexPosition" );
    glEnableVertexAttribArray(vTexPosition);
    glVertexAttribPointer(vTexPosition, 2, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(sizeof(gl_gameover_colors) + sizeof(gl_gameover_vertices)));
                           
    // genetate tex object (same as VAO)
    GLuint tex;
    glGenTextures(1, &tex);

    // Just like other objects, textures have to be bound to apply operations to them. Since images are 2D arrays of pixels, it will be bound to the GL_TEXTURE_2D target.
    glBindTexture(GL_TEXTURE_2D, tex);

    // Give the image to OpenGL
    unsigned width, height;
    unsigned char *data = loadBMPData(GAMEOVER_FILEPATH, width, height);
    if(data == NULL) {
        cerr << "Error Loading BMP file!\n";
        return;
    }

    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    
    glGenerateMipmap(GL_TEXTURE_2D);

    delete[] data;

}
void myInit(void) {
    startGame();
    initGameover();
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
// TODO: bugs
void drawGameover(void) {

    glUseProgram(program_gameover);
    glBindVertexArray( vao_gameover );
    // glDrawArrays( GL_TRIANGLES, 0, gl_brick_points.size());
    glDrawElements(GL_TRIANGLES, sizeof(gl_gameover_elements), GL_UNSIGNED_INT, 0);
    

    // TODO: try mipmaps
    // Note that you do have to load the texture image itself before mipmaps can be generated from it.
    // glGenerateMipmap(GL_TEXTURE_2D);



}
void drawGame(void) {
    // since we use different program, we should switch before drawing
    glUseProgram(program);
    updateBricks();

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
}
void display(void) {
    glClear( GL_COLOR_BUFFER_BIT );     // clear the window
    // draw game whether game is over
    drawGame();
    glFlush();
    if(!gameStatus) {
        // draw game over image if game is over
        drawGameover();
    }
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

void deleteBMPData(unsigned char* data) {
    delete[] data;
}

unsigned char* loadBMPData(const char *imagepath, unsigned int& width, unsigned int& height) {
    // Data read from the header of the BMP file
    unsigned char header[54]; // Each BMP file begins by a 54-bytes header
    unsigned int dataPos;     // Position in the file where the actual data begins
    unsigned int imageSize;   // = width*height*3
    // Actual RGB data
    unsigned char * data;
    // Open the file
    FILE * file = fopen(imagepath,"rb");
    if (!file){printf("Image could not be opened\n"); return 0;}
    if ( fread(header, 1, 54, file)!=54 ){ // If not 54 bytes read : problem
        printf("Not a correct BMP file\n");
        return NULL;
    }
    if ( header[0]!='B' || header[1]!='M' ){
        printf("Not a correct BMP file\n");
            return NULL;
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
    return data;
}