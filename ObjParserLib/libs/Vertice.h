/*
 * Vertices.h
 *
 *  Created on: Dec 29, 2013
 *      Author: Ra2l
 */

#ifndef VERTICES_H_
#define VERTICES_H_

#include <iostream>

class Vertice{
public:
	float X,Y,Z;

	Vertice(float x,float y,float z){
		this->X = x;
		this->Y = y;
		this->Z = z;
	}

	void show(int tag){
		std::cout<<"#"<<tag<<". "<<this->X << " " <<this->Y << " " <<this->Z<<std::endl;
	}
};

#endif /* VERTICES_H_ */
