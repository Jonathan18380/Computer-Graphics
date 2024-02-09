#include <string.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
/* GLUT callback Handlers */

float a,b,c,d,e,f;          //Floating variables needed for drawing the image
float x,y,x0,y0;
int it;                     //Variable for determining what the image will look like

void resize(int width, int height)
{
    if(width<=height)
        glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);

    else
        glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);
}

void init()
{

    x0=1;
    y0=1;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2.5,2.75,10.5,0, -1.0, 1.0); // adjusted for suitable viewport

}

void display(void)                     // Renderer
{
  glClear (GL_COLOR_BUFFER_BIT);        // clear display screen
  // your code here
    for(int i = 0; i < 200000; i++){    //Loops 200,000 times
        it = rand()%100+1;              //Randomly generates a number from 1-100 to determine the imagine
        if(it <= 7) {a= -0.15, b =0.26, c =0.28, d =0.24, e =0, f=0.44;}    //coordinates depending on the result of the random number generator
        else if (it <= 14){a= 0.2, b =0.23, c =-0.26, d =0.22, e =0, f=1.6;}
        else if (it <= 99){a= 0.85, b = -0.04, c =0.04, d =0.85, e =0, f=1.6;}
        else {a= 0, b =0, c =0,d =0.16, e =0, f=0;}
        x = a*x0+c*y+e;                 //Formulas to transform the image
        y = b*x0+d*y0+f;

        glColor3f(1.0,0.5,3.0);         //Determine the color of the imagine (Purple in this case)
        glBegin(GL_POINTS);             //Interprets and draws points to create image
        glVertex3f(x,y,0);              //A vertex that takes 3 floating point numbers
        glEnd();                        //Delimits the vertices
        x0=x;                           //Changes the old X with new X
        y0=y;
    }

  glFlush ();                           // clear buffer
}


void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :                       // esc key to exit
        case 'q':
            exit(0);
            break;
    }

    glutPostRedisplay();
}


/* Program entry point */

int main(int argc, char *argv[])
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (600, 800);                //window screen
   glutInitWindowPosition (100, 100);            //window position
   glutCreateWindow ("Program1");                //program title
   init();
   glutDisplayFunc(display);                     //callback function for display
   glutReshapeFunc(resize);                      //callback for reshape
   glutKeyboardFunc(key);                        //callback function for keyboard
   glutMainLoop();                               //loop

    return EXIT_SUCCESS;
}
