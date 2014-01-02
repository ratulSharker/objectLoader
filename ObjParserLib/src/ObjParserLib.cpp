//============================================================================
// Name        : ObjParserLib.cpp
// Author      : ratul
// Version     : 1.0
// Copyright   : It is under Apache Licence agreement
// Description : ObjectParser in C++, Ansi-style
//============================================================================


#include <iostream>
using namespace std;


#include <cmath>
#include <gl/freeglut.h>
#include "../openGL/myGrid.h"
#include "../libs/ObjectParser.h"
#include <vector>

#define PI 3.14159265
#define LOOKING_DISTANCE 30
#define LOOKING_HEIGHT 20
#define LOOKING_HEIGHT_MAX 60
#define LOOKING_HEIGHT_MIN -10
#define CAMERA_HEIGHT 30
#define HIGHEST_FAN_ROTATION_SPEED 20


ObjectParser heli("asset/heli.obj");


//fan constant
#define FAN_ACCERALATING 1
#define FAN_DECCERALATING 2
#define FAN_STOP 3

int fan_state = FAN_STOP;
double fan_angle = 0, fan_speed = 1, fan_accelaration = 0.05;

double door_angle = 0;

//camera management
double currentCameraX = 0, currentCameraY = 0, currentCameraZ = 20;
double currentLookingAtX = 10, currentLookingAtY = 10, currentLookingAtZ = 0;

double CameraMovingAngle = 0;
double movingSpeed = 2, LookAngleDeviation = 2;

//multiple key support -- for special keys
#define LEFT_ARROW 0
#define RIGHT_ARROW 1
#define UP_ARROW 2
#define DOWN_ARROW 3
bool fourArrow[4] = { false, false, false, false };
unsigned int currently_special_pressed = 0;

//multiple key support -- for normal keys
#define A_BUTTON 0
#define S_BUTTON 1
#define D_BUTTON 2
#define W_BUTTON 3
#define Q_BUTTON 4
#define E_BUTTON 5
#define Z_BUTTON 6
#define X_BUTTON 7
#define C_BUTTON 8
#define V_BUTTON 9
bool normalKeys[10] = { false, false, false, false, false, false, false, false,
		false, false };

unsigned int currently_normal_pressed = 0;

//forward declaration
void specialKeyListener(int key, int x, int y);
void keyBoardListener(unsigned char ch, int x, int y);

void init() {

	glClearColor(0, 0, 0, 0);

	//load the projection matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//enabling the depth test
	glEnable(GL_DEPTH_TEST);


	//enable lighting things
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	float color[] = {1.0,1.0,1.0,1.0};
	glLightfv(GL_LIGHT0,GL_DIFFUSE,color);


	//calculate the initial looking + camera looking at the first quadtrant
	currentCameraX = -200 * 1.3 * 0.75; //width of the room
	currentCameraY = 150 * 1.3 / 2;
	currentCameraZ = CAMERA_HEIGHT;

	currentLookingAtX = currentCameraX
			+ LOOKING_DISTANCE * cos(CameraMovingAngle * PI / 180);
	currentLookingAtY = currentCameraY
			+ LOOKING_DISTANCE * sin(CameraMovingAngle * PI / 180);
	currentLookingAtZ = LOOKING_HEIGHT;

}

