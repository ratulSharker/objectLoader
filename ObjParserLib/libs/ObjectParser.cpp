#include "ObjectParser.h"

#include <fstream>
#include "Vertice.h"
#include <vector>
#include <map>
#include <cstdlib>
#include <cstring>
#include <GL/freeglut.h>

/*
 * simply save the filename given nothing else
 */
ObjectParser::ObjectParser(std::string fileName){
	this->fileName = fileName;
}

/*
 * Load the given filename specified in the
 * constructor
 */
void ObjectParser::loadGivenFile(){
	std::ifstream objectFile;

		//opening the file -- in read mode
		objectFile.open(this->fileName.c_str(),std::fstream::in);

		if(objectFile.is_open() == false) {
			std::cout<<"error opening file -- "<< this->fileName <<std::endl;
			return;
		}

		char buf[256],tmpBuf[30];
		std::string lastSeenObject;
		int faceStat;

		while(objectFile.eof() == false){
			objectFile.getline(buf,256);

			//parser reside here
			if(buf[0] == '#'){//its a comment so continue
				continue;
			}
			else if(buf[0] == 'o' && buf[1] == ' '){
				//its a new object in the file
				//1.extract the object name
				//2. entry a new name into maps for that name
				//3.set the last seen name to this object name
				//4.set face status unknown

				sscanf(buf,"o %s",tmpBuf); //extracting the object name
				std::cout<<tmpBuf<< " --  Loaded"<<std::endl;
				lastSeenObject = tmpBuf;


				//this->vertices[lastSeenObject] = std::vector<Vertice>();//assign a vector
				this->faces[lastSeenObject] = std::vector<Face>();

				faceStat = FACE_COORDINATE_UNKNOWN; //setting face value unknown
			}
			else if(buf[0] == 'v' && buf[1] == ' '){
				//its a vertice for the last seen object
				//1. scan the X,Y,Z
				//2. push_back a entry to the vector

				//scanning
				float tmpX,tmpY,tmpZ;
				sscanf(buf,"v %f %f %f",&tmpX,&tmpY,&tmpZ);

				//pushing
				this->vertices.push_back(Vertice(tmpX,tmpY,tmpZ));
			}
			else if(buf[0] == 'v' && buf[1] == 'n'){
				//its a normal of vertice for the last seen object
				//1. scan the X,Y,Z
				//2. push_back a entry to the maps->lastseenobject->normal-vector
				float tmpX,tmpY,tmpZ;
				sscanf(buf,"vn %f %f %f",&tmpX,&tmpY,&tmpZ);

				//pushing the normal vectors
				this->normals.push_back(Vertice(tmpX,tmpY,tmpZ));
			}
			else if(buf[0] == 'f' && buf[1] == ' '){
				//its a face for the last seen object
				//0.detecting the number of point making the face
				//1.scan the faces four points
				//2. push back a entry to the maps->lastseenObject->vector

				if(faceStat == FACE_COORDINATE_UNKNOWN){
					//time to recognize that how many points on faces
					int count = 0;
					for(int p = 0;buf[p] != 0;p++){
						if(buf[p] == ' ')count++;
					}
					if(count == 3){ //three vertices in each face
						faceStat = FACE_COORDINATE_THREE;
					}
					else if(count == 4){//four vertices in each face
						faceStat = FACE_COORDINATE_FOUR;
					}
				}
				//extracting the value & pushing
				unsigned int a,b,c,d,id = 0;
				if(faceStat == FACE_COORDINATE_THREE){
					sscanf(buf,"f %u//%u %u//%u %u//%u",&a,&id,&b,&id,&c,&id);

					this->faces[lastSeenObject].push_back(Face(a,b,c,0,id,false));

					//if any object's consist of equal number of vertices -- comment the following line
					faceStat = FACE_COORDINATE_UNKNOWN;//allowing each object different number of faces
				}
				else if(faceStat == FACE_COORDINATE_FOUR){
					sscanf(buf,"f %u//%u %u//%u %u//%u %u//%u",&a,&id,&b,&id,&c,&id,&d,&id);

					this->faces[lastSeenObject].push_back(Face(a,b,c,d,id,true));

					//if any object's consist of equal number of vertices -- comment the following line
					faceStat = FACE_COORDINATE_UNKNOWN;//allowing each object different number of faces
				}

			}
		}

		//dont forget to close the shit
		objectFile.close();
}

/*
 * shows the loaded vertices & faces
 */
void ObjectParser::showVerticesAndFaces(){

	std::cout<<"Showing the vertices"<<std::endl;
	for(unsigned int i=0;i<this->vertices.size();i++){
			this->vertices[i].show(i + 1);
	}

	std::cout<<"Showing the faces"<<std::endl;
	for(std::map<std::string,std::vector<Face> >::iterator it = this->faces.begin(); it != this->faces.end() ; it++){
		std::cout<<"\t"<<it->first<<std::endl;
		for(unsigned int i=0;i<it->second.size();i++){
			//it->second[i].show(i+1);
		}
	}


}


/*
 * return's names of the loaded objects
 *
 */
