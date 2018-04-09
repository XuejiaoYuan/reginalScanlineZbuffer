#pragma once
#include "CommonFunc.h"
#include "Vector3D.h"
#include "ClassifiedEdge.h"
#include "ActiveEdge.h"

class Objects {
public:
	Objects();
	void InitializeObj(const string& ObjName, const int winWidth, const int winHeight, bool acc = false);
	void CalculateFaceParam(vector<GLfloat>& paramList, Vector3D& color, int faceIndex);
	void CalculateFaceMaxYZ(int& maxY, GLfloat& maxZ, int faceIndex);
	void CalculateFaceDeltaY(int& dy, int faceIndex);
	void CalculateFaceEdge(vector<ClassifiedEdge*>& edges, int faceIndex);
	void Rotate(int key);

	vector<Vector3D> verticesList;
	vector<vector<int>> faceIndexList;
	vector<Vector3D> colorList;
	vector<bool> isBackList;

	GLfloat yMin;
	GLfloat yMax;
	GLfloat xMax;
	GLfloat xMin;

private:
	bool accelerate;
	int winWidth;
	int winHeight;
	vector<Vector3D> originVerticesList;
	void ReadFile(const string & objName);
	void CalculateEdgeParam(int vIndex1, int vIndex2, ClassifiedEdge* edge, int polygonID);
	void ResizeVertices();

};