#pragma once
#include "CommonFunc.h"

class ClassifiedEdge
{
public:
	ClassifiedEdge() :x(0), yMax(0), dx(0), dy(-1), polygonID(-1), nxtEdge(NULL) {}
	ClassifiedEdge(GLfloat _x, GLfloat _yMax, GLfloat _dx, int _dy, int _polygonID = -1, ClassifiedEdge* _nxtEdge = NULL) :
		x(_x), yMax(_yMax), dx(_dx), dy(_dy), polygonID(_polygonID), nxtEdge(_nxtEdge) {}

//private:
	GLfloat x;
	GLfloat yMax;
	GLfloat dx;
	int dy;
	int polygonID;
	ClassifiedEdge *nxtEdge;
};
