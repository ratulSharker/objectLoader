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

public:
	std::vector<Vertice> vertices; //data structure for the vertices
	std::map<std::string,std::vector<Face> > faces; //data structure for the faces

	ObjectParser(std::string fileName);
	void loadGivenFile();
	void showVerticesAndFaces();
	std::vector<std::string> getSubObjectList();
	void DrawGivenSubobject(std::string objName,float displaceX,float displaceY,float displaceZ);

	void DrawGivenSubobjectWithAngle(std::string objName,float displaceX,float displaceY,float displaceZ,
			float thetaX,float thetaY,float angularRotation,bool keepAtCenter);

};

#endif /* OBJECTPARSER_H_ */
