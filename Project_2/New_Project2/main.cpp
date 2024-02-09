//Jonathan Wong
//ID: 108801712
//Computer Graphics Project #2: Modeling Transformations



#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <iostream>

#include <math.h>
using namespace std;

bool WireFrame= false;

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };


/* GLUT callback Handlers */

static void resize(int width, int height)
{
     double Ratio;

   if(width<=height)
            glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);
    else
          glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective (50.0f,1,0.1f, 100.0f);
 }


//float a = -3.5;
float cubez = -2.0;
float teaz = -3.0;
float spherez = -1.0;
float cubeangle = 0.0;
float teaangle = 0.0;
float sphereangle = 0.0;
string location; //Allows the location variable to take in a string
static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0,5,10,0.0,0.0,0.0,0.0,1.0,0.0);

    if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME

    // your code here
    //void glutSolidCube ( GLdouble size );     // size 2
    //void glutSolidTeapot ( GLdouble size );   // size 1.5
    //void glutSolidSphere ( GLdouble radius, GLint slices, GLint stacks); // Radius 1.3


    //void glRotatef ( GLfloat angle, GLfloat x, GLfloat y, GLfloat z );

    glPushMatrix(); //Pushes the matrix stack down
    glTranslatef (4.0, 0.0, cubez); //Produces a translation and creates the object for cube, also helps the z axis move for the special keys
    glColor3f(0,50,0); //Changes the color of the cube
    glRotatef ( cubeangle,0, 1 ,0); //Allows the object to rotate through the y-axis
    glutSolidCube ( 2 );
    glPopMatrix(); //Pops the matrix within the stack
    //glTranslatef (4.0, 0.0, -2.0);

    glPushMatrix(); //Pushes the matrix stack down
    glTranslatef (0.0, 0.0, teaz);  //Produces a translation and creates the object for the teapot, also helps the z axis move for the special keys
    glRotatef ( teaangle,0, 1 ,0);  //Allows the object to rotate through the y-axis
    glColor3f(50,0,0); //Changes the color of the Teacup
    glutSolidTeapot ( 1.5 );
    glPopMatrix();  //Pops the matrix within the stack
    //glTranslatef (0.0, 0.0, -3.0);

    glPushMatrix(); //Pushes the matrix stack down
    glTranslatef ( -3.5, 0.0,  spherez);    //Produces a translation and creates the object for sphere, also helps the z axis move for the special keys
    glRotatef ( sphereangle,0, 1 ,0); //Allows the object to rotate through the y-axis
    glColor3f(0,0,50);  //Changes the color of the Sphere
    glutSolidSphere ( 1.3, 200, 200);
    glPopMatrix();  //Pops the matrix within the stack

    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;
        case 's': //Sphere
            location = "sphere"; //the string inside of location will be set to sphere so that it will be read from the special key functions in order to move specifically the sphere object only
            break;
        case 't': //Teapot
            location = "tea"; //the string inside of location will be set to teacup so that it will be read from the special key functions in order to move specifically the teacup object only
            break;
        case 'c': //cube
            location = "cube";  //the string inside of location will be set to cube so that it will be read from the special key functions in order to move specifically the cube object only
            break;


    }
}

void Specialkeys(int key, int x, int y)
{


   //Left  && Right = rotate
   //Up && Down = Z axis (Forward and back)
   //up down left right spacebar
    switch(key)
    {
        //void glRotatef ( GLfloat angle, GLfloat x, GLfloat y, GLfloat z );
    case GLUT_KEY_UP: //Special case for when the up key is pressed
        if (location == "sphere"){spherez += 0.5;} //If the user is holding down the S key and up key, the sphere would move towards the sceen
        if (location == "tea"){teaz += 0.5;}    //If the user is holding down the T key and up key, the teacup would move towards the sceen
        if (location == "cube"){cubez += 0.5;}  //If the user is holding down the C key and up key, the cube would move towards the sceen
    break;
    case GLUT_KEY_DOWN: //Special case for when the down key is pressed
        if (location == "sphere"){spherez -= 0.5; } //If the user is holding down the S key and up key, the cube would move away from the sceen
        if (location == "tea"){teaz -= 0.5;}    //If the user is holding down the T key and up key, the cube would move away from the sceen
        if (location == "cube"){cubez -= 0.5;}  //If the user is holding down the C key and up key, the cube would move away from the sceen
    break;
    case GLUT_KEY_LEFT: //Special case for when the Left key is pressed
        if (location == "cube"){cubeangle += 5.0;}  //If the user is holding down the C key and up key, the Cube would rotate in the left direction
        if (location == "sphere") {sphereangle += 5.0;} //If the user is holding down the S key and up key, the Sphere would rotate in the left direction
        if (location == "tea") {teaangle += 5.0;}   //If the user is holding down the T key and up key, the Teacup would rotate in the left direction
    break;

    case GLUT_KEY_RIGHT:    //Special case for when the Right key is pressed
        if (location == "cube"){cubeangle -= 5.0;}  //If the user is holding down the C key and up key, the cube would rotate in the right direction
        if (location == "sphere") {sphereangle -= 5.0;} //If the user is holding down the S key and up key, the sphere would rotate in the right direction
        if (location == "tea") {teaangle -= 5.0;}   //If the user is holding down the T key and up key, the teacup would rotate in the right direction
    break;

   }
  glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

static void init(void)
{
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
}


/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(800,600);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Shapes");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);
    glutMainLoop();

    return EXIT_SUCCESS;
}