/*
void drawHeli(){
	float displaceX = 0,displaceY = 0,displaceZ = 0;
	for(std::map<std::string,std::vector<Face> >::iterator it = heli.faces.begin(); it != heli.faces.end() ; it++){

		//std::cout<<it->first<<" "<<it->second.size()<<endl;
		if(it->first.compare("rear_rotor")==0){
			displaceX = 6.3022; //main_rotor transformation values
			displaceY = -1.04241;
			displaceZ = 4.75804;

			//double ThetaX = 20.0,ThetaY = 25.0, ThetaZ = -30.0;

			glPushMatrix();
			//glTranslatef(displaceX,displaceY,displaceZ);


			//XZ axis angle can be provided here

			glRotatef(fan_angle,cos((ThetaX) * 3.1416/180.0) * sin((-ThetaZ) * 3.1416 / 180.0),
								cos((ThetaX) * 3.1416/180.0) * cos((ThetaZ) * 3.1416/180.0),
								sin((ThetaX) * 3.1416/180.0));

			glRotatef(10,1,0,0);
			glRotatef(20,0,1,0);
			glRotatef(30,0,0,1);

			glRotatef(fan_angle,0,1,0);

			glRotatef(-10,1,0,0);
			glRotatef(-20,0,1,0);
			glRotatef(-30,0,0,1);

		}
		else{
			displaceX = 0; //main_rotor transformation values
			displaceY = 0;
			displaceZ = 0;
		}
		//here we get a object
		for(unsigned int i=0;i<it->second.size();i++){
			//here we get a single face
				if(it->second[i].isFour){
					glBegin(GL_QUADS);
				}
				else{
					glBegin(GL_TRIANGLES);
				}


				Vertice *v1 = &heli.vertices[it->second[i].A - 1];
				Vertice *v2 = &heli.vertices[it->second[i].B - 1];
				Vertice *v3 = &heli.vertices[it->second[i].C - 1];


				glVertex3f(v1->X-displaceX,v1->Y-displaceY,v1->Z-displaceZ);
				glVertex3f(v2->X-displaceX,v2->Y-displaceY,v2->Z-displaceZ);
				glVertex3f(v3->X-displaceX,v3->Y-displaceY,v3->Z-displaceZ);

				if(it->second[i].isFour){
					Vertice *v4 = &heli.vertices[it->second[i].D - 1];
					glVertex3f(v4->X-displaceX,v4->Y-displaceY,v4->Z-displaceZ);
				}

				glEnd();

			}

		if(it->first.compare("rear_rotor")==0){
			glPopMatrix();
		}

		}
}
*/

void drawing() {

	//clearing :D
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	//matrix operations
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//lighting codes
	float position[] = {0,0,40,1};
	glLightfv(GL_LIGHT0,GL_POSITION,position);

	//camera settings
	gluLookAt(currentCameraX, currentCameraY, currentCameraZ, currentLookingAtX,currentLookingAtY, currentLookingAtZ, 0, 0, 1);

	//drawing code should be here
	drawGrid(10,200,5);

	//heli.DrawGivenSubobjectWithAngle("rear_rotor",6.3022,-1.04241,4.75804,10,20,30,fan_angle,ObjectParser::Y_AXIS,true);
	//heli.DrawGivenSubobject("rear_rotor",0,0,0);
	//heli.DrawWholeObjectWithNoTransformation();
	glColor3f(1,1,1);
	heli.DrawGivenSubobject("fuselage",0,0,0);
	//heli.DrawGivenSubobject("cabinDoor13",0,0,0);
	//glColor3f(0.9,0.9,0.9);
	heli.DrawGivenSubobject("cabinDoor_",0,0,0);
	//glColor3f(1,1,1);
	heli.DrawGivenSubobject("Door_R",0,0,0);
	//glColor3f(0.9,0.9,0.9);
	heli.DrawGivenSubobject("Door_L",0,0,0);
	//glColor3f(1,1,1);
	heli.DrawGivenSubobject("elevator",0,0,0);
	//glColor3f(0.9,0.9,0.9);
	heli.DrawGivenSubobject("tyre_R",0,0,0);
	//glColor3f(1,1,1);
	heli.DrawGivenSubobject("rear_Tyre",0,0,0);
	//glColor3f(0.9,0.9,0.9);
	heli.DrawGivenSubobject("Tyre_L",0,0,0);
	//glColor3f(1,1,1);
	heli.DrawGivenSubobject("Window_L2",0,0,0);
	//glColor3f(0.9,0.9,0.9);
	heli.DrawGivenSubobject("Window_L1",0,0,0);
	//glColor3f(1,1,1);
	heli.DrawGivenSubobject("Window_R2",0,0,0);
	//glColor3f(0.9,0.9,0.9);
	heli.DrawGivenSubobject("window_R1",0,0,0);

	//drawing main rotor
	heli.DrawGivenSubobjectWithAngle("main_rotor",6.32871,10.69359,4.16496,-90,0,0,fan_angle,ObjectParser::Y_AXIS,false);
	heli.DrawGivenSubobjectWithAngle("rear_rotor",6.41021,-1.04241,4.75804,7.004,0,-90,fan_angle,ObjectParser::Y_AXIS,false);


	//necessary to ensure that it draws
	glutSwapBuffers();

	if (currently_special_pressed != 0) {
		specialKeyListener(-1, -1, -1);
	}
	if (currently_normal_pressed != 0) {
		keyBoardListener(0, -1, -1);
	}
}

