#pragma once
#include "CommonFunc.h"

class ClassifiedPolygon {
public:

	ClassifiedPolygon(vector<GLfloat> _paramList,int _polygonID, GLfloat _zMax,
		Vector3D _color, int _dy = 0, ClassifiedPolygon* _nxtPolygon = NULL) :
		paramList(_paramList), polygonID(_polygonID), zMax(_zMax),
		color(_color), dy(_dy), nxtPolygon(_nxtPolygon) {}

	vector<GLfloat> paramList;
	int polygonID;
	int dy;
	Vector3D color;
	ClassifiedPolygon *nxtPolygon;
	GLfloat zMax;
};
