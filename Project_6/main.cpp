//Jonathan Wong
//108801712
//Project 6 - Bunny
/*
Load bunny into the room and write code to calculate normals (apply vertex normals)

bind light move to the mouse

bunny-3.obj Download bunny-3.obj sadsaa.objDownload sadsaa.obj

 glPushMatrix ();
            GLfloat position[] = { 0.0, 0.0, 3.5, 1.0 };
            glRotatef (spin, 0.0, 1.0, 0.0);
            glLightfv (GL_LIGHT0, GL_POSITION, position);
      glPopMatrix();

Light Move  in spin example
*/


#include <string.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <iostream>

#include <math.h>
#include <SOIL.h>
#include <fstream>
#include <vector>
#include <cstdlib>




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

GLuint tex;

//Variables used to manipulate the model and skybox
int rotBox_x = 0;
int rotBox_y = 0;
int rotBox_z = 0;
int rotMod_x = 0;
int rotMod_y = 0;



//Variables used to move the camera and mouse
float rot_x = 0;
float rot_y = 0;
float move_x = 0;
float move_y = 0;
int inout = 0;
float inoutObj = 0.0;
string filename1;

bool showmodel = true;



//Struct that holds the values of the vertexs and normals
struct v3{
    public:
    float a3;
    float b3;
    float c3;
    v3(float a, float b, float c){
        a3 = a;
        b3 = b;
        c3 = c;
    }
};
//Struct that holds the values of textures
struct v2{
    public:
    float a2;
    float b2;
    v2(float a, float b){
        a2 = a;
        b2 = b;

    }
};
//Struct that holds the values of face
struct faces{
    public:
    int a1;
    int b1;
    int c1;
    int a2;
    int b2;
    int c2;
    int a3;
    int b3;
    int c3;
    faces(int v1, int v2, int v3, int vt1, int vt2, int vt3, int vn1, int vn2, int vn3){

        a1 = v1;
        b1 = v2;
        c1 = v3;
        a2 = vt1;
        b2 = vt2;
        c2 = vt3;
        a3 = vn1;
        b3 = vn2;
        c3 = vn3;
    }
};
struct var1{
    public:
        int a;
        int a1;
        int a2;
        int b;
        int b1;
        int b2;
        var1(int a,int a1, int a2, int b, int b1, int b2){
            a = a;
            a1 = a1;
            a2 = a2;
            b = b;
            b1 = b1;
            b2 = b2;
        }
};
struct var2{
    public:
        int c;
        int d;
        int c1;
        int d1;
        int c2;
        int d2;
        int c3;
        int d3;
        var2(int c,int d, int c1, int d1, int c2, int d2,int c3, int d3){
            c = c;
            c1 = c1;
            c2 = c2;
            c3 = c3;
            d = d;
            d1 = d1;
            d2 = d2;
            d3 = d3;
        }
};
struct var3{
    public:
        int a;
        int a1;
        int a2;
        int b;
        int b1;
        int b2;
        var3(int v,int v1, int v2, int s, int s1, int s2){
            a = v;
            a1 = v1;
            a2 = v2;
            b = s;
            b1 = s1;
            b2 = s2;
        }
};

//Vector that holds every line in the file
vector<string> value;
//Vector used to hold the values of the Vertex in each file
vector<v3> vertexs;
//Vector used to hold the values of the normals in each file
vector<v3> normals;
//Vector used to hold the values of the textures in each file
vector<v2> textures;
//Vector used to hold the values of the faces in each file
vector<faces> face;
//Holds variants of faces if there's no texture but 3 face values
vector<var3> variants;
//Holds variants of faces if there's no texture but 4 face values
vector<var2> variants2;

//vector to hold our new faces
vector<faces>new_face;


//v3 = struct that holds the values of normal
//Subtract vertices
v3 vert_sub(v3 p1, v3 p2){
    v3 sub = v3(0, 0, 0);
    sub.a3 = p1.a3 - p2.a3;
    sub.b3 = p1.b3 - p2.b3;
    sub.c3 = p1.c3 - p2.c3;
    return sub;
}