void anim() {

	//animation code for the fan
	if (fan_state == FAN_ACCERALATING) {
		//time to accelerate
		if (fan_speed < HIGHEST_FAN_ROTATION_SPEED) {
			fan_speed += fan_accelaration;
		}
		fan_angle += fan_speed; //giving it the speed
	} else if (fan_state == FAN_DECCERALATING) {
		//time to de-cellarate
		fan_speed -= fan_accelaration;

		if (fan_speed <= 0) {
			fan_state = FAN_STOP;
		}
		fan_angle += fan_speed; //giving it the speed
	}

	glutPostRedisplay();
}

void keyBoardListener(unsigned char ch, int x, int y) {

	if (ch == 0 && x == -1 && y == -1 && currently_normal_pressed > 0) { //if any key is currenlty pressed down

		for (int i = 0; i < 10; i++) {
			//check each key is pressed or not :)
			if (normalKeys[i] == false) {
				continue;
			}

			//called from display function
			switch (i) {
			case W_BUTTON: //'w': //for small w
				//case 'W': //for capital W
				//looking upward
				if (currentLookingAtZ < LOOKING_HEIGHT_MAX)
					currentLookingAtZ += 1;

				break;

			case A_BUTTON:				//'a':
				//case 'A':
				//look leftward -- update angle + currently looking values
				CameraMovingAngle += LookAngleDeviation;
				currentLookingAtX = currentCameraX
						+ LOOKING_DISTANCE * cos(CameraMovingAngle * PI / 180);
				currentLookingAtY = currentCameraY
						+ LOOKING_DISTANCE * sin(CameraMovingAngle * PI / 180);

				break;
			case D_BUTTON:				//'d':
				//case 'D':
				//look leftward -- update angle + currently looking values
				CameraMovingAngle -= LookAngleDeviation;
				currentLookingAtX = currentCameraX
						+ LOOKING_DISTANCE * cos(CameraMovingAngle * PI / 180);
				currentLookingAtY = currentCameraY
						+ LOOKING_DISTANCE * sin(CameraMovingAngle * PI / 180);

				break;

			case S_BUTTON:				//'s':
				//case 'S':
				//loking downward
				if (currentLookingAtZ > LOOKING_HEIGHT_MIN) {
					currentLookingAtZ -= 1;
				}

				break;
			case Z_BUTTON:			//'z':
				//case 'Z':
				door_angle++;
				break;

			case X_BUTTON:			//'x':
				//case 'X':
				door_angle--;
				break;

			case Q_BUTTON:			//'q':
				//case 'Q': //go up

				currentCameraZ++;
				currentLookingAtZ++;
				break;

			case E_BUTTON:			//'e':
				//case 'E':
				currentCameraZ--;
				currentLookingAtZ--;
				break;
			}
		}

	} else {			//otherwise called from normal call back function
		switch (ch) {
		case 'w': //for small w
		case 'W': //for capital W
			//looking upward
			if (normalKeys[W_BUTTON] == false) {
				normalKeys[W_BUTTON] = true;
				currently_normal_pressed++;
			} else {
				//repress the same key :D -- nothing to handle
			}

			break;

		case 'a':
		case 'A':
			//look leftward -- update angle + currently looking values
			if (normalKeys[A_BUTTON] == false) {
				normalKeys[A_BUTTON] = true;
				currently_normal_pressed++;
			} else {
				//repress the same key :D -- nothing to handle
			}

			break;
		case 'd':
		case 'D':
			//look leftward -- update angle + currently looking values
			if (normalKeys[D_BUTTON] == false) {
				normalKeys[D_BUTTON] = true;
				currently_normal_pressed++;
			} else {
				//repress the same key :D -- nothing to handle
			}

			break;

		case 's':
		case 'S':
			//loking downward
			if (normalKeys[S_BUTTON] == false) {
				normalKeys[S_BUTTON] = true;
				currently_normal_pressed++;
			} else {
				//repress the same key :D -- nothing to handle
			}

			break;

		case 'f':
		case 'F':
			if (fan_state == FAN_STOP) {
				glutIdleFunc(anim);
				fan_state = FAN_ACCERALATING;
			} else {
				fan_state = FAN_DECCERALATING;
			}

			break;

		case 'z':
		case 'Z':
			if (normalKeys[Z_BUTTON] == false) {
				normalKeys[Z_BUTTON] = true;
				currently_normal_pressed++;
			} else {
				//repress the same key :D -- nothing to handle
			}
			break;

		case 'x':
		case 'X':
			if (normalKeys[X_BUTTON] == false) {
				normalKeys[X_BUTTON] = true;
				currently_normal_pressed++;
			} else {
				//repress the same key :D -- nothing to handle
			}
			break;

		case 'q':
		case 'Q': //go up

			if (normalKeys[Q_BUTTON] == false) {
				normalKeys[Q_BUTTON] = true;
				currently_normal_pressed++;
			} else {
				//repress the same key :D -- nothing to handle
			}
			break;

		case 'e':
		case 'E':
			if (normalKeys[E_BUTTON] == false) {
				normalKeys[E_BUTTON] = true;
				currently_normal_pressed++;
			} else {
				//repress the same key :D -- nothing to handle
			}
			break;
		case ' ':
				glutFullScreenToggle();
				break;
		}

	}

	glutPostRedisplay();
}

