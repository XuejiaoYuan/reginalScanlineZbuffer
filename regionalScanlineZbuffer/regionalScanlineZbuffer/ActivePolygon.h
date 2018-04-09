#pragma once
#include "CommonFunc.h"

class ActivePolygon {
public:
	ActivePolygon() :zMax(0), polygonID(-1), resDy(-1), flag(false), nxtPolygon(NULL) {}
	ActivePolygon(const vector<GLfloat>& _paramList, int _polygonID, int _resDy,
		Vector3D _color, GLfloat _zMax, bool _flag = false, ActivePolygon *_nxtPolygon = NULL) :
		paramList(_paramList), polygonID(_polygonID), resDy(_resDy), 
		color(_color), zMax(_zMax), flag(_flag), nxtPolygon(_nxtPolygon) {};


//private:
	GLfloat zMax;
	vector<GLfloat> paramList;
	int polygonID;
	int resDy;
	Vector3D color;
	bool flag;
	ActivePolygon *nxtPolygon;
};