//We need a cross product value
//v3 = struct that holds the values of normal
//Cross multiplies
v3 vert_mult(v3 p1, v3 p2){
    v3 xmult = v3(0, 0, 0);
    xmult.a3 = p1.b3 * p2.c3 - p1.c3 * p2.b3;  //p1.c3 = Uz    //p2.c3 = Vz   //Nx = xmult.a3
    xmult.b3 = p1.c3 * p2.a3 - p1.a3 * p2.c3;  //p1.b3 = Uy    //p2.b3 = Vy   //Ny = xmult.b3
    xmult.c3 = p1.a3 * p2.b3 - p1.b3 * p2.a3;  //p1.a3 = Ux    //p2.a3 = Vx   //Nz = xmult.c3
    return xmult;
}
//Gets our new normal
v3 get_normal(v3 p1, v3 p2, v3 p3){
    v3 normal = v3(0, 0, 0);
    //U = P2 - P1
    v3 U = vert_sub(p2, p1);
    //V = P1 - P3
    v3 V = vert_sub(p1, p3);
    //N = U X V
    normal = vert_mult(U, V);

    return normal;
}


//Nx = Uy * Vz  -  Uz * Vy
//Ny = Uz * Vx  -  Ux * Vz
//Nz = Ux * Vy  -  Uy * Vx



    //This function will be able to add normals to our faces that has no normals
    void add_normals(){  //This will only be for triangles
        if(normals.size() == 0){  //Check if there are any normals, if not then we can add them, otherwise ignore this function
            //These loops are for vertex faces without textures
            for(int i = 0; i < face.size(); i++){
                v3 normalA = get_normal(vertexs[face[i].a1], vertexs[face[i].b1], vertexs[face[i].c1]);
                v3 normalB = get_normal(vertexs[face[i].b1], vertexs[face[i].c1], vertexs[face[i].a1]);
                v3 normalC = get_normal(vertexs[face[i].c1], vertexs[face[i].a1], vertexs[face[i].b1]);
                normals.push_back(v3(normalA.a3, normalA.b3, normalA.c3));
                normals.push_back(v3(normalB.a3, normalB.b3, normalB.c3));
                normals.push_back(v3(normalC.a3, normalC.b3, normalC.c3));
                new_face.push_back(faces(face[i].a1, face[i].b1, face[i].c1, face[i].a2, face[i].b2, face[i].c2, i*3, i*3+1, i*3+2));

            }
            //Empty face because we have new normals
            while(!face.empty()){
                face.pop_back();
            }
            //Pushes the values inside of "new_face" back into the face vector with the new VN
            for(int i=0; i < new_face.size();i++){
                face.push_back(new_face[i]);

            }



        }
    }








    //Function that takes the file and reads off the value from said file line by line.
    void readFile(string filename){
        string vert;
        float verticies;
        float texture;
        float normal;
        float a,b,c,a1,b1,c1,a2,b2,c2,a3,b3,c3;
        fstream myfile;

        while(textures.size() > 0){
            textures.pop_back();
        }
        while(normals.size() > 0){
            normals.pop_back();
        }
        while(face.size() > 0){
            face.pop_back();
        }
        while(value.size() > 0){
            value.pop_back();
        }
        while(vertexs.size() > 0){
            vertexs.pop_back();
        }
        while(variants.size() > 0){
            variants.pop_back();
        }
        while(variants2.size() > 0){
            variants2.pop_back();
        }

        //gets file open without it become a constructor
        myfile.open(filename,ios::in);

        //Checks if file is opened or not, will return an error message if it's not open
        if (!myfile.is_open()){
            cout << "file can't open" << endl;
            return;
            }
        //Reads until end of file
        int counter = 0;

        while(!myfile.eof())
            {
                //Get each line in file
                getline(myfile, vert);
                //Pushes each line into a vector
                value.push_back(vert);
        }

        myfile.close();

        //Compares each line in the file and organizes them into their own vectors.
        int i = 0;
        for(string &vert : value ){
            //Checking the file for vertex values
            if(vert.front() == 'v'){
                    if(vert.at(1) == ' '){
                            sscanf(vert.c_str(),"v %f %f %f", &a, &b, &c );
                            vertexs.push_back(v3(a,b,c));

                    }
                    //Checking the file for texture values
                    if(vert.at(1) == 't'){
                            sscanf(vert.c_str(),"vt %f %f", &a, &b);
                            textures.push_back(v2(a,b));
                    }
                    //Checking the file for normal values

                    if(vert.at(1) == 'n'){
                            sscanf(vert.c_str(),"vn %f %f %f", &a, &b, &c );
                            normals.push_back(v3(a,b,c));
                    }
                }
                //Read files with no textures
                if(vert.front() == 'f'){
                    //counts the amount of slashes there are in a line for the face value
                    int counter = 0;
                    for(int i = 0; i < vert.size(); i++){
                        if(vert.at(i) == '/'){
                            counter++;
                        }
                    }
                    //if the size of the vector which holds the textures is empty, then enter if statement
                    if(textures.size()==0){
                        if (counter == 6){
                            //All a's will be vertexes and b's will be normals
                            sscanf(vert.c_str(),"f %f//%f %f//%f %f//%f", &a, &b, &a1, &b1, &a2, &b2 );
                            variants.push_back(var3(a-1,a1-1,a2-1,b-1,b1-1,b2-1));
                            //cout << "Vertexes: " << a  <<  "/" <<  a1 << "/" << a2 <<  "     |    Normals: " << variants[i].b << "/" << variants[i].b1 << "/" << variants[i].b2 << endl;
                            //i++;

                        }
                        else if (counter == 8){
                            //All a's will be vertexes and b's will be normals
                            sscanf(vert.c_str(),"f %f//%f %f//%f %f//%f %f//%f", &a, &b, &a1, &b1, &a2, &b2,&a3, &b3 );
                            variants2.push_back(var2(a-1,a1-1,a2-1,a3-1,b-1,b1-1,b2-1,b3-1));
                        }
                    }

                //Read rest of files with textures
                    else{
                    sscanf(vert.c_str(),"f %f/%f/%f %f/%f/%f %f/%f/%f", &a1, &b1, &c1, &a2, &b2, &c2, &a3, &b3, &c3 );
                    face.push_back(faces(a1-1,a2-1,a3-1,b1-1,b2-1,b3-1,c1-1,c2-1,c3-1));

                    }
                }
                //if there are no normals in the normal vector
        }
        //cout << "hello3" << endl;
        add_normals();

        }







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
	gluPerspective (50.0f,1,0.1f, 10000.0f);
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


