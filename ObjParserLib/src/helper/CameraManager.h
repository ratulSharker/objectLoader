/*
 * CameraManager.h
 *
 *  Created on: Feb 22, 2016
 *      Author: ratul
 */

#ifndef HELPER_CAMERAMANAGER_H_
#define HELPER_CAMERAMANAGER_H_

#include <cmath>

#define PI 3.14159265
#define LOOKING_DISTANCE 30
#define LOOKING_HEIGHT 20
#define LOOKING_HEIGHT_MAX 60
#define LOOKING_HEIGHT_MIN -10
#define CAMERA_HEIGHT 30

class CameraManager
{
/*
 * private section
 */
	double	currentCameraX,
			currentCameraY,
			currentCameraZ;
	double	currentLookingAtX,
			currentLookingAtY,
			currentLookingAtZ;
	double	CameraMovingAngle;
	double	movingSpeed,
			LookAngleDeviation;

	/*
	 * public section
	 */
public:

	CameraManager()
	{
		//initiate with the default values

		//calculate the initial looking + camera looking at the first quadtrant
		currentCameraX = -200 * 1.3 * 0.75; //width of the room
		currentCameraY = 150 * 1.3 / 2;
		currentCameraZ = CAMERA_HEIGHT;

		currentLookingAtX = currentCameraX
				+ LOOKING_DISTANCE * cos(CameraMovingAngle * PI / 180);
		currentLookingAtY = currentCameraY
				+ LOOKING_DISTANCE * sin(CameraMovingAngle * PI / 180);
		currentLookingAtZ = LOOKING_HEIGHT;

		CameraMovingAngle = 0;
		movingSpeed = 2,
		LookAngleDeviation = 1;
	}


	//getter
	double inline getCameraX(){ return this->currentCameraX; }
	double inline getCameraY(){ return this->currentCameraY; }
	double inline getCameraZ(){ return this->currentCameraZ; }

	double inline getLookAtX(){ return this->currentLookingAtX; }
	double inline getLookAtY(){ return this->currentLookingAtY; }
	double inline getLookAtZ(){ return this->currentLookingAtZ; }


	//manipulator
	void moveForward()
	{
		currentCameraX += movingSpeed
				* cos(CameraMovingAngle * PI / 180);
		currentLookingAtX += movingSpeed
				* cos(CameraMovingAngle * PI / 180);
		currentCameraY += movingSpeed
				* sin(CameraMovingAngle * PI / 180);
		currentLookingAtY += movingSpeed
				* sin(CameraMovingAngle * PI / 180);
	}

	void moveBackward()
	{
		currentCameraX -= movingSpeed
								* cos(CameraMovingAngle * PI / 180);
						currentLookingAtX -= movingSpeed
								* cos(CameraMovingAngle * PI / 180);
						currentCameraY -= movingSpeed
								* sin(CameraMovingAngle * PI / 180);
						currentLookingAtY -= movingSpeed
								* sin(CameraMovingAngle * PI / 180);
	}

	void moveLeftward()
	{
		currentCameraX += movingSpeed
				* cos((CameraMovingAngle + 90) * PI / 180);
		currentLookingAtX += movingSpeed
				* cos((CameraMovingAngle + 90) * PI / 180);
		currentCameraY += movingSpeed
				* sin((CameraMovingAngle + 90) * PI / 180);
		currentLookingAtY += movingSpeed
				* sin((CameraMovingAngle + 90) * PI / 180);
	}

	void moveRightward()
	{
		currentCameraX -= movingSpeed
				* cos((CameraMovingAngle + 90) * PI / 180);
		currentLookingAtX -= movingSpeed
				* cos((CameraMovingAngle + 90) * PI / 180);
		currentCameraY -= movingSpeed
				* sin((CameraMovingAngle + 90) * PI / 180);
		currentLookingAtY -= movingSpeed
				* sin((CameraMovingAngle + 90) * PI / 180);
	}


	void lookUpward()
	{
		if (currentLookingAtZ < LOOKING_HEIGHT_MAX)
			currentLookingAtZ += 1;
	}

	void lookDownward()
	{
		if (currentLookingAtZ > LOOKING_HEIGHT_MIN)
			currentLookingAtZ -= 1;
	}

	void lookLeftward()
	{
		CameraMovingAngle += LookAngleDeviation;
						currentLookingAtX = currentCameraX
								+ LOOKING_DISTANCE * cos(CameraMovingAngle * PI / 180);
						currentLookingAtY = currentCameraY
								+ LOOKING_DISTANCE * sin(CameraMovingAngle * PI / 180);
	}

	void lookRightward()
	{
		CameraMovingAngle -= LookAngleDeviation;
						currentLookingAtX = currentCameraX
								+ LOOKING_DISTANCE * cos(CameraMovingAngle * PI / 180);
						currentLookingAtY = currentCameraY
								+ LOOKING_DISTANCE * sin(CameraMovingAngle * PI / 180);
	}


	void moveCameraUp()
	{
		currentCameraZ++;
		currentLookingAtZ++;
	}

	void moveCameraDown()
	{
		currentCameraZ--;
		currentLookingAtZ--;
	}
};



#endif /* HELPER_CAMERAMANAGER_H_ */
