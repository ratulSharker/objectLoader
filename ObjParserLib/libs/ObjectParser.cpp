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
ObjectParser::ObjectParser(std::string fileName, bool useList) {
	this->fileName = fileName;
	this->useListAsDataStructure = useList;
}

/*
 * Load the given filename specified in the
 * constructor
 */
void ObjectParser::loadGivenFile() {
	if (this->useListAsDataStructure == false) {
		this->loadGivenFileInDataStructure();
	} else {
		this->loadGivenFileInOpenGLList();
	}
}

/*
 * shows the loaded vertices & faces
 */
void ObjectParser::showVerticesAndFaces() {

	std::cout << "Showing the vertices" << std::endl;
	for (unsigned int i = 0; i < this->vertices.size(); i++) {
		this->vertices[i].show(i + 1);
	}

	std::cout << "Showing the faces" << std::endl;
	for (std::map<std::string, std::vector<Face> >::iterator it =
			this->faces.begin(); it != this->faces.end(); it++) {
		std::cout << "\t" << it->first << std::endl;
		for (unsigned int i = 0; i < it->second.size(); i++) {
			//it->second[i].show(i+1);
		}
	}

}

/*
 * return's names of the loaded objects
 *
 */
std::vector<std::string> ObjectParser::getSubObjectList() {

	std::vector<std::string> subObject;

	for (std::map<std::string, std::vector<Face> >::iterator it =
			this->faces.begin(); it != this->faces.end(); it++) {
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
void ObjectParser::DrawGivenSubobject(std::string objName) {

	std::map<std::string, std::vector<Face> >::iterator it = this->faces.find(
			objName.c_str());

	if (it != this->faces.end()) { // we found a valid face to draw

		for (unsigned int i = 0; i < it->second.size(); i++) {
			//here we get a single face
			if (it->second[i].isFour) {
				glBegin(GL_QUADS);
			} else {
				glBegin(GL_TRIANGLES);
			}

			Vertice *v0 = &this->normals[it->second[i].faceId - 1]; //vertice for the normal
			Vertice *v1 = &this->vertices[it->second[i].A - 1];
			Vertice *v2 = &this->vertices[it->second[i].B - 1];
			Vertice *v3 = &this->vertices[it->second[i].C - 1];

			glNormal3f(v0->X, v0->Y, v0->Z); //specifying the normal

			glVertex3f(v1->X, v1->Y, v1->Z);
			glVertex3f(v2->X, v2->Y, v2->Z);
			glVertex3f(v3->X, v3->Y, v3->Z);

			if (it->second[i].isFour) {
				Vertice *v4 = &this->vertices[it->second[i].D - 1];
				glVertex3f(v4->X, v4->Y, v4->Z);
			}

			glEnd();

		}
	} else {
		//no subObject found in the given object file -- report to STDOUT that

		std::cout << "ERROR :: " << "No subObject found named '" << objName
				<< "' within '" << this->fileName << std::endl;
	}
}

/*
 * basically draws object at the center using the up function call
 * then rotate it along the supplied thetas with angularRotation
 *
 */
void ObjectParser::DrawGivenSubobjectWithAngle(std::string objName,
		float displaceX, float displaceY, float displaceZ, float thetaX,
		float thetaY, float thetaZ, float angularRotation, int rotateAbout,
		bool keepAtCenter) {

	glPushMatrix();
	{

		//translation
		if (keepAtCenter == false) { //when its true the overall drawing will be at the center -- with the angle
			glTranslatef(displaceX, displaceY, displaceZ);
		}

		//rotation
		if (thetaX != 0.0)
			glRotatef(thetaX, 1, 0, 0);
		if (thetaY != 0.0)
			glRotatef(thetaY, 0, 1, 0);
		if (thetaZ != 0.0)
			glRotatef(thetaZ, 0, 0, 1);

		switch (rotateAbout) {
		case X_AXIS:
			glRotatef(angularRotation, 1, 0, 0);
			break;
		case Y_AXIS:
			glRotatef(angularRotation, 0, 1, 0);
			break;
		case Z_AXIS:
			glRotatef(angularRotation, 0, 0, 1);
			break;
		}

		if (thetaX != 0.0)
			glRotatef(-thetaX, 1, 0, 0);
		if (thetaY != 0.0)
			glRotatef(-thetaY, 0, 1, 0);
		if (thetaZ != 0.0)
			glRotatef(-thetaZ, 0, 0, 1);

		glTranslatef(-displaceX, -displaceY, -displaceZ);
		this->DrawGivenSubobject(objName);

	}
	glPopMatrix();

}

/*
 * Draws whole object without any transformation applied
 * (in background it simply draws all the faces)
 */
void ObjectParser::DrawWholeObjectWithNoTransformation() {

	if(this->useListAsDataStructure == false){
		this->DrawWholeObjectWithNoTransformationInDataStructure();
	}
	else{
		this->DrawWholeObjectWithNoTransformationInOpenGLList();
	}

}

/*********************************************************/
//---- all underlying private funtion
/*********************************************************/

/*
 * used for load the whole file into the c data-structures
 */
void ObjectParser::loadGivenFileInDataStructure() {
	std::ifstream objectFile;

	//opening the file -- in read mode
	objectFile.open(this->fileName.c_str(), std::fstream::in);

	if (objectFile.is_open() == false) {
		std::cout << "error opening file -- " << this->fileName << std::endl;
		return;
	}

	char buf[256], tmpBuf[30];
	std::string lastSeenObject;
	int faceStat;

	while (objectFile.eof() == false) {
		objectFile.getline(buf, 256);

		//parser reside here
		if (buf[0] == '#') { //its a comment so continue
			continue;
		} else if (buf[0] == 'o' && buf[1] == ' ') {
			//its a new object in the file
			//1.extract the object name
			//2. entry a new name into maps for that name
			//3.set the last seen name to this object name
			//4.set face status unknown

			sscanf(buf, "o %s", tmpBuf); //extracting the object name
			std::cout << tmpBuf << " --  Loaded" << std::endl;
			lastSeenObject = tmpBuf;

			//this->vertices[lastSeenObject] = std::vector<Vertice>();//assign a vector
			this->faces[lastSeenObject] = std::vector<Face>();

			faceStat = FACE_COORDINATE_UNKNOWN; //setting face value unknown
		} else if (buf[0] == 'v' && buf[1] == ' ') {
			//its a vertice for the last seen object
			//1. scan the X,Y,Z
			//2. push_back a entry to the vector

			//scanning
			float tmpX, tmpY, tmpZ;
			sscanf(buf, "v %f %f %f", &tmpX, &tmpY, &tmpZ);

			//pushing
			this->vertices.push_back(Vertice(tmpX, tmpY, tmpZ));
		} else if (buf[0] == 'v' && buf[1] == 'n') {
			//its a normal of vertice for the last seen object
			//1. scan the X,Y,Z
			//2. push_back a entry to the maps->lastseenobject->normal-vector
			float tmpX, tmpY, tmpZ;
			sscanf(buf, "vn %f %f %f", &tmpX, &tmpY, &tmpZ);

			//pushing the normal vectors
			this->normals.push_back(Vertice(tmpX, tmpY, tmpZ));
		} else if (buf[0] == 'f' && buf[1] == ' ') {
			//its a face for the last seen object
			//0.detecting the number of point making the face
			//1.scan the faces four points
			//2. push back a entry to the maps->lastseenObject->vector

			if (faceStat == FACE_COORDINATE_UNKNOWN) {
				//time to recognize that how many points on faces
				int count = 0;
				for (int p = 0; buf[p] != 0; p++) {
					if (buf[p] == ' ')
						count++;
				}
				if (count == 3) { //three vertices in each face
					faceStat = FACE_COORDINATE_THREE;
				} else if (count == 4) { //four vertices in each face
					faceStat = FACE_COORDINATE_FOUR;
				} else { //too many faces for a single face
						 //no space to allocate this much vertices
						 //exit
					std::cout << "ERROR:: TOO MANY VERTICES FOR A SINGLE FACE."
							<< std::endl;
					std::cout << "object filename :: " << this->fileName
							<< std::endl;
					std::cout << "subobject name :: " << lastSeenObject
							<< std::endl;
					return;
				}
			}
			//extracting the value & pushing
			unsigned int a, b, c, d, id = 0;
			if (faceStat == FACE_COORDINATE_THREE) {
				sscanf(buf, "f %u//%u %u//%u %u//%u", &a, &id, &b, &id, &c,
						&id);

				this->faces[lastSeenObject].push_back(
						Face(a, b, c, 0, id, false));

				//if any object's consist of equal number of vertices -- comment the following line
				faceStat = FACE_COORDINATE_UNKNOWN;	//allowing each object different number of faces
			} else if (faceStat == FACE_COORDINATE_FOUR) {
				sscanf(buf, "f %u//%u %u//%u %u//%u %u//%u", &a, &id, &b, &id,
						&c, &id, &d, &id);

				this->faces[lastSeenObject].push_back(
						Face(a, b, c, d, id, true));

				//if any object's consist of equal number of vertices -- comment the following line
				faceStat = FACE_COORDINATE_UNKNOWN;	//allowing each object different number of faces
			}

		}
	}

	//dont forget to close the shit
	objectFile.close();
}

/*
 * used to load the whole file into the GLnewList
 */
void ObjectParser::loadGivenFileInOpenGLList() {
	std::ifstream objectFile;

	//opening the file -- in read mode
	objectFile.open(this->fileName.c_str(), std::fstream::in);

	if (objectFile.is_open() == false) {
		std::cout << "error opening file -- " << this->fileName << std::endl;
		return;
	}

	char buf[256], tmpBuf[30];
	std::string lastSeenObject;
	int faceStat;

	//local data structure
	std::vector<Vertice*> localVertices,localNormals;
	std::map<std::string*,std::vector<Face*> > localFaces = std::map<std::string*,std::vector<Face*> >();
	std::vector<Face*> *tempVect;
	unsigned int id = 0;


	while (objectFile.eof() == false) {
		objectFile.getline(buf, 256);	//reading a line from file

		//parser reside here
		if (buf[0] == '#') { //its a comment so continue
			continue;
		} else if (buf[0] == 'o' && buf[1] == ' ') {
			//its a new object in the file
			//0.clearing the vectors for the
			//1.extract the object name
			//2. entry a new name into maps for that name
			//3.set the last seen name to this object name
			//4.set face status unknown

			if(localVertices.size() != 0 && localNormals.size() != 0){//there are already assigned vertices
				//close the existing lits
				/*
				glEndList();
				//std::cout<<"glEndList called for the "<<lastSeenObject<<" "<<localVertices.size()<<std::endl;

				//time to clear the vectors
				for(unsigned int i=0;i<localVertices.size();i++){
					//delete each elem
					delete localVertices[i];
				}
				for(unsigned int i=0;i<localNormals.size();i++){
					//delete each elem
					delete localNormals[i];
				}

				localVertices.clear();
				localNormals.clear();

				if(localNormals.size() != 0 || localVertices.size() != 0){
					std::cout<<"ERROR :: AFTER CLEARING THE VECTORS -- THERE REMAINS SOME OBJECTS"<<std::endl;
					return;
				}
				*/
			}


			sscanf(buf, "o %s", tmpBuf); //extracting the object name
			std::cout << tmpBuf << " --  Loading... in GL_LIST" << std::endl;
			lastSeenObject = tmpBuf;

			//this->faces[lastSeenObject] = std::vector<Face>();
			faceStat = FACE_COORDINATE_UNKNOWN; //setting face value unknown

			id++;
			this->objectIdMap[lastSeenObject] = id;
			tempVect = &localFaces[new std::string(lastSeenObject)];
			//std::cout<<"glNewList called for the "<<lastSeenObject<<std::endl;
			//glNewList(id,GL_COMPILE_AND_EXECUTE);//compile a new list with the current id


		} else if (buf[0] == 'v' && buf[1] == ' ') {
			//its a vertice for the last seen object
			//1. scan the X,Y,Z
			//2. push_back a entry to the vector

			//scanning
			float tmpX, tmpY, tmpZ;
			sscanf(buf, "v %f %f %f", &tmpX, &tmpY, &tmpZ);

			//pushing
			localVertices.push_back(new Vertice(tmpX, tmpY, tmpZ));

		} else if (buf[0] == 'v' && buf[1] == 'n') {
			//its a normal of vertice for the last seen object
			//1. scan the X,Y,Z
			//2. push_back a entry to the maps->lastseenobject->normal-vector
			float tmpX, tmpY, tmpZ;
			sscanf(buf, "vn %f %f %f", &tmpX, &tmpY, &tmpZ);

			//pushing the normal vectors
			localNormals.push_back(new Vertice(tmpX, tmpY, tmpZ));

		} else if (buf[0] == 'f' && buf[1] == ' ') {
			//its a face for the last seen object
			//0.detecting the number of point making the face
			//1.scan the faces four points
			//2. push back a entry to the maps->lastseenObject->vector

			if (faceStat == FACE_COORDINATE_UNKNOWN) {
				//time to recognize that how many points on faces
				int count = 0;
				for (int p = 0; buf[p] != 0; p++) {
					if (buf[p] == ' ')
						count++;
				}
				if (count == 3) { //three vertices in each face
					faceStat = FACE_COORDINATE_THREE;
				} else if (count == 4) { //four vertices in each face
					faceStat = FACE_COORDINATE_FOUR;
				} else { //too many faces for a single face
						 //no space to allocate this much vertices
						 //exit
					std::cout << "ERROR:: TOO MANY VERTICES FOR A SINGLE FACE. GL_LIST"
							<< std::endl;
					std::cout << "object filename :: " << this->fileName
							<< " GL_LIST" <<  std::endl;
					std::cout << "subobject name :: " << lastSeenObject
							<< " GL_LIST"<<std::endl;
					return;
				}
			}
			//extracting the value & pushing
			unsigned int a, b, c, d, id = 0;
			if (faceStat == FACE_COORDINATE_THREE) {
				sscanf(buf, "f %u//%u %u//%u %u//%u", &a, &id, &b, &id, &c,
						&id);
				//this->faces[lastSeenObject].push_back(Face(a, b, c, 0, id, false));

				//time to draw a single face
				/*glBegin(GL_TRIANGLES);{


					Vertice *vn = localNormals[id-1  ];
					Vertice *va = localVertices[a-1 ];
					Vertice *vb = localVertices[b-1 ];
					Vertice *vc = localVertices[c-1 ];

					glNormal3f(vn->X,vn->Y,vn->Z);
					glVertex3f(va->X,va->Y,va->Z);
					glVertex3f(vb->X,vb->Y,vb->Z);
					glVertex3f(vc->X,vc->Y,vc->Z);



				}glEnd();*/

				//localFaces[new std::string(lastSeenObject)].push_back(new Face(a, b, c, 0, id, false));
				tempVect->push_back(new Face(a, b, c, 0, id, false));
				//if any object's consist of equal number of vertices -- comment the following line
				faceStat = FACE_COORDINATE_UNKNOWN;	//allowing each object different number of faces


			} else if (faceStat == FACE_COORDINATE_FOUR) {
				sscanf(buf, "f %u//%u %u//%u %u//%u %u//%u", &a, &id, &b, &id,
						&c, &id, &d, &id);

				//this->faces[lastSeenObject].push_back(Face(a, b, c, d, id, true));

				//time to draw a single face which is rectangle
				/*glBegin(GL_QUADS);{

					Vertice *vn = localNormals[id-1 ];
					Vertice *va = localVertices[a-1  ];
					Vertice *vb = localVertices[b-1  ];
					Vertice *vc = localVertices[c-1 ];
					Vertice *vd = localVertices[d-1 ];


					glNormal3f(vn->X,vn->Y,vn->Z);
					glVertex3f(va->X,va->Y,va->Z);
					glVertex3f(vb->X,vb->Y,vb->Z);
					glVertex3f(vc->X,vc->Y,vc->Z);
					glVertex3f(vd->X,vd->Y,vd->Z);

				}glEnd();*/

				//localFaces[new std::string(lastSeenObject)].push_back(new Face(a, b, c, d, id, false));
				tempVect->push_back(new Face(a, b, c, 0, id, false));
				//if any object's consist of equal number of vertices -- comment the following line
				faceStat = FACE_COORDINATE_UNKNOWN;	//allowing each object different number of faces
			}

		}

	}

	//all local list are maden -- now time to compile them
	//std::cout<<"Number Of obj:"<<localFaces.size()<<std::endl;
	for(std::map<std::string*, std::vector<Face*> >::iterator it = localFaces.begin();
			it != localFaces.end() ; it++){
		//it->first get the first object name -- the string name
		std::cout<<"Object name :: "<<*it->first<< " id=" <<this->objectIdMap[*it->first]<<std::endl;
		glNewList(this->objectIdMap[*it->first],GL_COMPILE);

		for(unsigned int i=0;i<it->second.size();i++){
			Face *f = it->second[i];
			if(f->isFour)
				glBegin(GL_QUADS);
			else
				glBegin(GL_TRIANGLES);
			{
				Vertice *vn = localNormals[f->faceId - 1] ;
				Vertice *v1 = localVertices[f->A - 1];
				Vertice *v2 = localVertices[f->B - 1];
				Vertice *v3 = localVertices[f->C - 1];
				Vertice *v4 = localVertices[f->D - 1];

				glNormal3f(vn->X,vn->Y,vn->Z);
				glVertex3f(v1->X,v1->Y,v1->Z);
				glVertex3f(v2->X,v2->Y,v2->Z);
				glVertex3f(v3->X,v3->Y,v3->Z);
				if(f->isFour){
					glVertex3f(v4->X,v4->Y,v4->Z);
				}
			}
			glEnd();

		}

		glEndList();//an object creation is completed
	}

	//you have to clean up all the mess u done with local-xxxxxx

}

void ObjectParser::DrawWholeObjectWithNoTransformationInDataStructure(){
	for (std::map<std::string, std::vector<Face> >::iterator it =
				this->faces.begin(); it != this->faces.end(); it++) {

			//here we get a object
			for (unsigned int i = 0; i < it->second.size(); i++) {
				//here we get a single face
				if (it->second[i].isFour) {
					glBegin(GL_QUADS);
				} else {
					glBegin(GL_TRIANGLES);
				}

				Vertice *v0 = &this->normals[it->second[i].faceId - 1]; //vertice for the normal
				Vertice *v1 = &this->vertices[it->second[i].A - 1];
				Vertice *v2 = &this->vertices[it->second[i].B - 1];
				Vertice *v3 = &this->vertices[it->second[i].C - 1];

				glNormal3f(v0->X, v0->Y, v0->Z); //specifying the normal

				glVertex3f(v1->X, v1->Y, v1->Z);
				glVertex3f(v2->X, v2->Y, v2->Z);
				glVertex3f(v3->X, v3->Y, v3->Z);

				if (it->second[i].isFour) {
					Vertice *v4 = &this->vertices[it->second[i].D - 1];
					glVertex3f(v4->X, v4->Y, v4->Z);
				}
				glEnd();
			}
		}
}

void ObjectParser::DrawWholeObjectWithNoTransformationInOpenGLList(){
	glColor3f(1,1,1);
	for(std::map<std::string,int>::iterator it = this->objectIdMap.begin();
			it != this->objectIdMap.end(); it++){
		//loop all the object existed
		glCallList(this->objectIdMap[it->first]);

	}
}