void render() {

  glClearColor( 1, 0, 0, 1 );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}


//Draw function for my bunny
    void draw_bunny(){
    for(int i=0; i < face.size(); i++){

        if(!WireFrame)
            glBegin(GL_TRIANGLES);
        else
            glBegin(GL_LINE_LOOP);


                glNormal3f(normals[face[i].a3].a3, normals[face[i].a3].b3, normals[face[i].a3].c3);
                glTexCoord2f(textures[face[i].a2].a2, textures[face[i].a2].b2);
                glVertex3f(vertexs[face[i].a1].a3, vertexs[face[i].a1].b3, vertexs[face[i].a1].c3);

                glNormal3f(normals[face[i].b3].a3, normals[face[i].b3].b3, normals[face[i].b3].c3);
                glTexCoord2f(textures[face[i].b2].a2, textures[face[i].b2].b2);
                glVertex3f(vertexs[face[i].b1].a3, vertexs[face[i].b1].b3, vertexs[face[i].b1].c3);

                glNormal3f(normals[face[i].c3].a3, normals[face[i].c3].b3, normals[face[i].c3].c3);
                glTexCoord2f(textures[face[i].c2].a2, textures[face[i].c2].b2);
                glVertex3f(vertexs[face[i].c1].a3, vertexs[face[i].c1].b3, vertexs[face[i].c1].c3);
                glEnd();
                //cout << "hello4" << endl;
                //cout << vertexs[face[i].a1].a3 << endl;


            }

    }

int spin = 0;
static void display(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0, 5, 10.0-move_x, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);



    if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME

    //glDisable(GL_TEXTURE_2D);

     glPushMatrix ();
            TextureLoader("images/whitefur.png");
            glScalef(0.01,0.01,0.01);
            draw_bunny();
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

    }
}




void Specialkeys(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_UP:
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


//controls the lighting with the mouse
void mouse(int x,int y){
    //light source moved by mouse
    //type casting int x and y into float x and y
    GLfloat position[] = { (float) x ,0.0 , 3.5 ,1.0 - (float) y };
    glRotatef (spin, 0.0, 1.0, 0.0);
    glLightfv (GL_LIGHT0, GL_POSITION, position);


}


/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(800,600);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    //reads the bunny file
    readFile("objFiles/bunny-3.obj");


    glutCreateWindow("GLUT Shapes");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);
    glutIdleFunc(idle);
    //passively interacts mouse movement
    glutPassiveMotionFunc(mouse);
    glutMainLoop();
    return EXIT_SUCCESS;
}
