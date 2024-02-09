//Jonathan Wong
//108810712
//Csci 172
//Project 3



#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <SOIL.h>


#define PI 3.14159

using namespace std;
GLUquadricObj *sphere= NULL;
bool WireFrame= false;

    float lightx = 0; //Value for moving the sun
    float lightz = 0; //Value for moving the sun

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
float light_position[] = { cos(lightx), 5.0f, sin(lightz), 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

    GLuint tex;
    float Wwidth,Wheight;
    GLdouble TranslateX,TranslateY,Zoom,RotateX,RotateY,RotateZ;
    float zoom = 0;  //Zoom in value

    float shift = 0; //moves the planets around
    float shift2 = 0; //moves the planets around
    bool start = false; //Begins the animation
    float spin = 0; //spins the sun

    //string location; //Allows the location variable to take in a string
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

 void TextureLoader(char* fileName){
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &tex);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, tex); // images are 2D arrays of pixels, bound to the GL_TEXTURE_2D target.
    int width, height; // width & height for the image reader
    unsigned char* image;
    image = SOIL_load_image(fileName, &width, &height, 0, SOIL_LOAD_RGBA);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
// binding image data
    SOIL_free_image_data(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}





static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
//Edit this to change perspective

    gluLookAt(0, 5.0, 10.0, 0.0, 0.0, -10.0, 0.0, 1.0, 0.0);
    if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME

    // your code here//

    //Earth
    glPushMatrix(); //Pushes the matrix stack down
    TextureLoader("images/Earth.png"); //Image of earth
    glTranslatef ( 0, 0.0,  0); //Starting position
    glScalef ( zoom+0.75, zoom+0.75, zoom+0.75 ); //Zooms in and out of the earth
    glRotatef ( shift2,0, 1 ,0); //Allows the object to rotate
    gluSphere(sphere,3.0, 40,40); //Prints a sphere
    glPopMatrix();  //Pops the matrix within the stack


    //Moon
    glPushMatrix(); //Pushes the matrix stack down
    TextureLoader("images/moon.jpg");   //Image of Moon
    glTranslatef (5.0*cos(shift), 2.0, -3.0*sin(shift));    //Produces a translation and creates the object for sphere, also helps the z axis move for the special keys
    glRotatef ( 0,0, 1 ,0); //Allows the object to rotate
    glScalef ( zoom+1.0, zoom+1.0, zoom+1.0 ); //Zooms in and out of the moon
    gluSphere(sphere,1.0, 10,10);   //Prints a sphere
    glPopMatrix();  //Pops the matrix within the stack


    //Planet
    glPushMatrix(); //Pushes the matrix stack down
    TextureLoader("images/planet.jfif"); //Image of planet
    glTranslatef (10.0*cos(shift), 2.0, 7 * sin(shift));    //Produces a translation and creates the object for sphere, also helps the z axis move for the special keys
    glRotatef ( 0,0, 1 ,0); //Allows the object to rotate
    glScalef ( zoom+1.0, zoom+1.0, zoom+1.0 ); //Zooms in and out of the moon
    gluSphere(sphere,1.0, 10,10);   //Prints a sphere
    glPopMatrix();  //Pops the matrix within the stack



    //Sun
    glDisable(GL_TEXTURE_2D); //Disables GL_TEXTURE_2D for light
    glPushMatrix();
    glRotatef(spin,0,1,0);
    glLightfv(GL_LIGHT0, GL_POSITION ,light_position);
    glPopMatrix();


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
        case ' ': //Space = Start
            start = !start;
            break;

    }
}

void Specialkeys(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_UP:
        zoom += .05;
    break;
    case GLUT_KEY_DOWN:
        zoom -= .05;
    break;
    case GLUT_KEY_LEFT:
        shift += 0.3;
        shift2 += 2.0;
    break;
    case GLUT_KEY_RIGHT:
        shift -= 0.3;
        shift2 -= 2.0;
    break;
   }
  glutPostRedisplay();
}

static void idle(void)
{

    if (start){ //Occurs when start is hit
     shift -= 0.3; //Automatically moves planets
     shift2 -= 2.0;

     spin += 0.5; //Spins light source
     lightx += 0.5; //Moves light source
     lightz += 0.5;

    }


    glutPostRedisplay();

}





static void init(void)
{
    sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_FILL);
    gluQuadricTexture(sphere, GL_TRUE);
    gluQuadricNormals(sphere, GLU_SMOOTH);

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
