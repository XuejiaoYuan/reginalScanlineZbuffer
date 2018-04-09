#include "Objects.h"

Objects::Objects()
{
}

void Objects::InitializeObj(const string & ObjName, const int _winWidth, const int _winHeight, bool _acc)
{
	accelerate = _acc;
	winWidth = _winWidth;
	winHeight = _winHeight;
	ReadFile(ObjName);
	ResizeVertices();
	if (accelerate)
		isBackList.resize(faceIndexList.size(), false);
	colorList.resize(faceIndexList.size());
}

void Objects::ReadFile(const string & objName)
{
	fstream inFile(objName);
	string s;
	stringstream ss;
	char v;
	GLfloat x, y, z;
	yMin = INT_MAX;
	yMax = INT_MIN;
	xMin = INT_MAX;
	xMax = INT_MIN;
	while (getline(inFile, s)) {
		if (s[0] == 'v'&&s[1] == ' ') {
			ss.clear();
			ss.str(s);
			ss >> v >> x >> y >> z;
			originVerticesList.push_back(Vector3D(x, y, z));
			yMin = yMin > y ? y : yMin;
			yMax = yMax < y ? y : yMax;
			xMin = xMin > x ? x : xMin;
			xMax = xMax < x ? x : xMax;
		}
		else if (s[0] == 'f' && s[1] == ' ') {
			ss.clear();
			ss.str(s);
			string::iterator iter;
			vector<int> index(3);
			vector<int> texture(3);
			vector<string> tmpS(4);
			char c = '/';
			ss >> tmpS[0] >> tmpS[1] >> tmpS[2] >> tmpS[3];
			for (int i = 1; i < 4; i++) {
				iter = find(tmpS[i].begin(), tmpS[i].end(), c);
				index[i - 1] = stoi(tmpS[i].substr(0, iter - tmpS[i].begin()));
			}

			index[0]--;
			index[1]--;
			index[2]--;
			faceIndexList.push_back(index);
		}
		else
			continue;
	}

}

void Objects::CalculateEdgeParam(int vIndex1, int vIndex2, ClassifiedEdge * edge, int polygonID)
{
	Vector3D v1 = verticesList[vIndex1];
	Vector3D v2 = verticesList[vIndex2];

	edge->x = v1.y > v2.y ? v1.x : v2.x;
	edge->yMax = (int)(fmaxf(v1.y, v2.y) + 0.5);
	edge->dx = -(v2.x - v1.x) / (v2.y - v1.y);
	edge->dy = (int)(v1.y + 0.5) - (int)(v2.y + 0.5);
	edge->polygonID = polygonID;
	
}

void Objects::ResizeVertices()
{
	float xDis = xMax - xMin;
	float yDis = yMax - yMin;
	float xScale = (float)(xMax - xMin) / winWidth;
	float yScale = (float)(yMax - yMin) / winHeight;
	float xCenter = (float)(xMax + xMin) / 2;
	float yCenter = (float)(yMax + yMin) / 2;

	float Scale = xScale > yScale ? xScale : yScale;

	float xMovement = xMin <= 0 ? 1.2 * abs(xMin) : 0.05*xMin;
	float yMovement = yMin <= 0 ? 1.2 * abs(yMin) : 0.05*yMin;

	if (Scale <= 0.01)
		Scale = winHeight / 1.5;
	else if (Scale < 0.5)
		Scale = (int)(1 / Scale);
	else if (Scale > 1)
		Scale = 1.0f /(int)(Scale + 0.5);
	else
		Scale = 1;
	
	yMax = INT_MIN;
	yMin = INT_MAX;
	xMax = INT_MIN;
	xMin = INT_MAX;
	verticesList.clear();
	if (Scale != 1 || xMovement!=0 || yMovement!=0) {
		for (auto v : originVerticesList) {
			Vector3D tmpV(v.x + xMovement, v.y + yMovement, v.z);
			tmpV = tmpV*Scale;
			yMin = yMin > tmpV.y ? tmpV.y : yMin;
			yMax = yMax < tmpV.y ? tmpV.y : yMax;
			xMin = xMin > tmpV.x ? tmpV.x : xMin;
			xMax = xMax < tmpV.x ? tmpV.x : xMax;
			verticesList.push_back(tmpV);
		}
	}

}

void Objects::CalculateFaceParam(vector<GLfloat>& paramList, Vector3D& color, int faceIndex)
{
	//caculate plane parameters
	paramList.resize(4);
	int vIndex1 = faceIndexList[faceIndex][0];
	int vIndex2 = faceIndexList[faceIndex][1];
	int vIndex3 = faceIndexList[faceIndex][2];
	Vector3D vec1 = verticesList[vIndex3] - verticesList[vIndex2];
	Vector3D vec2 = verticesList[vIndex1] - verticesList[vIndex2];
	paramList[0] = vec1.y*vec2.z - vec1.z*vec2.y;
	paramList[1] = vec1.z*vec2.x - vec1.x*vec2.z;
	paramList[2] = vec1.x*vec2.y - vec1.y*vec2.x;

	if (!isBackList.empty() && (paramList[2]) < 0)
		isBackList[faceIndex] = true;
	
	GLfloat param_abssum = abs(paramList[0]) + abs(paramList[1]) + abs(paramList[2]);
	if (param_abssum == 0) {
		cerr << "normal vector is 0." << endl;
		param_abssum = 1;
	}
	paramList[0] /= param_abssum;
	paramList[1] /= param_abssum;
	paramList[2] /= param_abssum;
	paramList[3] = - paramList[0] * verticesList[vIndex1].x 
				   - paramList[1] * verticesList[vIndex1].y 
				   - paramList[2] * verticesList[vIndex1].z;


	//calculate color parameter
	GLfloat p0 = abs(paramList[0]);
	GLfloat p1 = abs(paramList[1]);
	GLfloat p2 = abs(paramList[2]);
	GLfloat cosTheta = p2 / sqrt(p0 + p1 + p2);

	color.x = cosTheta;
	color.y = cosTheta;
	color.z = cosTheta;

	colorList[faceIndex] = color;
}

