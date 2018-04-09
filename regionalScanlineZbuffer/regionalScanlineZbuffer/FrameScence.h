#pragma once
#include "CommonFunc.h"
#include "Objects.h"
#include "ClassifiedPolygon.h"
#include "ClassifiedEdge.h"
#include "ActivePolygon.h"
#include "ActiveEdge.h"

class FrameScence {
public:
	FrameScence();
	void InitialFrameScence(int _width, int _height, string &_objName, bool _acc = false);
	void ResizeFrameScence(int _width, int _height);
	int GetWidth() { return width; }
	int GetHeight() { return height; }
	auto GetFrameBufferAdd() { return &frameBuffer[0]; }
	void ScanLineZbuffer();
	void SetRotate(int key);

private:
	int width;
	int height;
	Objects obj;
	vector<ClassifiedPolygon*> CPT;
	vector<ClassifiedEdge*> CET;
	ActivePolygon* APT;
	ActiveEdge* AET;
	vector<GLfloat> frameBuffer;

	static bool cmp(const ActivePolygon* a, const ActivePolygon* b);
	void BuildCPT();
	void BuildCET();
	void BuildAPT(int y);
	void BuildAET(int y);
	void FillZBuffer(int y);

	void ClearData();
	void ClearCPTCET();
	void ClearAPT();
	void ClearAET();
};