/* 
*       Computer Graphics Project #4: SkyBox Design
*
This program uses OpenGL and soil to...
*
*               Instructions to use program
* Zoom In                       : End_Key  (0.2 increment)
* Zoom Out                      : Home_Key (0.2 decrement)
* Rotate Up   (around x - axis) : Up_Key   (5 degree increment)
* Rotate Down (around x - axis) : Down_Key (5 degree increment)
* Rotate Left (around y - axis) : Left_Key (5 degree increment)
* Rotate Right(around y - axis) : Right_Key(5 degree decrement)
* Toggle Wire Frame             : w_Key
*
* Used tutorial "https://www.youtube.com/watch?v=Ya_HESCH1iU" for VS soil setup
*/

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "ExternalLibraries/SOIL2/include/SOIL2/SOIL2.h"//used soil2 library
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <cstring>
#include <vector>

using namespace std;
const bool DEBUG = true; //This turns debugging on/off to display in window 
//const bool DEBUG = false; //This turns debugging on/off to display in window 

//This struct is used to hold values for vertices and normals
typedef struct V {
    float x;
    float y;
    float z;

    //construcor
    V(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
}V;

//This struct is used to hold values for texture values
typedef struct VT {
    float x;
    float y;
    //construcor
    VT(float x, float y) {
        this->x = x;
        this->y = y;
    }
}VT;

vector<V> v;                  //a vector of typedef struct for the vertex indices
vector<V> vn;                //a vector of typedef struct for the vector normal indices
vector<VT> vt;              //a vector of typedef struct for the texture indices

int numFaces = 12; //12 (sets of 3) faces per object file total faces = 36

//light settings
GLfloat position[] = { 0.0f, 1.0f, 1.0f, 0.0f };//for my light
const GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

/* Declare variables for inputs to functions */

//cube position for glTranslatef function
GLfloat cubeX, cubeY, cubeZ;

//rotation angle variables for glRotatef
GLfloat cubeAngleX, cubeAngleY;
GLfloat skyBoxAngleX, skyBoxAngleY;

//size of teapot and cube for glTranslatef function
GLfloat cubeSize;

//scale multiplier (set to 1 when not in use)
GLfloat sMulC;//for cube
GLfloat sbMulC;//for skyBox

//camera position
GLfloat camX;
GLfloat camY;

//light position
GLfloat lightX;
GLfloat lightY;

//bool to trigger the wireframe functionality
bool WireFrame = false;

//a 2D array of ints to hold faces as shown in class
int** F = nullptr;

//to hold ids of buckets (now we need two one for cube, one for skybox) 
GLuint tex[2];

/* GLUT callback Handlers */

//Resises the window(Given by Dr. Dhanyu
static void resize(int width, int height)
{
    double Ratio;

    width = (float)width;
    height = (float)height;

    Ratio = (double)width / (double)height;

    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(60.0f, Ratio, 0.1f, 1500.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

//Draw the cube
void createCube() {
    //create push/pop matrix for Cube
    glPushMatrix();

    glRotatef(cubeAngleX, 1, 0, 0);      //rotates around X axis
    glRotatef(cubeAngleY, 0, 1, 0);      //rotates around Y axis  
    glScalef(sMulC, sMulC, sMulC);      //zoom in/out depending on user input

    glBindTexture(GL_TEXTURE_2D, tex[0]);//bind cube texture to cube
    int j = 0;//for adding to vn, vt, v values 
    //draw the sphere
    for (int i = 0; i < numFaces; i++) {
        glBegin(GL_TRIANGLES);
        for (int c = 0, j = 0; c <= 2; c++) {
            glNormal3f(vn[F[i][2 + j] - 1].x, vn[F[i][2 + j] - 1].y, vn[F[i][2 + j] - 1].z);
            glTexCoord2f(vt[F[i][1 + j] - 1].x, vt[F[i][1 + j] - 1].y);
            glVertex3f(v[F[i][0 + j] - 1].x, v[F[i][0 + j] - 1].y, v[F[i][0 + j] - 1].z);
            j = j + 3;
        }
        glEnd();
    }
    glPopMatrix();
}

//Draw the skyBox
void createSkyBox() {
    //DRAW THE SKYBOX
    glBindTexture(GL_TEXTURE_2D, tex[1]);//bind skyBox texture to skyBox
    glPushMatrix();

    glRotatef(skyBoxAngleX, 1, 0, 0);      //rotates around X axis
    glRotatef(skyBoxAngleY, 0, 1, 0);      //rotates around Y axis  
    glScalef(sbMulC, sbMulC, sbMulC);      //zoom in/out depending on user input

    //had to draw these inverse transpose 
    // 0,0 tex(bottom-left of current face)
    // 1,0 tex(bottom-right of current face)
    // 1,1 tex(top-right of current face)
    // 0,1 tex(top-left of current face)
    // vertex values
    // -1, -1, -1 = v1 = back lower left   = BLL 
    // -1, -1,  1 = v2 = front lower left  = FLL
    // -1,  1, -1 = v3 = back upper left   = BUL
    // -1,  1,  1 = v4 = front upper left  = FUL
    //  1, -1, -1 = v5 = back lower right  = BLR
    //  1, -1,  1 = v6 = front lower right = FLR
    //  1,  1, -1 = v7 = back upper right  = BUR
    //  1,  1,  1 = v8 = front upper right = FUR

    //Draw the back face of the skybox    
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    glTexCoord2f(.75, .667);glVertex3f(-1, -1, -1);
    glTexCoord2f(1, .667); glVertex3f(1, -1, -1);
    glTexCoord2f(1, .333); glVertex3f(1, 1, -1);
    glTexCoord2f(.75, .333); glVertex3f(-1, 1, -1);

    //Draw the left face of the skybox    
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0, .667);      glVertex3f(1, -1, -1);
    glTexCoord2f(.25, .667);    glVertex3f(1, -1, 1);
    glTexCoord2f(.25, .333);    glVertex3f(1, 1, 1);
    glTexCoord2f(0, .333);      glVertex3f(1, 1, -1);

    //Draw the top face of the skybox
    glNormal3f(0, -1, 0);
    glTexCoord2f(.25, .333);    glVertex3f(1, 1, 1);
    glTexCoord2f(.5, .333);     glVertex3f(-1, 1, 1);
    glTexCoord2f(.5, 0);        glVertex3f(-1, 1, -1);
    glTexCoord2f(.25, 0);       glVertex3f(1, 1, -1);

    //Draw the right face of the skybox    
    glNormal3f(1, 0, 0);
    glTexCoord2f(.5, .667);    glVertex3f(-1, -1, 1);
    glTexCoord2f(.75, .667);   glVertex3f(-1, -1, -1);
    glTexCoord2f(.75, .333);   glVertex3f(-1, 1, -1);
    glTexCoord2f(.5, .333);    glVertex3f(-1, 1, 1);

    //Draw the bottom face of the skybox
    glNormal3f(0, 1, 0);
    glTexCoord2f(.25, 1);    glVertex3f(1, -1, -1);
    glTexCoord2f(.5, 1);     glVertex3f(-1, -1, -1);
    glTexCoord2f(.5, .667);  glVertex3f(-1, -1, 1);
    glTexCoord2f(.25, .667); glVertex3f(1, -1, 1);

    //Draw the front face of the skybox
    glNormal3f(0, 0, -1);
    glTexCoord2f(.25, .667);    glVertex3f(1, -1, 1);
    glTexCoord2f(.5, .667);     glVertex3f(-1, -1, 1);
    glTexCoord2f(.5, .333);     glVertex3f(-1, 1, 1);
    glTexCoord2f(.25, .333);    glVertex3f(1, 1, 1);

    glEnd();
    glPopMatrix();
}

//Displays the object and the skybox
static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //creates viewing matrix derived from an eye point, 
    //a reference point indicating the center of the scene, and an UP vector.
    //(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ)
    gluLookAt(camX, camY, 3, camX, camY, 0.0, 0.0, 1.0, 0.0);

    if (WireFrame)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//Draw Our Mesh In Wireframe Mesh
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME

    //CREATE LIGHT
    glPushMatrix();
        glRotatef(lightX, 1, 0, 0);      //rotates around X axis
        glRotatef(lightY, 0, 1, 0);      //rotates around Y axis  
        glTranslatef(2, 0, 0);
        glColor3f(1.0, 0.8, 0.6);
        glLightfv(GL_LIGHT0, GL_POSITION, position);
    glPopMatrix();


    //CREATE SKYBOX
    createSkyBox();

    //CREATE CUBE
    createCube();
    glutSwapBuffers();
}

//Assign functionality for keys 
static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        //press ESC or 'q' to terminate program
    case 27:
    case 'q':
        exit(0);
        break;
        
        //user presses key 'a' and is able to preform a rotation left around the Y axis
    case 'a':
        if (DEBUG) cout << "ROTATE CUBE LEFT AROUND Y AXIS\n";
        cubeAngleY -= 5; //set the angle to be rotated to 5 degrees
        break;

        //user presses key 'd' and is able to preform a rotation right around the Y axis
    case 'd':
        if (DEBUG)  cout << "ROTATE CUBE RIGHT AROUND Y AXIS\n";
        cubeAngleY += 5; //set the angle to be rotated to 5 degrees
        break;

        //user presses key 'w' and is able to preform a rotation up around the X axis
    case 'w':
        if (DEBUG) cout << "ROTATE CUBE UP AROUND X AXIS\n";
        cubeAngleX -= 5; //set the angle to be rotated to 5 degrees
        break;

        //user presses key 's' and is able to preform a rotation down around the X axis
    case 's':
        if (DEBUG) cout << "ROTATE CUBE DOWN AROUND X AXIS\n";
        cubeAngleX += 5; //set the angle to be rotated to 5 degrees
        break;
    
        //user presses key 'j' and is rotate camera left around the Y axis
    case 'j':
        if (DEBUG) cout << "ROTATE CAMERA LEFT AROUND Y AXIS\n";
        camX -= .2;
        break;

        //user presses key 'l' and is rotate camera right around the Y axis
    case 'l':
        if (DEBUG) cout << "ROTATE CAMERA RIGHT AROUND Y AXIS\n";
        camX += .2;
        break;
    
        //user presses key 'i' and is rotate camera up around the X axis
    case 'i':
        if (DEBUG) cout << "ROTATE CAMERA UP AROUND X AXIS\n";
        camY += .2;
        break;

        //user presses key 'k' and is rotate camera down around the X axis
    case 'k':
        if (DEBUG) cout << "ROTATE CAMERA DOWN AROUND X AXIS\n";
        camY -= .2;
        break;
    
        //user presses key 'o' and resets camera to starting position
    case 'o':
        if (DEBUG) cout << "RESET CAMERA POSITION\n";
        camX = 0;
        camY = 0;
        break;

        //user presses key '1' and is rotate light UP around the X axis
    case '1':
        if (DEBUG) cout << "ROTATE LIGHT UP AROUND X AXIS\n";
        lightY += 5;
        break;

        //user presses key '2' and is rotate light down around the X axis
    case '2':
        if (DEBUG) cout << "ROTATE LIGHT DOWN AROUND X AXIS\n";
        lightX -= 5;
        break;
    
        //user presses key '3' and is rotate light down around the Y axis
    case '3':
        if (DEBUG) cout << "ROTATE LIGHT UP AROUND Y AXIS\n";
        lightY += 5;
        break;
    
        //user presses key '4' and is rotate light down around the Y axis
    case '4':
        if (DEBUG) cout << "ROTATE LIGHT DOWN AROUND Y AXIS\n";
        lightY -= 5;
        break;
    
        //user presses key '0' and resets light to starting position
    case '0':
        if (DEBUG) cout << "RESET LIGHT POSITION\n";
        lightX = 0; 
        lightY = 0;
        break;

        //user presses key 'z' and is able to preform a zoom in
    case 'z':
        if (DEBUG) cout << "ZOOM IN\n";
        sMulC += 0.2; //increments by .2 in scaling function (above 1.0 stretches)
        sbMulC += 0.2; //increments by .2 in scaling function (above 1.0 stretches)
        break;

        //user presses key 'x' and is able to preform a zoom out
    case 'x':
        if (DEBUG) cout << "ZOOM OUT\n";
        sMulC -= 0.2; //decrements by .2 in scaling function (below 1.0 shrinks)
        sbMulC -= 0.2; //decrements by .2 in scaling function (below 1.0 shrinks)
        break;

        //user presses key space and is able to reset the scene
    case ' ':
        if (DEBUG) cout << "RESET SCENE\n";
        //camera position
        camX = 0;
        camY = 0;

        //rotation degree variables for glRotatef
        cubeAngleX = 0, cubeAngleY = 0;
        skyBoxAngleX = 0, skyBoxAngleY = 180;
        lightX = 0; lightY = 0;

        //scale multiplier (set to 1 when not in use)
        sMulC = .4;  //for cube
        sbMulC = 6;  //for skyBox
        break;

        //user presses key 'f' and is able to display the cube in wireframe mode
    case 'f':
        if (DEBUG) cout << "\nWireframe..\n";
        WireFrame = !WireFrame;//change the wireframe flag back and forth
        break;
    }
}