void Objects::CalculateFaceMaxYZ(int& maxY, GLfloat& maxZ, int faceIndex)
{
	int vIndex1 = faceIndexList[faceIndex][0];
	int vIndex2 = faceIndexList[faceIndex][1];
	int vIndex3 = faceIndexList[faceIndex][2];
	float tmpMaxY = fmaxf(verticesList[vIndex1].y, verticesList[vIndex2].y);
	tmpMaxY = fmaxf(verticesList[vIndex3].y, tmpMaxY);

	maxZ = fmaxf(verticesList[vIndex1].z, verticesList[vIndex2].z);
	maxZ  = fmaxf(verticesList[vIndex3].z, maxZ);

	maxY = (int)(tmpMaxY + 0.5);
}

void Objects::CalculateFaceDeltaY(int& dy, int faceIndex)
{
	int vIndex1 = faceIndexList[faceIndex][0];
	int vIndex2 = faceIndexList[faceIndex][1];
	int vIndex3 = faceIndexList[faceIndex][2];

	float tmpMaxY = fmaxf(verticesList[vIndex1].y, verticesList[vIndex2].y);
	tmpMaxY = fmaxf(verticesList[vIndex3].y, tmpMaxY);

	float tmpMinY = fminf(verticesList[vIndex1].y, verticesList[vIndex2].y);
	tmpMinY = fminf(verticesList[vIndex3].y, tmpMinY);

	dy = (int)(tmpMaxY + 0.5) - (int)(tmpMinY + 0.5);
}

void Objects::CalculateFaceEdge(vector<ClassifiedEdge*>& edges, int faceIndex)
{
	GLfloat xMin;
	int vTopIndex, vMidIndex, vBottomIndex;
	int vIndex1, vIndex2, vIndex3;
	ClassifiedEdge *edg1 = new ClassifiedEdge;
	ClassifiedEdge *edg2 = new ClassifiedEdge;
	ClassifiedEdge *edg3 = new ClassifiedEdge;

	vIndex1 = faceIndexList[faceIndex][0];
	vIndex2 = faceIndexList[faceIndex][1];
	vIndex3 = faceIndexList[faceIndex][2];

	if (verticesList[vIndex1].y > verticesList[vIndex2].y&&verticesList[vIndex1].y > verticesList[vIndex3].y) {
		vTopIndex = vIndex1;
		if (verticesList[vIndex2].y > verticesList[vIndex3].y) {
			vMidIndex = vIndex2;
			vBottomIndex = vIndex3;
		}
		else {
			vMidIndex = vIndex3;
			vBottomIndex = vIndex2;
		}
	}
	else if (verticesList[vIndex2].y > verticesList[vIndex1].y&&verticesList[vIndex2].y > verticesList[vIndex3].y) {
		vTopIndex = vIndex2;
		if (verticesList[vIndex1].y > verticesList[vIndex3].y) {
			vMidIndex = vIndex1;
			vBottomIndex = vIndex3;
		}
		else {
			vMidIndex = vIndex3;
			vBottomIndex = vIndex1;
		}
	}
	else{
		vTopIndex = vIndex3;
		if (verticesList[vIndex1].y > verticesList[vIndex2].y) {
			vMidIndex = vIndex1;
			vBottomIndex = vIndex2;
		}
		else {
			vMidIndex = vIndex2;
			vBottomIndex = vIndex1;
		}
	}


	//cout << "Top: " << verticesList[vTopIndex].x << ' ' << verticesList[vTopIndex].y << endl;
	//cout << "Mid: " << verticesList[vMidIndex].x << ' ' << verticesList[vMidIndex].y << endl;
	//cout << "Bottom: " << verticesList[vBottomIndex].x << ' ' << verticesList[vBottomIndex].y << endl;


	//vTop & vMid
	CalculateEdgeParam(vTopIndex, vMidIndex, edg1, faceIndex);

	//vTop & vBottom
	CalculateEdgeParam(vTopIndex, vBottomIndex, edg2, faceIndex);

	//vMid & vBottom
	CalculateEdgeParam(vMidIndex, vBottomIndex, edg3, faceIndex);

	edges.push_back(edg1);
	edges.push_back(edg2);
	edges.push_back(edg3);

}

void Objects::Rotate(int key)
{
	GLfloat tmp;
	yMin = INT_MAX;
	yMax = INT_MIN;
	xMin = INT_MAX;
	xMax = INT_MIN;
	for (auto &v : originVerticesList) {
		switch (key)
		{
		case GLUT_KEY_RIGHT:
			tmp = v.x;
			v.x = v.z;
			v.z = -tmp;
			break;
		case GLUT_KEY_LEFT:
			tmp = v.x;
			v.x = -v.z;
			v.z = tmp;
			break;
		default:
			break;
		}
		yMin = yMin > v.y ? v.y : yMin;
		yMax = yMax < v.y ? v.y : yMax;
		xMin = xMin > v.x ? v.x : xMin;
		xMax = xMax < v.x ? v.x : xMax;
	}

	ResizeVertices();
	if (accelerate) {
		isBackList.clear();
		isBackList.resize(faceIndexList.size(), false);
	}
	colorList.resize(faceIndexList.size());

}

