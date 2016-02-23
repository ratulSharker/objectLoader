/*
 * myGrid.h
 *
 *  Created on: Dec 9, 2013
 *      Author: Ra2l
 */

#ifndef MYGRID_H_
#define MYGRID_H_

#include <gl/freeglut.h>

void drawGrid(int gridNum,int gridLength,int perGridDist){


	gridNum = gridNum / 2;

	glColor3f(1,1,1);

	glBegin(GL_LINES);{
	for(int i = 1; i < gridNum ; i++){



			glVertex2d(-gridLength,i * perGridDist);glVertex2d(gridLength,i * perGridDist);
			glVertex2d(-gridLength,-i * perGridDist);glVertex2d(gridLength,-i * perGridDist);

			glVertex2d(i * perGridDist,-gridLength);glVertex2d(i * perGridDist,gridLength);
			glVertex2d(-i * perGridDist,-gridLength);glVertex2d(-i * perGridDist,gridLength);



	}
	}glEnd();

}

#endif /* MYGRID_H_ */