//Assign functionality for keys UP, DOWN, RIGHT, LEFT, HOME, END
void Specialkeys(int key, int x, int y)
{
    switch (key)
    {
        //user presses key UP and SKYBOX is able to preform a rotation around the X axis
    case GLUT_KEY_UP:
        if (DEBUG) cout << "ROTATE SKYBOX UP AROUND X AXIS\n";
        skyBoxAngleX -= 5; //set the angle to be rotated to 5 degrees
        break;

        //user presses key DOWN and SKYBOX is able to preform a rotation around the X axis
    case GLUT_KEY_DOWN:
        if (DEBUG) cout << "ROTATE SKYBOX DOWN AROUND X AXIS\n";
        skyBoxAngleX += 5; //set the angle to be rotated to 5 degrees
        break;

        //user presses key LEFT and SKYBOX is able to preform a left rotation around the Y axis
    case GLUT_KEY_LEFT:
        if (DEBUG) cout << "ROTATE SKYBOX LEFT AROUND Y AXIS\n";
        skyBoxAngleY -= 5; //set the angle to be rotated to 5 degrees
        break;

        //user presses key RIGHT and SKYBOX is able to preform a right rotation around the Y axis
    case GLUT_KEY_RIGHT:
        if (DEBUG)  cout << "ROTATE SKYBOX RIGHT AROUND Y AXIS\n";
        skyBoxAngleY += 5; //set the angle to be rotated to 5 degrees
        break;
    }
    glutPostRedisplay();
}