std::vector<std::string> ObjectParser::getSubObjectList(){

	std::vector<std::string> subObject;

	for(std::map<std::string, std::vector<Face> >::iterator it = this->faces.begin(); it != this->faces.end();it++){
		subObject.push_back(it->first);
	}

	return subObject;
}

/*
 * draw the given subject within the loaded object file -- without any rotation
 * first param is the name of the subObject
 * other 3 params are dis..X,dis..Y,dis..Z which are set to the
 * object transfomation value. you can get the transformation values from the
 * modeling software for each of the object's. By default to draw actually as the
 * model shows we call this function with the default value 0,0,0. But if we want
 * to draw our object initially at the origin we have to pass the values of the
 * last three variable readen from our modeling software
 *
 */
void ObjectParser::DrawGivenSubobject(std::string objName,float displaceX,float displaceY,float displaceZ){

	std::map<std::string,std::vector<Face> >::iterator it = this->faces.find(objName.c_str());

	if(it != this->faces.end()){ // we found a valid face to draw

		for(unsigned int i=0;i<it->second.size();i++){
			//here we get a single face
				if(it->second[i].isFour){
					glBegin(GL_QUADS);
				}
				else{
					glBegin(GL_TRIANGLES);
				}

				Vertice *v0 = &this->normals[it->second[i].faceId - 1]; //vertice for the normal
				Vertice *v1 = &this->vertices[it->second[i].A - 1];
				Vertice *v2 = &this->vertices[it->second[i].B - 1];
				Vertice *v3 = &this->vertices[it->second[i].C - 1];

				glNormal3f(v0->X,v0->Y,v0->Z);//specifying the normal

				glVertex3f(v1->X-displaceX,v1->Y-displaceY,v1->Z-displaceZ);
				glVertex3f(v2->X-displaceX,v2->Y-displaceY,v2->Z-displaceZ);
				glVertex3f(v3->X-displaceX,v3->Y-displaceY,v3->Z-displaceZ);

				if(it->second[i].isFour){
					Vertice *v4 = &this->vertices[it->second[i].D - 1];
					glVertex3f(v4->X-displaceX,v4->Y-displaceY,v4->Z-displaceZ);
				}

				glEnd();

			}
	}
	else{
		//no subObject found in the given object file -- report to STDOUT that

		std::cout<<"ERROR :: "<<"No subObject found named '"<<objName<<"' within '"<<this->fileName<<std::endl;
	}
}

/*
 * basically draws object at the center using the up function call
 * then rotate it along the supplied thetas with angularRotation
 *
 */
void ObjectParser::DrawGivenSubobjectWithAngle(std::string objName,float displaceX,float displaceY,float displaceZ,
			float thetaX,float thetaY,float thetaZ,float angularRotation,int rotateAbout,bool keepAtCenter){

	glPushMatrix();{

		//translation
		if(keepAtCenter == false){//when its true the overall drawing will be at the center -- with the angle
			glTranslatef(displaceX,displaceY,displaceZ);
		}

		//rotation
		if(thetaX != 0.0)
		glRotatef(thetaX,1,0,0);
		if(thetaY != 0.0)
		glRotatef(thetaY,0,1,0);
		if(thetaZ != 0.0)
		glRotatef(thetaZ,0,0,1);

		switch(rotateAbout){
		case X_AXIS:
			glRotatef(angularRotation,1,0,0);
			break;
		case Y_AXIS:
			glRotatef(angularRotation,0,1,0);
			break;
		case Z_AXIS:
			glRotatef(angularRotation,0,0,1);
			break;
		}

		if(thetaX != 0.0)
		glRotatef(-thetaX,1,0,0);
		if(thetaY != 0.0)
		glRotatef(-thetaY,0,1,0);
		if(thetaZ != 0.0)
		glRotatef(-thetaZ,0,0,1);
		this->DrawGivenSubobject(objName,displaceX,displaceY,displaceZ);

	}glPopMatrix();


}

/*
 * Draws whole object without any transformation applied
 * (in background it simply draws all the faces)
 */
void ObjectParser::DrawWholeObjectWithNoTransformation(){

	for(std::map<std::string,std::vector<Face> >::iterator it = this->faces.begin(); it != this->faces.end() ; it++){

		//here we get a object
		for(unsigned int i=0;i<it->second.size();i++){
			//here we get a single face
				if(it->second[i].isFour){
					glBegin(GL_QUADS);
				}
				else{
					glBegin(GL_TRIANGLES);
				}


				Vertice *v0 = &this->normals[it->second[i].faceId - 1]; //vertice for the normal
				Vertice *v1 = &this->vertices[it->second[i].A - 1];
				Vertice *v2 = &this->vertices[it->second[i].B - 1];
				Vertice *v3 = &this->vertices[it->second[i].C - 1];


				glNormal3f(v0->X,v0->Y,v0->Z);//specifying the normal

				glVertex3f(v1->X,v1->Y,v1->Z);
				glVertex3f(v2->X,v2->Y,v2->Z);
				glVertex3f(v3->X,v3->Y,v3->Z);

				if(it->second[i].isFour){
					Vertice *v4 = &this->vertices[it->second[i].D - 1];
					glVertex3f(v4->X,v4->Y,v4->Z);
				}
				glEnd();
			}
		}
}
