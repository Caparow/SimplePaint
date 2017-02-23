#include "Defines.h"

#include <windows.h>
#include <glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>



class paint
{
public:
	//Constructor (set up default mode)
	paint();    
	//Procedure to proccess mouses clicks
	void mouseClickListener(pixelPoint point);
	//Procedure that redraw screen with def. color
	void redraw();
	//Auto rechape screen and change screen* and pixelsNum* variables 
	void reshape(int w, int h);
	//Procedure to process keyboard input
	void keyPressListener(unsigned char key, int x, int y);
	//Procedure to initialize screen
	void initialize();
	//Procedure that process main menu
	void mainMenuClickListener(int value);
	//Procedure to process colors submenu
	void colorsMenuClickListener(int value);

private:
	
	//Setting pixel by pixelPoint coordinates
	void setPixel(const pixelPoint& point, const RGBColor& color);
	//Changing pixel size
	void changePixelSize(const char ch);
	//Compare two RGBColor variables
	bool colorsAreEqual(RGBColor color1, RGBColor color2);
	//Getting RGBColor color of the pixel by pixelPoint coordinates
	RGBColor getPixelColor(const pixelPoint& point);

	//Drawing line
	void drawLine(pixelPoint start, pixelPoint end);
	//Drawing circle
	void drawCircle(pixelPoint center, int radius);
	//Drawing rectangle
	void drawRectangle(pixelPoint start, pixelPoint end);
	//Flood fill closed area that contains *point* coordinates by *fillc* RGBColor
	void floodFill(pixelPoint point, RGBColor fillc);
	

	//Modes of the painting
	enum
	{
		POINT_MODE = 0,
		PEN_MODE,
		LINE_MODE,
		CIRCLE_MODE,
		RECTANGLE_MODE,
		FLOODFILL_MODE,
	};

	int currentMode;

	//Mouse start location for those primitives that take two points.
	bool startDrawing;
	pixelPoint start;

	// Number of pixels in the x and y directions
	// Automatically updated if the user resizes the window
	int pixelsNumX;
	int pixelsNumY;

	// This is the color currently being used to draw to the screen.
	RGBColor currentDrawColor;
};

//Mouse holding button variable
extern bool mouseIsPressed;
//Current pixel size variable
extern int pixelSize;
//Sticking mode variable(paint with mouse hold button)
extern bool stickingMode;
//make a fake instance for th callback functions
static paint *instance = NULL;