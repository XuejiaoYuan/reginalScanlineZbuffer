#include "CommonFunc.h"
#include "Vector3D.h"
#include "FrameScence.h"

FrameScence fs;

static void display() {

	glClear(GL_COLOR_BUFFER_BIT);

	fs.ScanLineZbuffer();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glDrawPixels(fs.GetWidth(), fs.GetHeight(), GL_RGB, GL_FLOAT, fs.GetFrameBufferAdd());

	glutSwapBuffers();

}

static void Reshape(int width, int height)
{
	display();
}

static void SpecialKeys(int key, int x, int y)
{
	fs.SetRotate(key);
	display();
}

void InitialFrame()
{
	cout << "=======================================" << endl;
	cout << "======= Region Scanline Zbuffer =======" << endl;
	cout << "=======================================" << endl;
	cout << "输入Model目录下的文件名(如 deer.obj)：" << endl;

	string objPath = "Models/";
	string objName;
	bool acc = false;
	cin >> objName;
	objPath += objName;

	cout << "是否加速[Y/N]" << endl;
	cin >> objName;
	if (objName == "Y" || objName == "y")
		acc = true;

	fs.InitialFrameScence(WINWIDTH, WINHEIGHT, objPath, acc);

}

int main(int argc, char* argv[])
{
	InitialFrame();

	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(WINWIDTH, WINHEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Region Scanline Zbuffer");

	glutDisplayFunc(display);
	glutReshapeFunc(Reshape);
	glutIdleFunc(display);
	glutSpecialFunc(SpecialKeys);
	glClearColor(0, 0, 0, 0);
	glutMainLoop();

	return 0;
}