void printLines(string& filename) {
    int i = 0;
    //open the file
    fstream myfile;
    myfile.open(filename);

    string temp;//to hold each line while we sort
    while (getline(myfile, temp)) {
        cout << "Line" << i++ << "          " << temp << "          " << endl;
    }

    myfile.close();
}

static void idle(void)
{
    glutPostRedisplay();
}

//load images for cube and skybox
void myImageLoad() {

    glEnable(GL_TEXTURE_2D);        //enable texture 2d bucket
    glGenTextures(2, tex);         //generate id for bucket
    
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, tex[0]);//this is the texture that we are going to use for cube
    
    int width, height;          //width and height of image
    unsigned char* image;       //loaded image 
    //loads bucket
    image = SOIL_load_image("images/crate.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    if (!image) cout << "image not found" << endl;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

    SOIL_free_image_data(image);//free the image data cause we are done with it
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //loads bucket2
    glBindTexture(GL_TEXTURE_2D, tex[1]);//this is the texture that we are going to use for skybox
    image = SOIL_load_image("images/lost3.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    if (!image) cout << "image not found" << endl;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

    SOIL_free_image_data(image);//free the image data cause we are done with it
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_CLAMP, GL_LINEAR);//helps the image blend
}

//Initialize all variables with starting values 
static void init(void)
{
    //camera position
    camX = 0;
    camY = 0;

    //cube position for glTranslatef function
    cubeX = 0.0, cubeY = 0.0, cubeZ = -2.0;

    //rotation degree variables for glRotatef
    cubeAngleX = 0, cubeAngleY = 0;
    skyBoxAngleX = 0, skyBoxAngleY = 180;

    //scale multiplier (set to 1 when not in use)
    sMulC = .4;  //for cube
    sbMulC = 6;  //for skyBox

    //light position
    lightX = 0;
    lightY = 0;

    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);

    myImageLoad();//loads images
}

