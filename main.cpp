//#include <stdlib.h>
//#include <fstream>
#include <iostream>
#include <Windows.h>
#include <GL/glut.h>
#include <vector>
#include <string>
#include <math.h>
#include <time.h>
using namespace std;

vector<string> *trees = new vector<string>();
float  ANGLE = 20, length = 0.001, incr = 0.1;
int movementX = 350, movementY = 100, movementZ = 200, centerX = 0, centerY = 150, fieldOfView = 75;
float lastTime = 0, elapsedTime = 0, lastElapsedTime = 0;
const float PI = 3.14159265358979323846  , NoOfIterations = 5;
int delay = 0,minDepth = 0, lineWidth = 6;
string str = "B";       //Default String

void push(){
	glPushMatrix();
	if (lineWidth > 0)
		lineWidth -= 1;
}
void pop(){
	glPopMatrix();
	lineWidth += 1;
}
void rotateL(){
            glRotatef(ANGLE, 1, 0, 0);
			glRotatef(ANGLE*4, 0, 1, 0);
			glRotatef(ANGLE, 0, 0, 1);
}
void rotateR(){
			glRotatef(-ANGLE, 1, 0, 0);
			glRotatef(ANGLE*4, 0, 1, 0);
			glRotatef(-ANGLE, 0, 0, 1);
}
void leaf(){
            glColor3f(0, 0.4, 0);      //Dark Green
			glutSolidCube(minDepth+1);     //Leaf
}
void drawLine(){

            glColor3f(0.545, 0.271, 0.075);     //Brown
			glLineWidth(lineWidth);
			glBegin(GL_LINES);
				glVertex3f(0, 0, 0);
				glVertex3f(0, length, 0);
			glEnd();
			glTranslatef(0, length, 0);
}

void wind(){
float numR = (float) rand()/RAND_MAX;

	delay+=1;
if (delay%4 == 0)
{
	if (ANGLE > 21.5){
		if (numR < 0.5){
			incr = -0.15;
		} else {
			incr = -0.1;
		}
	} else if (ANGLE < 18.5){
		if (numR > 0.5){
			incr = 0.15;
		} else {
			incr = 0.1;
		}
	}
	ANGLE += incr;
}
}

void ground(){
    glColor3f(0.42, 0.557, 0.137);  //Olive Colour
    glBegin(GL_TRIANGLES);        //Ground
		glVertex3f(-3000,0,-3000);
		glVertex3f(3000,0,-3000);
		glVertex3f(3000,0,3000);
		glVertex3f(-3000,0,3000);
		glVertex3f(-3000,0,-3000);
		glVertex3f(3000,0,3000);
	glEnd();

}


void draw(){
	string ch = "";
	string LSystem = trees->at(minDepth);
	for (int i = 0; i < LSystem.length(); i++){
		ch = LSystem.at(i);

		if (ch.compare("B") == 0 || ch.compare("A") == 0){
			drawLine();
		} else if (ch.compare("[") == 0){
			push();
		} else if (ch.compare("]") == 0){
			pop();
		} else if (ch.compare("L") == 0){
			leaf();
		} else if (ch.compare("-") == 0){
			rotateR();
		} else if (ch.compare("+") == 0){
			rotateL();
		}
	}
}

void expand(float num){
	string ch = "";

	for (int i = 0; i < str.length(); i++){
		ch = str.at(i);

		if (ch.compare("A") == 0){
			str.replace(i, 1, "AA");
			i = i + 1;
		} else if (ch.compare("B") == 0){
			str.replace(i, 1, "A[+BL]A[-BL]+B");        //A[+BL]A[-BL]+B
//			str.replace(i, 1, "A-[[B]+B]+B[+BA]-B");
			i = i + 13;
		}
	}
	trees->push_back(str);
}

void init(float r, float g, float b)
{
    glClearColor(r,g,b,0);      //glClearColor(0.53, 0.8, 0.9, 1.0);      //Sky Colour
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}

void display(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gluPerspective (fieldOfView, 1.0, 1, 2000);
	gluLookAt(movementX,movementY,movementZ,centerX,centerY,0,0,1,0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


    wind();
    ground();
	draw();

	glutSwapBuffers();
}





void animate(){
	if (minDepth < NoOfIterations)
		length += 0.0004;

	if (lastTime == 0)
		lastTime = timeGetTime();
	elapsedTime = timeGetTime()-lastTime;

	if(elapsedTime-lastElapsedTime > 2000 && minDepth < NoOfIterations){
		minDepth++;
		lastElapsedTime = elapsedTime;
	}
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y){
   switch (key) {
          case 'q':			// q - Exit the program
		  exit(1);
		  break;
		  case 'r':			// r - Reset the camera
		  fieldOfView = 75;
		  movementX = 350;
		  movementY = 100;
		  movementZ = 200;
		  centerX = 0;
		  centerY = 150;
		  break;
		  case 'w':			// w - Move Forward
		  movementX-=10;
		  glutPostRedisplay();
		  break;
		  case 's':			// s - Move Backwards
		  movementX+=10;
		  glutPostRedisplay();
		  break;
		  case 'd':			// d - Move to the right
		  movementZ+=10;
		  glutPostRedisplay();
		  break;
		  case 'a':			// a - Move to the left
		  movementZ-=10;
		  glutPostRedisplay();
		  break;
   }
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE);
	glutInitWindowSize (800, 640);
//	glutInitWindowPosition (100, 100);
	glutCreateWindow ("Recursive Tree");
//	init(1, 1, 1);
    init(0.53, 0.8, 0.9);                 // init(0.53, 0.8, 0.9); Blue Background
    glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(animate);
	trees = new vector<string>();
	for(int i = 0; i <= NoOfIterations; i++)
        {expand(0);}
	glutMainLoop();
	return 0;
}