void specialKeyListener(int key, int x, int y) {

	if (key == -1 && x == -1 && y == -1 && currently_special_pressed > 0) { //if any key is currenlty pressed down

	//called from display function
		for (int i = 0; i < 4; i++) {
			//check each key is pressed or not :)
			if (fourArrow[i] == false) {
				continue;
			}
			//navigation keys -> ^ v <-
			switch (i) {
			case UP_ARROW: //GLUT_KEY_UP:
				//moving forward
				currentCameraX += movingSpeed
						* cos(CameraMovingAngle * PI / 180);
				currentLookingAtX += movingSpeed
						* cos(CameraMovingAngle * PI / 180);
				currentCameraY += movingSpeed
						* sin(CameraMovingAngle * PI / 180);
				currentLookingAtY += movingSpeed
						* sin(CameraMovingAngle * PI / 180);
				break;

			case DOWN_ARROW:					//GLUT_KEY_DOWN:
				//moving backward
				currentCameraX -= movingSpeed
						* cos(CameraMovingAngle * PI / 180);
				currentLookingAtX -= movingSpeed
						* cos(CameraMovingAngle * PI / 180);
				currentCameraY -= movingSpeed
						* sin(CameraMovingAngle * PI / 180);
				currentLookingAtY -= movingSpeed
						* sin(CameraMovingAngle * PI / 180);
				break;

			case LEFT_ARROW:					//GLUT_KEY_LEFT:
				//moving left
				currentCameraX += movingSpeed
						* cos((CameraMovingAngle + 90) * PI / 180);
				currentLookingAtX += movingSpeed
						* cos((CameraMovingAngle + 90) * PI / 180);
				currentCameraY += movingSpeed
						* sin((CameraMovingAngle + 90) * PI / 180);
				currentLookingAtY += movingSpeed
						* sin((CameraMovingAngle + 90) * PI / 180);

				break;

			case RIGHT_ARROW:					//GLUT_KEY_RIGHT:
				//moving right
				currentCameraX -= movingSpeed
						* cos((CameraMovingAngle + 90) * PI / 180);
				currentLookingAtX -= movingSpeed
						* cos((CameraMovingAngle + 90) * PI / 180);
				currentCameraY -= movingSpeed
						* sin((CameraMovingAngle + 90) * PI / 180);
				currentLookingAtY -= movingSpeed
						* sin((CameraMovingAngle + 90) * PI / 180);

			}

		}
	} else {				//otherwise called from normal call back function
		switch (key) {
		case GLUT_KEY_UP:
			if (fourArrow[UP_ARROW] == false) {
				fourArrow[UP_ARROW] = true;
				currently_special_pressed++;
			} else {
				//repress the same key :D -- nothing to handle
			}
			break;
		case GLUT_KEY_DOWN:
			if (fourArrow[DOWN_ARROW] == false) {
				fourArrow[DOWN_ARROW] = true;
				currently_special_pressed++;
			} else {
				//repress the same key :D -- nothing to handle
			}
			break;
		case GLUT_KEY_RIGHT:
			if (fourArrow[RIGHT_ARROW] == false) {
				fourArrow[RIGHT_ARROW] = true;
				currently_special_pressed++;
			} else {
				//repress the same key :D -- nothing to handle
			}
			break;
		case GLUT_KEY_LEFT:
			if (fourArrow[LEFT_ARROW] == false) {
				fourArrow[LEFT_ARROW] = true;
				currently_special_pressed++;
			} else {
				//repress the same key :D -- nothing to handle
			}
			break;
		case GLUT_KEY_END:
			glutExit();
			break;
		}

	}

	//refreshing the display
	glutPostRedisplay();
}

void reshapeWindow(int width, int height) {

	gluPerspective(70, ((float) width) / height, 0.1, 10000);
}

