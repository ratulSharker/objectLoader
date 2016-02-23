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
#include "helper/GridDrawing.h" 	// primitive grid drawing
#include "helper/CameraManager.h"	// camera management
#include "../libs/ObjectParser.h" 	// needed for the object parser class
#include <vector>

/*
 * loading of other assets
ObjectParser heli("asset/heli.obj",true);
ObjectParser porsche911("asset/porsche.obj",false);
ObjectParser aventador("asset/aventador.obj",false);
ObjectParser lady("asset/lady.obj",true);
ObjectParser sr71("asset/sr71.obj",false);
*/

//global objects
ObjectParser city("asset/aventador.obj", true); //initialize the class that i made as global - so anyone can run any operation on that
CameraManager camMgr;


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
bool normalKeys[10] = { false,
						false,
						false,
						false,
						false,
						false,
						false,
						false,
						false,
						false };

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
	float color[] = {1,1,1,1};
	glLightfv(GL_LIGHT0,GL_AMBIENT_AND_DIFFUSE,color);

	//material and enable coloring
	glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
	glEnable ( GL_COLOR_MATERIAL );
}

void drawing() {

	//clearing :D
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	//matrix operations
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//lighting codes
	float position[] = {100,100,400,1};
	glLightfv(GL_LIGHT0,GL_POSITION,position);

	//camera settings
	gluLookAt(	camMgr.getCameraX(),
				camMgr.getCameraY(),
				camMgr.getCameraZ(),
				camMgr.getLookAtX(),
				camMgr.getLookAtY(),
				camMgr.getLookAtZ(), 0, 0, 1);

	//drawing code should be here
	drawGrid(10,200,5);


/*//whenever heli object is uncommented above u can uncomment the following codes then press F to
 * rotate the main rotor of the helicopter
 */

/*

	//heli.DrawGivenSubobjectWithAngle("rear_rotor",6.3022,-1.04241,4.75804,10,20,30,fan_angle,ObjectParser::Y_AXIS,true);
	//heli.DrawGivenSubobject("rear_rotor",0,0,0);
	//heli.DrawWholeObjectWithNoTransformation();
	glColor3f(1,1,1);
	heli.DrawGivenSubobject("fuselage");
	heli.DrawGivenSubobject("cabinDoor_");
	heli.DrawGivenSubobject("Door_R");
	heli.DrawGivenSubobject("Door_L");
	heli.DrawGivenSubobject("elevator");
	heli.DrawGivenSubobject("tyre_R");
	heli.DrawGivenSubobject("rear_Tyre");
	heli.DrawGivenSubobject("Tyre_L");
	heli.DrawGivenSubobject("Window_L2");
	heli.DrawGivenSubobject("Window_L1");
	heli.DrawGivenSubobject("Window_R2");
	heli.DrawGivenSubobject("window_R1");
	heli.DrawGivenSubobject("cabinDoo13");

	//drawing main rotor
	heli.DrawGivenSubobjectWithAngle("main_rotor",6.32871,10.69359,4.16496,-90,0,0,fan_angle,ObjectParser::Y_AXIS,false);
	heli.DrawGivenSubobjectWithAngle("rear_rotor",6.41021,-1.04241,4.75804,7.004,0,-90,fan_angle,ObjectParser::Y_AXIS,false);
*/


	//porsche911.DrawWholeObjectWithNoTransformation();
	//aventador.DrawWholeObjectWithNoTransformation();
	//lady.DrawWholeObjectWithNoTransformation();
	//sr71.DrawWholeObjectWithNoTransformation();

	glPushMatrix();{

		//here u can do any kind of transformation here
		glScalef(1,1,1);
		glRotatef(45,0,0,1);
		city.DrawWholeObjectWithNoTransformation();

	}glPopMatrix();



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

	// 	for animation usage only
	//	currently no animation is
	//	done, thats why it's implementation is meant to kept blank

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
				camMgr.lookUpward();

				break;

			case A_BUTTON:				//'a':
				//case 'A':
				//look leftward -- update angle + currently looking values
				camMgr.lookLeftward();

				break;
			case D_BUTTON:				//'d':
				//case 'D':
				//look rightward -- update angle + currently looking values
				camMgr.lookRightward();


				break;

			case S_BUTTON:				//'s':
				//case 'S':
				//looking downward
				camMgr.lookDownward();

				break;

			case Q_BUTTON:			//'q':
				//case 'Q': //go up
				camMgr.moveCameraUp();
				break;

			case E_BUTTON:			//'e':
				//case 'E':
				camMgr.moveCameraDown();
				break;
			}
		}

	} else {			//otherwise called from normal call back function
		switch (ch) {

		case 27:
			glutExit();
			return ;
			break;

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
				camMgr.moveForward();
				break;

			case DOWN_ARROW:					//GLUT_KEY_DOWN:
				//moving backward
				camMgr.moveBackward();
				break;

			case LEFT_ARROW:					//GLUT_KEY_LEFT:
				//moving left
				camMgr.moveLeftward();

				break;

			case RIGHT_ARROW:					//GLUT_KEY_RIGHT:
				//moving right
				camMgr.moveRightward();
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
			return;
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

void passiveMotionListener(int x,int y)
{

}

int main(int argc, char **argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1024, 768);
	glutCreateWindow("object - library");

	init();

	city.loadGivenFile(); //loading the file must be called after the init function called
	std::cout<<"Loading complete"<<std::endl;


	//
	//	providing the drawing function
	//
	glutDisplayFunc(drawing);

	//
	//	window reshape listener
	//
	glutReshapeFunc(reshapeWindow);

	//
	//	keyboard events
	//
	glutKeyboardFunc(keyBoardListener);
	glutSpecialFunc(specialKeyListener);


	//glutMotionFunc(mouseEvent);
	glutSpecialUpFunc(specialkeyboardUpFunc);
	glutKeyboardUpFunc(normalkeyboardUpFunc);
	//glutPassiveMotionFunc(passiveMotionListener);

	//
	//	starting the main loop
	//
	glutMainLoop();

	return 0;
}
