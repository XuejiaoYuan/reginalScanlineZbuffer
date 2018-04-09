#include "FrameScence.h"

FrameScence::FrameScence()
{
}

void FrameScence::InitialFrameScence(int _width, int _height, string & _objName, bool _acc)
{
	obj.InitializeObj(_objName, _width, _height, _acc);
	width = obj.xMax + 2;
	height = obj.yMax + 2;

	CPT.clear();
	CPT.resize(height, NULL);
	CET.clear();
	CET.resize(height, NULL);
	APT = NULL;
	AET = NULL;

	frameBuffer.clear();
	frameBuffer.resize(3 * width*height, 0);

}

void FrameScence::ResizeFrameScence(int _width, int _height)
{
	CPT.clear();
	CPT.resize(height, NULL);
	CET.clear();
	CET.resize(height, NULL);
	APT = NULL;
	AET = NULL;

	frameBuffer.clear();
	frameBuffer.resize(3 * width*height, 0);

}

void FrameScence::ScanLineZbuffer()
{
	time_t t_start = clock();

	BuildCPT();
	BuildCET();

	//scanline from ymax to ymin
	for (int y = height - 1; y >= obj.yMin; y--) {
		BuildAPT(y);
		BuildAET(y);
		FillZBuffer(y);
	}

	ClearData();

	time_t cur_end = clock();

	GLfloat deltaT = (cur_end - t_start + 0.0) / CLOCKS_PER_SEC;
	system("cls");
	cout << "顶点数目: " << obj.verticesList.size() << endl;
	cout << "面片数目：" << obj.faceIndexList.size() << endl;
	cout << "区域扫描线算法时间： " << deltaT << endl;

}

void FrameScence::SetRotate(int key)
{
	obj.Rotate(key);
	width = obj.xMax + 2;
	height = obj.yMax + 2;

	CPT.clear();
	CPT.resize(height, NULL);
	CET.clear();
	CET.resize(height, NULL);

	frameBuffer.clear();
	frameBuffer.resize(3 * width*height, 0);

}

bool FrameScence::cmp(const ActivePolygon * a, const ActivePolygon * b)
{
	return a->zMax > b->zMax;
}

void FrameScence::BuildCPT()
{
	for (int i = 0; i < obj.faceIndexList.size(); i++) {
		//calculate polygon's parameters
		int maxY;
		GLfloat maxZ;
		int dy;
		vector<GLfloat> paramList;
		Vector3D color;

		//Accelerate
		if (obj.isBackList.size()!=0 && obj.isBackList[i])
			continue;

		obj.CalculateFaceParam(paramList, color, i);
		obj.CalculateFaceMaxYZ(maxY, maxZ, i);
		obj.CalculateFaceDeltaY(dy, i);

		ClassifiedPolygon* cp = new ClassifiedPolygon(paramList, i, maxZ, color, dy);

		//Insert polygon
		if (CPT[maxY] == NULL)
			CPT[maxY] = cp;
		else {
			cp->nxtPolygon = CPT[maxY];
			CPT[maxY] = cp;
		}
	}
}

void FrameScence::BuildCET()
{
	for (int fIndex = 0; fIndex < obj.faceIndexList.size(); fIndex++) {
		//Accelerate
		if (obj.isBackList.size()!=0 && obj.isBackList[fIndex])
			continue;
		vector<ClassifiedEdge*> edges;
		obj.CalculateFaceEdge(edges, fIndex);
		for (auto edg : edges) {
			if (CET[edg->yMax] == NULL) {
				edg->nxtEdge = CET[edg->yMax];
				CET[edg->yMax] = edg;
			}
			else {
				edg->nxtEdge = CET[edg->yMax];
				CET[edg->yMax] = edg;
			}
		}
	}
}

void FrameScence::BuildAPT(int y)
{
	ActivePolygon* aPtr = NULL;
	ActivePolygon* aPre = NULL;
	//update APT (dy)
	if (APT != NULL) {
		aPtr = APT->nxtPolygon;
		aPre = APT;
		while (aPtr != NULL) {
			aPtr->resDy--;
			if (aPtr->resDy < 0) {
				ActivePolygon *tmp = aPtr;
				aPtr = aPtr->nxtPolygon;
				aPre->nxtPolygon = aPtr;
				delete tmp;
			}
			else {
				aPre = aPtr;
				aPtr = aPtr->nxtPolygon;
			}
		}
	}


	//add new polygon into APT
	ClassifiedPolygon* cPtr = CPT[y];
	while (cPtr != NULL) {
		ActivePolygon *ap = new ActivePolygon(
			cPtr->paramList, cPtr->polygonID, cPtr->dy,  cPtr->color, cPtr->zMax
		);
		if (APT == NULL) {
			ActivePolygon *head = new ActivePolygon();
			head->nxtPolygon = ap;
			APT = head;
		}
		else {
			ap->nxtPolygon = APT->nxtPolygon;
			APT->nxtPolygon = ap;
		}
		cPtr = cPtr->nxtPolygon;
	}

}