void specialkeyboardUpFunc(int ch, int x, int y) {
	switch (ch) {
	case GLUT_KEY_UP:
		if (fourArrow[UP_ARROW] == true) {
			fourArrow[UP_ARROW] = false;
			currently_special_pressed--;
		} else {
			//repress the same key :D -- nothing to handle
		}
		break;
	case GLUT_KEY_DOWN:
		if (fourArrow[DOWN_ARROW] == true) {
			fourArrow[DOWN_ARROW] = false;
			currently_special_pressed--;
		} else {
			//repress the same key :D -- nothing to handle
		}
		break;
	case GLUT_KEY_RIGHT:
		if (fourArrow[RIGHT_ARROW] == true) {
			fourArrow[RIGHT_ARROW] = false;
			currently_special_pressed--;
		} else {
			//repress the same key :D -- nothing to handle
		}
		break;
	case GLUT_KEY_LEFT:
		if (fourArrow[LEFT_ARROW] == true) {
			fourArrow[LEFT_ARROW] = false;
			currently_special_pressed--;
		} else {
			//repress the same key :D -- nothing to handle
		}
		break;
	}
}

void normalkeyboardUpFunc(unsigned char ch, int x, int y) {
	switch (ch) {
	case 'w': //for small w
	case 'W': //for capital W
		//looking upward
		if (normalKeys[W_BUTTON] == true) {
			normalKeys[W_BUTTON] = false;
			currently_normal_pressed--;
		} else {
			//repress the same key :D -- nothing to handle
		}

		break;

	case 'a':
	case 'A':
		//look leftward -- update angle + currently looking values
		if (normalKeys[A_BUTTON] == true) {
			normalKeys[A_BUTTON] = false;
			currently_normal_pressed--;
		} else {
			//repress the same key :D -- nothing to handle
		}

		break;
	case 'd':
	case 'D':
		//look leftward -- update angle + currently looking values
		if (normalKeys[D_BUTTON] == true) {
			normalKeys[D_BUTTON] = false;
			currently_normal_pressed--;
		} else {
			//repress the same key :D -- nothing to handle
		}

		break;

	case 's':
	case 'S':
		//loking downward
		if (normalKeys[S_BUTTON] == true) {
			normalKeys[S_BUTTON] = false;
			currently_normal_pressed--;
		} else {
			//repress the same key :D -- nothing to handle
		}

		break;

	case 'z':
	case 'Z':
		if (normalKeys[Z_BUTTON] == true) {
			normalKeys[Z_BUTTON] = false;
			currently_normal_pressed--;
		} else {
			//repress the same key :D -- nothing to handle
		}
		break;

	case 'x':
	case 'X':
		if (normalKeys[X_BUTTON] == true) {
			normalKeys[X_BUTTON] = false;
			currently_normal_pressed--;
		} else {
			//repress the same key :D -- nothing to handle
		}
		break;

	case 'q':
	case 'Q': //go up

		if (normalKeys[Q_BUTTON] == true) {
			normalKeys[Q_BUTTON] = false;
			currently_normal_pressed--;
		} else {
			//repress the same key :D -- nothing to handle
		}
		break;

	case 'e':
	case 'E':
		if (normalKeys[E_BUTTON] == true) {
			normalKeys[E_BUTTON] = false;
			currently_normal_pressed--;
		} else {
			//repress the same key :D -- nothing to handle
		}
		break;

	case 'c':
	case 'C':
		if (normalKeys[C_BUTTON] == true) {
			normalKeys[C_BUTTON] = false;
			currently_normal_pressed--;
		} else {
			//repress the same key :D -- nothing to handle
		}
		break;
	case 'v':
	case 'V':
		if (normalKeys[V_BUTTON] == true) {
			normalKeys[V_BUTTON] = false;
			currently_normal_pressed--;
		} else {
			//repress the same key :D -- nothing to handle
		}
		break;


	}
}

void passiveMotionListener(int x,int y){

}

int main(int argc, char **argv) {

	//preloading
	heli.loadGivenFile();
	//heli.showVerticesAndFaces();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1024, 768);
	glutCreateWindow("My seminar room :D");

	init();

	glutDisplayFunc(drawing);

	glutKeyboardFunc(keyBoardListener);
	glutSpecialFunc(specialKeyListener);

	glutReshapeFunc(reshapeWindow);
	//glutMotionFunc(mouseEvent);
	glutSpecialUpFunc(specialkeyboardUpFunc);
	glutKeyboardUpFunc(normalkeyboardUpFunc);
	glutPassiveMotionFunc(passiveMotionListener);

	glutMainLoop();

	return 0;
}
