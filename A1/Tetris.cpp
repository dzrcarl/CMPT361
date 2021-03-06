

#include "Angel.h"

using namespace std;
//#include "data.h"

//It is nice to define and use a color pallete on your software. You can put this information on another utility file.
static vec3  base_colors[] = {
		vec3( 1.0, 0.5, 0.0 ),
		vec3( 0.5, 1.0, 0.0 ),
		vec3( 0.0, 0.0, 1.0 ),
		vec3( 1.0, 1.0, 1.0 )  // white
};

static vec3 tile_colors[] = {
    vec3( 1.0, 0.34, 0.24 ),
    vec3( 1.0, 0.73, 0.24 ),
    vec3( 0.24, 0.42, 0.75 ),
    vec3( 0.2, 0.8, 0.38 ),
    vec3( 0.53, 0.22, 0.75 ),
    vec3( 0.25, 0.36, 0.76 ),
    vec3( 1.0, 0.82, 0.42 )
};

//three triangles
const int NumPoints = 4;

//vertex count
const int TotalNumVertexTile = 16;
const int TotalNumVertexGrid = 800;
const int TotalNumVertex = 816;
const int ColorOffset = TotalNumVertex*sizeof(vec2);

//line counts
const int NumVLine = 11;
const int NumHLine = 21;

// size of a block
const float BlockWidth = 0.176;
const float BlockHeight = 0.092;
// size of the grid
const float Width = 1.76;
const float Height = 1.84;

const int NumOfColor = 7;

int grid[20][10] = {-1};
const bool shapes[28][4] =
{
    {0, 0, 0, 0},
    {0, 1, 1, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0},
    
    {0, 0, 0, 0},
    {1, 1, 1, 1},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    
    {0, 0, 0, 0},
    {0, 0, 1, 1},
    {0, 1, 1, 0},
    {0, 0, 0, 0},
    
    {0, 0, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 1, 1},
    {0, 0, 0, 0},
    
    {0, 0, 0, 0},
    {0, 1, 1, 1},
    {0, 1, 0, 0},
    {0, 0, 0, 0},
    
    {0, 0, 0, 0},
    {0, 1, 1, 1},
    {0, 0, 0, 1},
    {0, 0, 0, 0},
    
    {0, 0, 0, 0},
    {0, 1, 1, 1},
    {0, 0, 1, 0},
    {0, 0, 0, 0},
};

//vertex array for the triangles and for the lines, respectively
GLuint vao, vao1, vao2;

//triangle points and colors arrays
vec2 points[TotalNumVertex];
vec3 colors[TotalNumVertex];

//lines points and colors arrays
vec2 HLinePoints[NumHLine*2];
vec2 VLinePoints[NumVLine*2];
vec3 HLineColor[NumHLine*2];
vec3 VLineColor[NumVLine*2];

//----------------------------------------------------------------------------

void DrawGrid( void ){
    // initializing Horizontal lines
    for(int i = 0; i < NumHLine*2; i++){
        if(i%2 == 0){
            HLinePoints[i] = vec2( -1*Width/2 , Height/2.0 - BlockHeight*(i/2) );
        }
        else{
            HLinePoints[i] = vec2( Width/2 , Height/2.0 - BlockHeight*((i-1)/2) );
        }
        HLineColor[i] = base_colors[3];
    }

    // initializing Vertical lines
    for(int i = 0; i < NumVLine*2; i++){
        if(i%2 == 0){
            VLinePoints[i] = vec2( Width/2.0 - BlockWidth*(i/2) , -1*Height/2 );
        }
        else{
            VLinePoints[i] = vec2( Width/2.0 - BlockWidth*((i-1)/2), Height/2 );
        }
    //cout << VLinePoints[i] << endl;
        VLineColor[i] = base_colors[3];
    }

    //Here we create another vertexArrayObject to render some lines. This is intended to be your grid, so since the
    //grid positions never change, you can leave this vertex array object on the initialization.
    // Create and bind a vertex array object
    glGenVertexArrays( 1, &vao1 );
    glBindVertexArray( vao1 );

    // Create and initialize a buffer object
    GLuint buffer1;
    glGenBuffers( 1, &buffer1 );
    glBindBuffer( GL_ARRAY_BUFFER, buffer1 );
    
    //glBufferData( GL_ARRAY_BUFFER, sizeof(points1), points1, GL_STATIC_DRAW );
    glBufferData( GL_ARRAY_BUFFER, sizeof(HLinePoints) + sizeof(HLineColor), HLinePoints, GL_STATIC_DRAW );

    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(HLinePoints), HLinePoints );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(HLinePoints), sizeof(HLineColor), HLineColor );

    // Load shaders and use the resulting shader program
    GLuint program1 = InitShader( "vshader.glsl", "fshader.glsl" );
    glUseProgram( program1 );

    // Initialize the vertex position attribute from the vertex shader
    GLuint loc1 = glGetAttribLocation( program1, "vPosition" );
    glEnableVertexAttribArray( loc1 );
    glVertexAttribPointer( loc1, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

    GLuint vColor1 = glGetAttribLocation( program1, "vColor" );
    glEnableVertexAttribArray( vColor1 );
    glVertexAttribPointer( vColor1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(HLinePoints)) );

    // Same for Vertical Lines
    glGenVertexArrays( 1, &vao2 );
    glBindVertexArray( vao2 );

    GLuint buffer2;
    glGenBuffers( 1, &buffer2 );
    glBindBuffer( GL_ARRAY_BUFFER, buffer2 );

    glBufferData( GL_ARRAY_BUFFER, sizeof(VLinePoints) + sizeof(VLineColor), VLinePoints, GL_STATIC_DRAW );

    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(VLinePoints), VLinePoints );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(VLinePoints), sizeof(VLineColor), VLineColor );

    GLuint program2 = InitShader( "vshader.glsl", "fshader.glsl" );
    glUseProgram( program2 );

    GLuint loc2 = glGetAttribLocation( program2, "vPosition" );
    glEnableVertexAttribArray( loc2 );
    glVertexAttribPointer( loc2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

    GLuint vColor2 = glGetAttribLocation( program2, "vColor" );
    glEnableVertexAttribArray( vColor2 );
    glVertexAttribPointer( vColor2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(VLinePoints)) );
}

