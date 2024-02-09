//Jonathan Wong
//108801712
//Project 5 - Model Texture Loader

//❖ Use the SkyBox you created and place the following inside

//❖ Read WaveFront models Ex: ‘Cube.obj’ file and store the following data in appropriate structures
//o Get vertices x,y,z (v lines)
//o Get texture cords either x,y (vt lines)
//o Get indices list that include vertex index, texture index, normal index, f lines
//o Apply Texture image to Models

//❖ Indices format is as follows
//o f → v index /vt index /vn index
//o if no vt f → v//vn
//o handle the error checking accordingly
//o Use ‘given framework for your implementation
//o Use the Texture image from here or similar

//❖ Draw the scene using the following format & follow the given key setup to navigate though the scene

//glBegin(GL_TRIANGLES);
//   glNormal3f(…..
//   glTexCoord2f(..
//   glVertex3f(……

//  glNormal3f(…..
//  glTexCoord2f(..
//  glVertex3f(……


//  glNormal3f(…..
//  glTexCoord2f(..
//  glVertex3f(……
//glEnd()

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



//Struct that holds the values of the vertex and normal
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



    //Function that takes the file and reads off the value from said file line by line.
    void readFile(string filename){
        //Holds the value of each line in the file
        //vector<string> value;
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
        }
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
    //Function to create my Sky Box.
    void skybox(){
    glPushMatrix();
    glDisable(GL_NORMALIZE);
    glDisable(GL_LIGHTING);

    glTranslatef(0,0,inout); //zoom in/out and determines the size of the Skybox

    glScalef(100,100,100);

    //top wall
    TextureLoader("images/Moon.JPG");
    glBegin(GL_QUADS);  //Begins the creation of an object from vertices
    glTexCoord2f(0.0, 1.0); //Maps out points to draw the side
    glVertex3f(1.0, 1.0, -1.0); //Helps draws the sides by specifying the vertex
    glTexCoord2f(0.0, 0.0);
    glVertex3f(1.0, 1.0, 1.0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(-1.0, 1.0, 1.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(-1.0, 1.0, -1.0);
    glEnd();    //Ends glBegin()

    //bottom wall
    TextureLoader("images/grass.png");
    glBegin(GL_QUADS);  //Begins the creation of an object from vertices
    glTexCoord2f(1.0, 1.0); //Maps out points to draw the side
    glVertex3f(-1.0, -1.0, 1.0);//Helps draws the sides by specifying the vertex
    glTexCoord2f(0.0, 1.0);
    glVertex3f(1.0, -1.0, 1.0);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(1.0, -1.0, -1.0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(-1.0, -1.0, -1.0);
    glEnd();    //Ends glBegin()

    //back wall
    TextureLoader("images/stonewall.png");
    glBegin(GL_QUADS);  //Begins the creation of an object from vertices
    glTexCoord2f(1.0, 0.0); //Maps out points to draw the side
    glVertex3f(-1.0, 1.0, -1.0);//Helps draws the sides by specifying the vertex
    glTexCoord2f(1.0, 1.0);
    glVertex3f(-1.0, -1.0, -1.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(1.0, -1.0, -1.0);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(1.0, 1.0, -1.0);
    glEnd();    //Ends glBegin()

    //Draw the back of the skybox

    glBegin(GL_QUADS);  //Begins the creation of an object from vertices
    glTexCoord2f(0.0, 0.0); //Maps out points to draw the side
    glVertex3f(-1.0, 1.0, 1.0);//Helps draws the sides by specifying the vertex
    glTexCoord2f(1.0, 0.0);
    glVertex3f(1.0, 1.0, 1.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(1.0, -1.0, 1.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-1.0, -1.0, 1.0);
    glEnd();    //Ends glBegin()

    //right wall
    glBegin(GL_QUADS);  //Begins the creation of an object from vertices
    glTexCoord2f(1.0, 0.0); //Maps out points to draw the side
    glVertex3f(1.0, 1.0, -1.0);//Helps draws the sides by specifying the vertex
    glTexCoord2f(1.0, 1.0);
    glVertex3f(1.0, -1.0, -1.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(1.0, -1.0, 1.0);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(1.0, 1.0, 1.0);
    glEnd();    //Ends glBegin()

    //left wall

    glBegin(GL_QUADS);  //Begins the creation of an object from vertices
    glTexCoord2f(0.0, 0.0); //Maps out points to draw the side
    glVertex3f(-1.0, 1.0, -1.0);//Helps draws the sides by specifying the vertex
    glTexCoord2f(1.0, 0.0);
    glVertex3f(-1.0, 1.0, 1.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(-1.0, -1.0, 1.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-1.0, -1.0, -1.0);
    glEnd();    //Ends glBegin()
    glPopMatrix();
    }


//function to draw models
    //draws elephant model
    void draw_ele(){
        //filename1 = "objFiles/ele.obj";
        readFile("objFiles/ele.obj");
    for(int i=0; i < variants.size(); i++){

        if(!WireFrame)
            glBegin(GL_TRIANGLES);
        else
            glBegin(GL_LINE_LOOP);

            //glTranslate3f(movex,movey,movez);
            //To draw objects with no textures

                glNormal3f(normals[variants[i].b].a3, normals[variants[i].b].b3, normals[variants[i].b].c3);
                glVertex3f(vertexs[variants[i].a].a3, vertexs[variants[i].a].b3, vertexs[variants[i].a].c3);

                glNormal3f(normals[variants[i].b1].a3, normals[variants[i].b1].b3, normals[variants[i].b1].c3);
                glVertex3f(vertexs[variants[i].a1].a3, vertexs[variants[i].a1].b3, vertexs[variants[i].a1].c3);

                glNormal3f(normals[variants[i].b2].a3, normals[variants[i].b2].b3, normals[variants[i].b2].c3);
                glVertex3f(vertexs[variants[i].a2].a3, vertexs[variants[i].a2].b3, vertexs[variants[i].a2].c3);
                glEnd();

            }


    for(int i=0; i < face.size(); i++){
        if(!WireFrame)
            glBegin(GL_TRIANGLES);
        else
            glBegin(GL_LINE_LOOP);
            if(textures.size() == 0){
                //glTranslate3f(movex,movey,movez);
                //To draw objects with no textures
                glNormal3f(normals[variants[i].b].a3, normals[variants[i].b].a3, normals[variants[i].b].a3);
                glVertex3f(vertexs[variants[i].a].a3, vertexs[variants[i].a].a3, vertexs[variants[i].a].a3);

                glNormal3f(normals[variants[i].b1].b3, normals[variants[i].b1].b3, normals[variants[i].b1].b3);
                glVertex3f(vertexs[variants[i].a1].b3, vertexs[variants[i].a1].b3, vertexs[variants[i].a1].b3);

                glNormal3f(normals[variants[i].b2].c3, normals[variants[i].b2].c3, normals[variants[i].b2].c3);
                glVertex3f(vertexs[variants[i].a2].c3, vertexs[variants[i].a2].c3, vertexs[variants[i].a2].c3);
                glEnd();
            }
            else{
            //glTranslate3f(movex,movey,movez);
            //To draw the rest of the objects with textures
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
            }
    }

    }
    //draws ateneum model
    void draw_ate(){
    //filename1 = "objFiles/ateneam-2.obj";
    readFile("objFiles/ateneam-2.obj");
    for(int i=0; i < variants.size(); i++){

        if(!WireFrame)
            glBegin(GL_TRIANGLES);
        else
            glBegin(GL_LINE_LOOP);
          //glTranslate3f(movex,movey,movez);
            //To draw the rest of the objects with textures
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

            }


    for(int i=0; i < face.size(); i++){
        if(!WireFrame)
            glBegin(GL_TRIANGLES);
        else
            glBegin(GL_LINE_LOOP);
            if(textures.size() == 0){
                //glTranslate3f(movex,movey,movez);
                //To draw objects with no textures
                glNormal3f(normals[variants[i].b].a3, normals[variants[i].b].a3, normals[variants[i].b].a3);
                glVertex3f(vertexs[variants[i].a].a3, vertexs[variants[i].a].a3, vertexs[variants[i].a].a3);

                glNormal3f(normals[variants[i].b1].b3, normals[variants[i].b1].b3, normals[variants[i].b1].b3);
                glVertex3f(vertexs[variants[i].a1].b3, vertexs[variants[i].a1].b3, vertexs[variants[i].a1].b3);

                glNormal3f(normals[variants[i].b2].c3, normals[variants[i].b2].c3, normals[variants[i].b2].c3);
                glVertex3f(vertexs[variants[i].a2].c3, vertexs[variants[i].a2].c3, vertexs[variants[i].a2].c3);
                glEnd();
            }
            else{
            //glTranslate3f(movex,movey,movez);
            //To draw the rest of the objects with textures
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
            }
    }

    }
    //draws cube model
    void draw_cube(){
    //filename1 = "objFiles/cube.obj";
    readFile("objFiles/cube.obj");
    for(int i=0; i < variants.size(); i++){

        if(!WireFrame)
            glBegin(GL_TRIANGLES);
        else
            glBegin(GL_LINE_LOOP);
            //glTranslate3f(movex,movey,movez);
            //To draw the rest of the objects with textures
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
            }


    for(int i=0; i < face.size(); i++){
        if(!WireFrame)
            glBegin(GL_TRIANGLES);
        else
            glBegin(GL_LINE_LOOP);
            if(textures.size() == 0){
                //glTranslate3f(movex,movey,movez);
                //To draw objects with no textures
                glNormal3f(normals[variants[i].b].a3, normals[variants[i].b].a3, normals[variants[i].b].a3);
                glVertex3f(vertexs[variants[i].a].a3, vertexs[variants[i].a].a3, vertexs[variants[i].a].a3);

                glNormal3f(normals[variants[i].b1].b3, normals[variants[i].b1].b3, normals[variants[i].b1].b3);
                glVertex3f(vertexs[variants[i].a1].b3, vertexs[variants[i].a1].b3, vertexs[variants[i].a1].b3);

                glNormal3f(normals[variants[i].b2].c3, normals[variants[i].b2].c3, normals[variants[i].b2].c3);
                glVertex3f(vertexs[variants[i].a2].c3, vertexs[variants[i].a2].c3, vertexs[variants[i].a2].c3);
                glEnd();
            }
            else{
            //glTranslate3f(movex,movey,movez);
            //To draw the rest of the objects with textures
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
            }
    }

    }
    //draws trunk model
    void draw_trunk(){
//    filename1 = "objFiles/trunk.obj";
    readFile("objFiles/trunk.obj");

    for(int i=0; i < variants.size(); i++){

        if(!WireFrame)
            glBegin(GL_TRIANGLES);
        else
            glBegin(GL_LINE_LOOP);

            //glTranslate3f(movex,movey,movez);
            //To draw objects with no textures

                glNormal3f(normals[variants[i].b].a3, normals[variants[i].b].b3, normals[variants[i].b].c3);
                glVertex3f(vertexs[variants[i].a].a3, vertexs[variants[i].a].b3, vertexs[variants[i].a].c3);

                glNormal3f(normals[variants[i].b1].a3, normals[variants[i].b1].b3, normals[variants[i].b1].c3);
                glVertex3f(vertexs[variants[i].a1].a3, vertexs[variants[i].a1].b3, vertexs[variants[i].a1].c3);

                glNormal3f(normals[variants[i].b2].a3, normals[variants[i].b2].b3, normals[variants[i].b2].c3);
                glVertex3f(vertexs[variants[i].a2].a3, vertexs[variants[i].a2].b3, vertexs[variants[i].a2].c3);
                glEnd();

            }


    for(int i=0; i < face.size(); i++){
        if(!WireFrame)
            glBegin(GL_TRIANGLES);
        else
            glBegin(GL_LINE_LOOP);
            if(textures.size() == 0){
                //glTranslate3f(movex,movey,movez);
                //To draw objects with no textures
                glNormal3f(normals[variants[i].b].a3, normals[variants[i].b].a3, normals[variants[i].b].a3);
                glVertex3f(vertexs[variants[i].a].a3, vertexs[variants[i].a].a3, vertexs[variants[i].a].a3);

                glNormal3f(normals[variants[i].b1].b3, normals[variants[i].b1].b3, normals[variants[i].b1].b3);
                glVertex3f(vertexs[variants[i].a1].b3, vertexs[variants[i].a1].b3, vertexs[variants[i].a1].b3);

                glNormal3f(normals[variants[i].b2].c3, normals[variants[i].b2].c3, normals[variants[i].b2].c3);
                glVertex3f(vertexs[variants[i].a2].c3, vertexs[variants[i].a2].c3, vertexs[variants[i].a2].c3);
                glEnd();
            }
            else{
            //glTranslate3f(movex,movey,movez);
            //To draw the rest of the objects with textures
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
            }
    }

    }
    //draws ven2 model
    void draw_ven2(){
//    filename1 = "objFiles/ven-2.obj";
    readFile("objFiles/ven-2.obj");

    for(int i=0; i < variants.size(); i++){

        if(!WireFrame)
            glBegin(GL_TRIANGLES);
        else
            glBegin(GL_LINE_LOOP);

            //glTranslate3f(movex,movey,movez);
            //To draw objects with no textures

                glNormal3f(normals[variants[i].b].a3, normals[variants[i].b].b3, normals[variants[i].b].c3);
                glVertex3f(vertexs[variants[i].a].a3, vertexs[variants[i].a].b3, vertexs[variants[i].a].c3);

                glNormal3f(normals[variants[i].b1].a3, normals[variants[i].b1].b3, normals[variants[i].b1].c3);
                glVertex3f(vertexs[variants[i].a1].a3, vertexs[variants[i].a1].b3, vertexs[variants[i].a1].c3);

                glNormal3f(normals[variants[i].b2].a3, normals[variants[i].b2].b3, normals[variants[i].b2].c3);
                glVertex3f(vertexs[variants[i].a2].a3, vertexs[variants[i].a2].b3, vertexs[variants[i].a2].c3);
                glEnd();

            }


    for(int i=0; i < face.size(); i++){
        if(!WireFrame)
            glBegin(GL_TRIANGLES);
        else
            glBegin(GL_LINE_LOOP);
            if(textures.size() == 0){
                //glTranslate3f(movex,movey,movez);
                //To draw objects with no textures
                glNormal3f(normals[variants[i].b].a3, normals[variants[i].b].a3, normals[variants[i].b].a3);
                glVertex3f(vertexs[variants[i].a].a3, vertexs[variants[i].a].a3, vertexs[variants[i].a].a3);

                glNormal3f(normals[variants[i].b1].b3, normals[variants[i].b1].b3, normals[variants[i].b1].b3);
                glVertex3f(vertexs[variants[i].a1].b3, vertexs[variants[i].a1].b3, vertexs[variants[i].a1].b3);

                glNormal3f(normals[variants[i].b2].c3, normals[variants[i].b2].c3, normals[variants[i].b2].c3);
                glVertex3f(vertexs[variants[i].a2].c3, vertexs[variants[i].a2].c3, vertexs[variants[i].a2].c3);
                glEnd();
            }
            else{
            //glTranslate3f(movex,movey,movez);
            //To draw the rest of the objects with textures
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
            }
    }

    }

void render() {

  glClearColor( 1, 0, 0, 1 );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}


static void display(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0, 5, 10.0-move_x, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glRotatef(rot_x, 1, 0, 0);      //rotates skybox around X axis
    glRotatef(rot_y, 0, 1, 0);      //rotates skybox around Y axis


    if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME

    glRotatef(rotBox_x, 1, 0, 0);      //rotates skybox around X axis
    glRotatef(rotBox_y, 0, 1, 0);      //rotates skybox around Y axis

    glDisable(GL_TEXTURE_2D);
    //Draws objects
    //Checks if the object is ateneam, then scales it down so it can be viewed in the skybox
    /*if(filename1 == "objFiles/ateneam-2.obj"){
        glScalef(inoutObj + 0.0005,inoutObj + 0.0005,inoutObj + 0.0005);
        glTranslatef(1000.0,0.0,0.0);
    }*/
/*
    if(filename1 == "objFiles/cow.obj"){
        glScalef(inoutObj + 0.5,inoutObj + 0.5,inoutObj + 0.5);
    }

    if(filename1 == "objFiles/ele.obj"){
        glScalef(inoutObj + 0.005,inoutObj + 0.005,inoutObj + 0.005);
        glTranslatef(500.0,0.0,0.0);
    }

    if(filename1 == "objFiles/cube.obj"){
        glScalef(inoutObj + 10.0,inoutObj + 10.0,inoutObj + 10.0);
        glTranslatef(-200.0,0.0,0.0);
    }
*/
    //Draws the skybox
    skybox();

    //draws elephant
    glPushMatrix();
    TextureLoader("images/fire.png");
    glScalef(inoutObj + 0.005,inoutObj + 0.005,inoutObj + 0.005);
    glTranslatef(500.0,0.0,0.0);
    draw_ele();
    glPopMatrix();


    //draws Ateneum
    glPushMatrix();
    TextureLoader("images/Statue.png");
    glScalef(inoutObj + 0.0005,inoutObj + 0.0005,inoutObj + 0.0005);
    glTranslatef(1000.0,0.0,0.0);
    draw_ate();
    glPopMatrix();

    //draws trunk
    glPushMatrix();
    TextureLoader("images/b_tile.png");
    glScalef(inoutObj +0.5, inoutObj +0.5, inoutObj +0.5);
    glTranslatef(-3.0,0.0,0.0);
    draw_trunk();
    glPopMatrix();

    //draws ven2
    glPushMatrix();
    TextureLoader("images/g_tile.png");
    glScalef(inoutObj +0.001, inoutObj +0.001, inoutObj +0.001);
    glTranslatef(-4000.0,0.0,0.0);
    draw_ven2();
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
    case 'a':   // moves left
        move_y -= 0.01;
        break;

    case 'd':   //moves right
        move_y += 0.01;
        break;

    case 'w':   //move forward
        move_x += 0.5;
        break;
    case 's':   //moves backwards
        move_x -= 0.5;
        break;

        //rotate camera left around the Y axis
    case 'j':
        rotBox_y -= 5.0;
        break;

        //rotate camera right around the Y axis
    case 'l':
        rotBox_y += 5.0;
        break;

        //rotate camera up around the X axis
    case 'i':
        rotBox_x -= 5.0;
        break;

        //rotate camera down around the X axis
    case 'k':
        rotBox_x += 5.0;
        break;

        //Shows Wireframe of object
    case 'o':
        WireFrame = !WireFrame;
        break;
        //resets whole entire scene when its rotated
    case ' ':

            rotBox_x = 0; //resets Skybox's X axis
            rotBox_y = 0; //resets Skybox's Y axis
            rotBox_z = 0; //resets Zoom
            rotMod_x = 0; //resets Model's X axis
            rotMod_y = 0; //resets Model's Y axis

        break;
    }
}

int old_y;
int old_x;


//Mouse Movement
void mouseMovement(int x,int y)
{
    //compares the old y with a new y, rotates X if condition is met
     if(old_y < y){
        if(move_x <= 15){
            move_x += 2.0;
        }
     }
     //compares the old y with a new y, rotates X if condition is met
      if(old_y > y){
        if(move_x >= -80){
            move_x -= 2.0;
        }
     }
     //compares the old x with a new x, rotates Y if condition is met
     if(old_x < x){
        move_y += 3.0;
     }
     //compares the old x with a new x, rotates Y if condition is met
      if(old_x > x){
        move_y -= 3.0;
     }
    //Update the mouse positions
    old_x = x;
    old_y = y;
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


/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(800,600);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
//    filename1 = "objFiles/cube.obj";
//    readFile("objFiles/cube.obj");
//    filename1 = "objFiles/ateneam-2.obj";
//    readFile("objFiles/ateneam-2.obj");
//    filename1 = "objFiles/cow.obj";
//    readFile("objFiles/cow.obj");
//    filename1 = "objFiles/ele.obj";
//    readFile("objFiles/ele.obj");
//    filename1 = "objFiles/trunk.obj";
//    readFile("objFiles/trunk.obj");
//    filename1 = "objFiles/ven-2.obj";
//    readFile("objFiles/ven-2.obj");

    glutCreateWindow("GLUT Shapes");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);
    glutMotionFunc(mouseMovement);
    glutIdleFunc(idle);
    glutMainLoop();

    return EXIT_SUCCESS;
}
