/*
 * ObjectParser.h
 *
 *  Created on: Dec 28, 2013
 *      Author: Ra2l
 */

#ifndef OBJECTPARSER_H_
#define OBJECTPARSER_H_

#include <iostream>
#include <map>
#include <vector>
#include "Vertice.h"
#include "Face.h"

class ObjectParser{
private:

#define FACE_COORDINATE_UNKNOWN 0
#define FACE_COORDINATE_THREE 1
#define FACE_COORDINATE_FOUR 2


	std::string fileName;
	std::vector<Vertice> vertices; //data structure for the vertices
	std::map<std::string,std::vector<Face> > faces; //data structure for the faces
	std::vector<Vertice> normals; //data structure for the faces

public:

const static int X_AXIS  = 3;
const static int Y_AXIS  = 4;
const static int Z_AXIS  = 5;

	ObjectParser(std::string fileName);
	void loadGivenFile();
	void showVerticesAndFaces();
	std::vector<std::string> getSubObjectList();
	void DrawGivenSubobject(std::string objName);
	void DrawWholeObjectWithNoTransformation();

	/*too much memory & CPU consuming code*/
	void DrawGivenSubobjectWithAngle(std::string objName,float displaceX,float displaceY,float displaceZ,
			float thetaX,float thetaY,float thetaZ,float angularRotation,int rotateAbout,bool keepAtCenter);

};

#endif /* OBJECTPARSER_H_ */
