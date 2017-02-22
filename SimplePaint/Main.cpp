#include "Paint.h"

void keyDownCallback(unsigned char key, int x, int y)
{
	instance->keyPressListener(key, x, y);
}

void userChoiceCallBack(int value)
{
	instance->mainMenuClickListener(value);
}

void colorsMenuCallback(int value)
{
	instance->colorsMenuClickListener(value);
}

void redrawCallback()
{
	instance->redraw();
}

void reshapeCallback(int w, int h)
{
	instance->reshape(w, h);
}

void mouseClickCallback(int button, int state, int x, int y)
{
	pixelPoint point;
	point.x = x / pixelSize;
	point.y = ((screenHeight - 1) - y) / pixelSize;
	// Catch left mouse buttons and pass them to the user.
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		instance->mouseClickListener(point);
		mouseIsPressed = true;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		mouseIsPressed = false;
	}
}

void mouseMoveCallback(int x, int y)
{
	pixelPoint point;
	point.x = x / pixelSize;
	point.y = ((screenHeight - 1) - y) / pixelSize;
	instance->mouseClickListener(point);
}

int buildColorSelectorMenu()
{
	int menu = glutCreateMenu(colorsMenuCallback);
	glutAddMenuEntry("White", 0);
	glutAddMenuEntry("Black", 1);
	glutAddMenuEntry("Red", 2);
	glutAddMenuEntry("Green", 3);
	glutAddMenuEntry("Blue", 4);
	glutAddMenuEntry("Cyan", 5);
	glutAddMenuEntry("Yellow", 6);
	glutAddMenuEntry("Magenta", 7);
	glutAddMenuEntry("Orange", 8);
	glutAddMenuEntry("Purple", 9);
	return menu;
}

void buildMainMenu(int colorsMenu) 
{
	glutCreateMenu(userChoiceCallBack);
	glutAddMenuEntry("Point", 1);
	glutAddMenuEntry("Pen", 2);
	glutAddMenuEntry("Line", 3);
	glutAddMenuEntry("Circle", 4);
	glutAddMenuEntry("Rectangle", 5);
	glutAddMenuEntry("FloodFill", 6);
	glutAddSubMenu("Colors", colorsMenu);
	glutAddMenuEntry("- Pixel Size", 8);
	glutAddMenuEntry("+ Pixel Size", 9);
	glutAddMenuEntry("Clear Screen", 10);
	glutAddMenuEntry("On Sticking Mode", 11);
	glutAddMenuEntry("Exit", 12);
}

void createMenu(void)
{
	buildMainMenu(buildColorSelectorMenu());
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char** argv)
{
	instance = new paint();
	std::cerr << controlsMessage;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(windowName);

	glutDisplayFunc(redrawCallback);
	glutMouseFunc(mouseClickCallback);
	glutMotionFunc(mouseMoveCallback);
	glutKeyboardFunc(keyDownCallback);
	glutReshapeFunc(reshapeCallback);
	createMenu();
	instance->initialize();

	glutMainLoop();
	return 0;
}