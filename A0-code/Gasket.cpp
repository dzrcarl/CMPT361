// Two-Dimensional Sierpinski Gasket       
// Generated using randomly selected vertices and bisection

#include "Angel.h"

static vec3 base_colors[] = {
    vec3(1.0,0.0,0.0),
    vec3(0.0,1.0,0.0)
}

const int NumPoints = 9;

//----------------------------------------------------------------------------

void
init( void )
{
    vec2 points[NumPoints];

    // three triangles
    points[0] = vec2(-1.0,-1.0);
    points[1] = vec2(0.0,-1.0);
    points[1] = vec2(-0.5,0.0);

    points[3] = vec2(0.0,-1.0);
    points[4] = vec2(0.5,0.0);
    points[5] = vec2(1.0,-1.0);

    points[6] = vec2(0.0,-1.0);
    points[7] = vec2(-0.5,0.0);
    points[8] = vec2(0.5,0.0);

    // color
    vec2  colors[NumPoints];
    colors[0] = base_colors[0];
    colors[1] = base_colors[0];
    colors[2] = base_colors[0];

    colors[3] = base_colors[1];
    colors[4] = base_colors[1];
    colors[5] = base_colors[1];

    colors[6] = base_colors[1];
    colors[7] = base_colors[1];
    colors[8] = base_colors[1];


    glClearColor( 1.0, 1.0, 1.0, 1.0 ); // white background
}

//----------------------------------------------------------------------------

void
display( void )
{
    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );

    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(points), colors );

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
    glUseProgram( program );

    // Initialize the vertex position attribute from the vertex shader
    GLuint loc = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( loc );
    glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );

    glClear( GL_COLOR_BUFFER_BIT );     // clear the window
    glDrawArrays( GL_TRAIANGLES, 0, NumPoints );    // draw the points
    glFlush();
}

//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {

    case 155:
        printf("go down\n");

        static vec2 downStep vec2(0.0, -0.4);

        for (int i = 0; i < 3; ++i)
        {
            points[i] = points[i] + downStep;
        }

        //glutDisplayFunc( display );
        glutPostRedisplay();
        break;

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
    glutInitWindowSize( 512, 512 );

    // If you are using freeglut, the next two lines will check if 
    // the code is truly 3.2. Otherwise, comment them out
    glutInitContextVersion( 3, 2 );
    glutInitContextProfile( GLUT_CORE_PROFILE );

    glutCreateWindow( "Sierpinski Gasket" );

    // Iff you get a segmentation error at line 34, please uncomment the line below
    //glewExperimental = GL_TRUE; 
    glewInit();

    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );

    glutMainLoop();
    return 0;
}
