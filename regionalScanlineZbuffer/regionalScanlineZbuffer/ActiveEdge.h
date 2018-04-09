#pragma once
#include "CommonFunc.h"

class ActiveEdge {
public:
	ActiveEdge() :x(0), dx(0), resDy(-1), polygonID(-1), nxtEdge(NULL) {};
	ActiveEdge(GLfloat _x, GLfloat _dx, int _resDy, int _polygonID, ActiveEdge* _nxtEdge = NULL) :
		x(_x), dx(_dx), resDy(_resDy), polygonID(_polygonID), nxtEdge(_nxtEdge) {}

//private:
	GLfloat x;
	GLfloat dx;
	int resDy;
	int polygonID;
	ActiveEdge* nxtEdge;
};