void addTile( void ){

}

void
init( void )
{

    //Just some hard coded data
    int randColor = rand() % NumOfColor;
    int randShape = rand() % 7;
    cout << randColor << endl;
    cout << randShape << endl;

    // loading shape into grid
    bool loadShape[4][4];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            loadShape[i][j] = shapes[randShape*4 + i][j];
            //cout << "loading: " << i << " " << j << " as: " << shapes[randShape*4 + i][j] << endl;
        }
    }
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if(loadShape[i][j]){
                grid[1+i][2+j] = randColor;
                //cout << "adding color: " << i << " " << j << endl;
            }
        }
    }
    
    //***************************

    DrawGrid();

    //****************************

    //glClearColor( 0.3, 0.3, 0.3, 1.0 ); // black background
}

//----------------------------------------------------------------------------

void
display( void )
{
    // Create and bind a vertex array object
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and bind a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    

    //glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points)+sizeof(colors) , NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, TotalNumVertex*sizeof(vec2), points );
    glBufferSubData( GL_ARRAY_BUFFER, ColorOffset, TotalNumVertex*sizeof(vec3), colors );
    

    // Next, we load the real data in parts.  We need to specify the
    //   correct byte offset for placing the color data after the point
    //   data in the buffer.  Conveniently, the byte offset we need is
    //   the same as the size (in bytes) of the points array, which is
    //   returned from "sizeof(points)".
    int renderCounts = 0;

    for (int i = 0; i < 20; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            if(grid[i][j] != -1){
                vec2 vecPoints[4];
                vecPoints[0] = ( (-1)*Width + j * BlockWidth, (-1)*Height + i * BlockHeight );
                vecPoints[1] = ( (-1)*Width + j * BlockWidth, (-1)*Height + (i+1) * BlockHeight );
                vecPoints[2] = ( (-1)*Width + (j+1) * BlockWidth, (-1)*Height + i * BlockHeight );
                vecPoints[3] = ( (-1)*Width + (j+1) * BlockWidth, (-1)*Height + (i+1) * BlockHeight );
                glBufferSubData( GL_ARRAY_BUFFER, (0 + renderCounts * 4)*sizeof(vec2), sizeof(vecPoints), vecPoints );

                vec3 selectedColor = tile_colors[grid[i][j]];
                vec3 vecColors[4] = { selectedColor, selectedColor, selectedColor, selectedColor};
                glBufferSubData( GL_ARRAY_BUFFER, ColorOffset + (renderCounts * 4)*sizeof(vec3), sizeof(vecColors), vecColors );
                cout << "painting: " << i << " " << j << " color is: " << selectedColor << endl;
            }
            //cout << "painting: " << i << " " << j << endl;
        }
    }
    
    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
    glUseProgram( program );

    // Initialize the vertex position attribute from the vertex shader
    // Keep in mind that the second parameter is associated with the length of the array sent to opengl (1D,2D,3D or 4D).
    // The last parameter is the offset where the data is stored on the buffer.
    GLuint loc = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( loc );
    glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );


    // Likewise, initialize the vertex color attribute.  Once again, we
    //    need to specify the starting offset (in bytes) for the color
    //    data.  Just like loading the array, we use "sizeof(points)"
    //    to determine the correct value.
    GLuint vColor = glGetAttribLocation( program, "vColor" );
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(ColorOffset) );


    glClear( GL_COLOR_BUFFER_BIT );     // clear the window
    

    //Draw squares
    //Here we are binding back the first vertex array object. Now we can acess all the buffers associated to it and render accordingly
    glBindVertexArray( vao );
    glDrawArrays( GL_TRIANGLE_STRIP, 0, TotalNumVertex );

    //Draw lines using the second vertex array object. On your tetris code, you probabily want to draw the lines first, then the triangles.
    //If you want to change the thickness of the lines, this is how:  glLineWidth(5.0);    
    glBindVertexArray( vao1 );
    glDrawArrays( GL_LINES, 0, 42 );

    glBindVertexArray( vao2 );
    glDrawArrays( GL_LINES, 0, 22 );

    //Causes all issued commands to be executed as quickly as they are accepted by the actual rendering engine
    glFlush();
}