void myLoader(bool debug) {

    // print the file if debugging is on
    if (debug) {
        string filename = "Cube.obj";
        printLines(filename);
    }

    //open the object file for cube
    fstream myfile;
    myfile.open("Cube.obj");

    //check that file was opened succsessfully
    if (debug && myfile.is_open()) {
        cout << "File opened for parsing...\n";
    }

    string temp;//creat a string var to hold lines from file 
    string token;//creat a string var to hold token from line
    getline(myfile, temp);//skip first line in file
    getline(myfile, temp);//skip second line in file

    //put each token in its place in its struct

    int i = 0;//a standard counter to keep track of where we are at in the file
    int lineNum = 2;//keeps tack of what line we are on for debugging
    string x, y, z;
    bool done = false;//set flags

    /*Note at this point I tried my damndest to use strtok as Dr. Dhanyu asked
    but his way is c programming, decided to switch to c++ string
    functionality rather than using legacy c functions.  Also I ran into the
    same error, turns out the getline function only will delimit one space,
    so i had to skip an extra space when i found myself at a 0*/
    
    F = new int* [12];    //allocate mem for numFaces number of rows
    //get a line from file and put into temp
    while (getline(myfile, temp) && !done) {
        if (temp.empty()) { continue; }
        if (debug) cout << "Parsing line" << lineNum << ": " << temp << "    ---->    ";

        //Constructs a stringstream object with a copy of temp as content.
        stringstream ss(temp);

        //place tokens in correct struct
        while (getline(ss, token, ' ')) {//read identifier token

            //get the v tokens, convert to float and add to vector
            if (token == "v") {
                //capture x
                getline(ss, token, ' ');
                if (token.empty()) { getline(ss, token, ' '); }
                x = token;

                //capture y
                getline(ss, token, ' ');
                if (token.empty()) { getline(ss, token, ' '); }
                y = token;

                //capture z
                getline(ss, token, ' ');
                if (token.empty()) { getline(ss, token, ' '); }
                z = token;

                //push x, y, z into struct
                v.push_back(V(atof(x.c_str()), atof(y.c_str()), atof(z.c_str())));

                if (debug) cout << "tokens: " << x << "  " << y << "  " << z << "   inVec--->  ";
                if (debug) cout << " x: " << v[i].x << "  y: " << v[i].y << "  z: " << v[i].z << "   " << endl;

                i++; //increment counter for vector index

                //set flag to kill if block, struct is full
                if (lineNum == 9) {
                    if (debug) cout << "done w / v" << endl;
                    //update counter
                    i = 0;
                    lineNum++;//increment line number counter
                }
                lineNum++;//increment line number counter
            }

            //get the vn tokens, convert to float and add to vector
            if (token == "vn") {
                //capture x
                getline(ss, token, ' ');
                if (token.empty()) { getline(ss, token, ' '); }
                x = token;

                //capture y
                getline(ss, token, ' ');
                if (token.empty()) { getline(ss, token, ' '); }
                y = token;

                //capture z
                getline(ss, token, ' ');
                if (token.empty()) { getline(ss, token, ' '); }
                z = token;

                //push x, y, z into struct
                vn.push_back(V(atof(x.c_str()), atof(y.c_str()), atof(z.c_str())));

                if (debug) cout << "tokens: " << x << "  " << y << "  " << z << "   inVec--->  ";
                if (debug) cout << " x: " << vn[i].x << "  y: " << vn[i].y << "  z: " << vn[i].z << "   " << endl;

                i++; //increment counter for vector index

                //set flag to kill if block, struct is full
                if (lineNum == 16) {
                    if (debug) cout << "done w / vn" << endl;
                    //update counter
                    i = 0;
                    lineNum++;//increment line number counter
                }
                lineNum++;//increment line number counter
            }

            //get the vt tokens, convert to float and add to vector
            if (token == "vt") {
                //capture x
                getline(ss, token, ',');
                //if (token.empty()) { getline(ss, token, ' '); }
                x = token;

                //capture y
                getline(ss, token, ',');
                if (token.empty()) { getline(ss, token, ' '); }
                y = token;

                //push x, y into struct
                vt.push_back(VT(atof(x.c_str()), atof(y.c_str())));

                if (debug) cout << "tokens: " << x << "  " << y << "   inVec--->  ";
                if (debug) cout << " x: " << vt[i].x << "  y: " << vt[i].y << "   " << endl;

                i++; //increment counter for vector index

                //set flag to kill if block, struct is full
                if (lineNum == 21) {
                    if (debug) cout << "done w / vt" << endl;
                    if (debug) cout << "done with building structs" << endl;
                    //update counter
                    i = 0;
                    lineNum++;//increment line number counter
                    lineNum++;//increment line number counter(s off skip here)
                    getline(myfile, temp);//(s off skip here)
                }
                lineNum++;//increment line number counter
            }

            //skip the 's off' line
            if (token == "s") {
                i = 0; //set i to correct value for face index start
                getline(ss, token, ' ');
                if (debug) cout << "Skipped line " << lineNum++ << endl;//increment line number counter
            }

            //get the faces 
            if (token == "f") {
                int numSlots = 9;       //9 slots for each index per row
                int numFaces = 12;
                char separators[] = " /s,\t\n";//values we skip in the string 
                char* token, * nextToken, * char_arr;//needed for tokenization process

                //set a flag
                string str_obj(temp);//convert string to char array
                char_arr = &str_obj[0];//convert string to char array (a v, vn, or vt)
                token = strtok_s(char_arr, separators, &nextToken);//get first token
                F[i] = new int[numSlots];  //allocate mem for ind number of columns	
                for (int col = 0; col < numSlots; col++) {
                    F[i][col] = atoi(strtok_s(NULL, separators, &nextToken));//insert
                    if (debug) cout << "inserted " << F[i][col] << " at F[" << i << "][" << col << "]\n";
                }
                i++;
                lineNum++;
                
            }
        }
    }
    getline(myfile, temp);      //skip a line
    myfile.close(); //close the object file
}

/* Program entry point */

int main(int argc, char* argv[])
{
    myLoader(DEBUG);   //debug on/off == true/false set at top of program
    //myLoader(false);   //debug on/off == true/false set at top of program
    glutInit(&argc, argv);
   
    glutInitWindowSize(1000, 900);//starting size of window
    glutInitWindowPosition(1100, 100);//start window pos 
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Project 4: SkyBox Design");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);
    glutIdleFunc(idle);

    glutMainLoop();

    return EXIT_SUCCESS;
}