void FrameScence::BuildAET(int y)
{
	ActiveEdge* tmp = NULL;
	ActiveEdge* pre = NULL;
	ActiveEdge* ptr = NULL;
	//update AET
	if (AET != NULL) {
		tmp = AET->nxtEdge;
		pre = AET;
		while (tmp != NULL) {
			tmp->resDy--;
			if (tmp->resDy <= 0) {
				ActiveEdge* del = tmp;
				tmp = tmp->nxtEdge;
				pre->nxtEdge = tmp;
				delete del;
			}
			else {
				tmp->x += tmp->dx;
				if (pre == AET) {
					pre = tmp;
					tmp = tmp->nxtEdge;
				}
				else {
					if (tmp->x < pre->x) {
						ptr = AET;
						for (; ptr->nxtEdge->x < tmp->x ||
							(ptr->nxtEdge->x == tmp->x && ptr->nxtEdge->dx < tmp->dx);
							ptr = ptr->nxtEdge);
						pre->nxtEdge = tmp->nxtEdge;
						tmp->nxtEdge = ptr->nxtEdge;
						ptr->nxtEdge = tmp;
						tmp = pre->nxtEdge;
					}
					else {
						pre = tmp;
						tmp = tmp->nxtEdge;
					}
				}
			}
		}
	}


	//insert new edges
	ClassifiedEdge *cPtr = CET[y];
	while (cPtr != NULL) {
		if (cPtr->dy == 0) {		//ignore parallel lines
			cPtr = cPtr->nxtEdge;
			continue;
		}
		ActiveEdge *ae = new ActiveEdge(
			cPtr->x, cPtr->dx, cPtr->dy, cPtr->polygonID, NULL
		);
		if (AET == NULL) {
			ActiveEdge* head = new ActiveEdge();
			head->nxtEdge = ae;
			AET = head;
		}
		else {
			tmp = AET->nxtEdge;
			pre = AET;
			for (; tmp != NULL && tmp->x < ae->x; pre = tmp, tmp = tmp->nxtEdge);
			if (tmp == NULL) {
				pre->nxtEdge = ae;
			}
			else if (tmp->x == ae->x) {
				for (; tmp != NULL && tmp->x == ae->x && tmp->dx < ae->dx; pre = tmp, tmp = tmp->nxtEdge);
				if (tmp == NULL)
					pre->nxtEdge = ae;
				else {
					ae->nxtEdge = tmp;
					pre->nxtEdge = ae;
				}
			}
			else {
				ae->nxtEdge = tmp;
				pre->nxtEdge = ae;
			}
		}

		cPtr = cPtr->nxtEdge;
	}
}

void FrameScence::FillZBuffer(int y)
{
	if (AET == NULL || AET->nxtEdge==NULL)
		return;

	vector<ActivePolygon*>::iterator iter;
	ActivePolygon* aPtr = NULL;
	ActiveEdge* leftEdg = AET->nxtEdge;
	vector<ActivePolygon*> inPolygon;
	int xl = 0;
	int xr = 0;
	while (leftEdg->nxtEdge != NULL) {
		xl = leftEdg->x + 0.5;
		xr = leftEdg->nxtEdge->x + 0.5;
		//insert inner polygons
		aPtr = APT->nxtPolygon;
		while (aPtr != NULL) {
			if (aPtr->polygonID == leftEdg->polygonID) {
				if (aPtr->flag == false) {
					aPtr->flag = !aPtr->flag;
					inPolygon.push_back(aPtr);
				}
				else {
					aPtr->flag = !aPtr->flag;
					iter = find(inPolygon.begin(), inPolygon.end(), aPtr);
					inPolygon.erase(iter);
				}
				break;
			}
			aPtr = aPtr->nxtPolygon;
		}

		if (!inPolygon.empty()) {
			std::sort(inPolygon.begin(), inPolygon.end(), cmp);
			for (int i = xl - 1; i < xr; i++) {
				frameBuffer[3 * (y*width + i)] = inPolygon[0]->color.x;
				frameBuffer[3 * (y*width + i) + 1] = inPolygon[0]->color.y;
				frameBuffer[3 * (y*width + i) + 2] = inPolygon[0]->color.z;
			}
		}

		leftEdg = leftEdg->nxtEdge;
	}

	if (!inPolygon.empty()) {
		for (auto p : inPolygon)
			p->flag = false;
	}
}

void FrameScence::ClearData()
{
	ClearCPTCET();
	ClearAPT();
	ClearAET();
}

void FrameScence::ClearCPTCET()
{
	ClassifiedPolygon* cp1 = NULL;
	ClassifiedPolygon* cp2 = NULL;
	ClassifiedEdge* ce1 = NULL;
	ClassifiedEdge* ce2 = NULL;

	for (int y = 0; y < height; y++) {
		cp1 = CPT[y];
		while (cp1 != NULL) {
			cp2 = cp1->nxtPolygon;
			delete cp1;
			cp1 = cp2;
		}
		CPT[y] = NULL;

		ce1 = CET[y];
		while (ce1 != NULL) {
			ce2 = ce1->nxtEdge;
			delete ce1;
			ce1 = ce2;
		}
		CET[y] = NULL;
	}
}

void FrameScence::ClearAPT()
{
	ActivePolygon* ap1 = APT;
	ActivePolygon* ap2 = NULL;
	while (ap1 != NULL) {
		ap2 = ap1->nxtPolygon;
		delete ap1;
		ap1 = ap2;
	}
	APT = NULL;
}

void FrameScence::ClearAET()
{
	ActiveEdge* ae1 = AET;
	ActiveEdge* ae2 = AET;
	while (ae1 != NULL) {
		ae2 = ae1->nxtEdge;
		delete ae1;
		ae1 = ae2;
	}
	AET = NULL;
}