//----------------------------------------------------------------------------

//timed function. We intended to execute this every one second.
/*
void rotateDelay(int)
{
    //mat2 constructor is defined as follows
    //mat 2 ( GLfloat m00, GLfloat m10, GLfloat m01, GLfloat m11 )

    //We can define a rotation angle in Radians
    GLfloat angle = M_PI / 4;

    //This is the 2x2 rotation matrix for 2D points.
    static mat2 rotate10degccw = mat2 ( cos(angle), sin(angle), -sin(angle), cos(angle) );


    //The rotation transform is applied using the origin as the rotation axis, if you want a different axis you need to define yourself.
    //in this case we are just using the center of the triangle
    vec2 center = vec2(0.0,0.0);

    for ( int i = 3; i < 6; ++i ) {
	center += points[i];
    }

    center = center / 3;

    

    for ( int i = 3; i < 6; ++i ) {
        //finally, we apply the transformation. The idea here is just to translate the points to origin, rotate and then translate it back
	points[i] = rotate10degccw * (points[i] - center) + center;
    }


    //Always remember to update your canvas
    glutPostRedisplay();

    //then we can set another identical event in 1000 miliseconds in the future, that is how we keep the triangle rotating
    glutTimerFunc(1000.0, rotateDelay, 0);
}
*/

//If you want to use arrows, take a look at this function
//It works similarly, but you have no acess to the ASCII keys using it. Please see the link below for details.
//https://www.opengl.org/resources/libraries/glut/spec3/node54.html
void keyboardSpecial( int key, int x, int y )
{
    switch ( key ) {
    //similar to the stuff inside the keyboard class, but applying the translaton only to the third triangle.
    case  GLUT_KEY_DOWN:
         printf("down arrow pressed!\n");
         static vec2 downStep = vec2( 0.0, -0.1 );

         for ( int i = 6; i < 9; ++i ) {
		points[i] = points[i] + downStep;
	 }
         glutPostRedisplay();
         break;
    case 033:
        exit( EXIT_SUCCESS );
        break;
    }
    
}

//bind most keys on your keyboard to this function
void
keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
    case 115:
         printf("w pressed!\n");

         //this is just a simple 2D array that represents a vector pointing downwards
         static vec2 downStep = vec2( 0.0, -0.4 );

         //you can apply this transformation to specific points, in this case, the vertices of the first triangle
         for ( int i = 0; i < 3; ++i ) {
                //notice that you can just sum 2D arrays directly! This makes things more easy.
		points[i] = points[i] + downStep;
	 }
         //Always remember to call glutpostRedisplay after changes. It will just call the "display" method again.
         glutPostRedisplay();
         break;

    //Esc pressed!
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
    //We explicitly tell OpenGL that we are using the RGBA mode. When we start working with 3D resources there will be changes here.
    glutInitDisplayMode( GLUT_RGBA );

    //Change the size of the screen here. Keep in mind that the coordinates will also change accordingly
    //so, if you want to keep your points on the rendering canvas undisturbed you need a square screen.
    glutInitWindowSize( 340, 640 );

    // If you are using freeglut, the next two lines will check if 
    // the code is truly 3.2. Otherwise, comment them out
    glutInitContextVersion( 3, 2 );
    glutInitContextProfile( GLUT_CORE_PROFILE );

    //Title of your screen, change accordingly
    glutCreateWindow( "Gasket" );

    // Iff you get a segmentation error at line 34, please uncomment the line below
    glewExperimental = GL_TRUE; 
    glewInit();

    //this is the initialization function for your data.
    init();

    //Here we bind the function "display" to the OpenGL handler that will render your data
    glutDisplayFunc( display );

    //Similarly, we can also bind a function that will be executed 1000 miliseconds later. We bind this to the function rotateDelay.
    // We then bind this to the function "rotateDelay". The 0 in the end is the input of the rotateDelay class, it can only be an integer.
    // glutTimerFunc(1000.0, rotateDelay, 0);

    //Finally, we bind the keyboard events inside the class "keyboard" using the line below
    glutKeyboardFunc( keyboard );

    //If you really want to use F1 ~ F9 keys, or the arrow keys, then use the line of code below.
    glutSpecialFunc( keyboardSpecial );

    glutMainLoop();
    return 0;
}
