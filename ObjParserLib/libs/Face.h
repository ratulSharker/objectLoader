/*
 * Face.h
 *
 *  Created on: Dec 29, 2013
 *      Author: Ra2l
 */

#ifndef FACE_H_
#define FACE_H_

class Face{
public:
	unsigned int A,B,C,D;
	bool isFour;
	Face(unsigned int a,unsigned int b,unsigned int c,unsigned int d = 0,bool isFou = false){
		this->A = a;
		this->B = b;
		this->C = c;
		this->D = d;
		this->isFour = isFou;
	}
	void show(int tag){
		if(isFour)
			std::cout<<"#"<<tag<<". "<<this->A << " " <<this->B << " " <<this->C<<" "<<this->D<<std::endl;
		else
			std::cout<<"#"<<tag<<". "<<this->A << " " <<this->B << " " <<this->C<<std::endl;
	}
};



#endif /* FACE_H_ */
