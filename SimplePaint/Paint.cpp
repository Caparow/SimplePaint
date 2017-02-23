#include "Paint.h"

using namespace std;

bool mouseIsPressed = false;
bool stickingMode = false;
int pixelSize = 2;

void paint::changePixelSize(char ch)
{
	if (ch == '=')
		pixelSize = (pixelSize == MAX_PIXEL_SIZE) ? 1 : pixelSize + 1;
	else
		pixelSize = (pixelSize == MIN_PIXEL_SIZE) ? 10 : pixelSize - 1;
}

void paint::setPixel(const pixelPoint& point, const RGBColor& color)
{
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_POINTS);
	glVertex2i(point.x * pixelSize + pixelSize / 2, point.y * pixelSize + pixelSize / 2);
	glEnd();
	glFlush();
}

RGBColor paint::getPixelColor(const pixelPoint& point)
{
	RGBColor newColor;
	glReadPixels(
		point.x * pixelSize + pixelSize / 2,
		point.y * pixelSize + pixelSize / 2, 1, 1,
		GL_RGB, GL_FLOAT, ((float*)(&newColor))
	);
	return newColor;
}

bool paint::colorsAreEqual(RGBColor color1, RGBColor color2)
{
	return (color1.r == color2.r && color1.b == color2.b && color1.g == color2.g);
}

void paint::drawLine(pixelPoint start, pixelPoint end)
{
	int dx = abs(end.x - start.x); 
	int dy = -abs(end.y - start.y); 
	int sx = start.x < end.x ? 1 : -1;
	int sy = start.y < end.y ? 1 : -1; //change by start and end positions
	int err = dx + dy, e2; //Error value e_xy

	do   //Loop for drawing 
	{
		setPixel(start, currentDrawColor);
		e2 = 2 * err;
		if (e2 >= dy)
		{
			err += dy;
			start.x += sx;
		}
		if (e2 <= dx)
		{
			err += dx;
			start.y += sy;
		}
	} while (start.x != end.x && start.y != end.y);
	glFlush();
	cerr << "line drawn." << endl;
}

void paint::drawRectangle(pixelPoint start, pixelPoint end)
{
	//Find rectangle postion and draw
	if (end.x < start.x)
		if (end.y < start.y)
		{
			drawLine(end, pixelPoint(end.x, start.y));
			drawLine(pixelPoint(end.x, start.y), start);
			drawLine(start, pixelPoint(start.x, end.y));
			drawLine(pixelPoint(start.x, end.y), end);
		}
		else
		{
			drawLine(pixelPoint(end.x, start.y), start);
			drawLine(start, pixelPoint(start.x, end.y));
			drawLine(pixelPoint(start.x, end.y), end);
			drawLine(end, pixelPoint(end.x, start.y));
		}
	else
		if (end.y < start.y)
		{
			drawLine(pixelPoint(start.x, end.y), end);
			drawLine(end, pixelPoint(end.x, start.y));
			drawLine(pixelPoint(end.x, start.y), start);
			drawLine(start, pixelPoint(start.x, end.y));
		}
		else
		{
			drawLine(start, pixelPoint(start.x, end.y));
			drawLine(pixelPoint(start.x, end.y), end);
			drawLine(end, pixelPoint(end.x, start.y));
			drawLine(pixelPoint(end.x, start.y), start);
		}
	glFlush();
	cerr << "rectangle drawn." << endl;
}

void paint::floodFill(pixelPoint point, RGBColor fillc)
{
	//Vector for pixels holding
	vector<pixelPoint> points;
	points.push_back(point);
	pixelPoint p;
	RGBColor oldColor = getPixelColor(point);

	//While vector not empty
	while (points.begin() != points.end())
	{
		p = points.back();
		points.pop_back();
		RGBColor cur = getPixelColor(p);
		//If collor of curent pixel equal color of the starting pixel
		if (colorsAreEqual(cur, oldColor))
		{
			setPixel(p, fillc);
			//Adding new points to the vector
			points.push_back(pixelPoint(p.x + 1, p.y));
			points.push_back(pixelPoint(p.x - 1, p.y));
			points.push_back(pixelPoint(p.x, p.y + 1));
			points.push_back(pixelPoint(p.x, p.y - 1));
		}
	}
}

void paint::drawCircle(pixelPoint center, int radius)
{
	int f = 1 - radius;
	int x = 0;
	int y = radius;

	//Bresenthem Midpoint Circle algorithm
	setPixel(pixelPoint(center.x + x, center.y + y), currentDrawColor);
	setPixel(pixelPoint(center.x + x, center.y - y), currentDrawColor);
	setPixel(pixelPoint(center.x - x, center.y + y), currentDrawColor);
	setPixel(pixelPoint(center.x - x, center.y - y), currentDrawColor);
	setPixel(pixelPoint(center.x + y, center.y + x), currentDrawColor);
	setPixel(pixelPoint(center.x - y, center.y + x), currentDrawColor);
	setPixel(pixelPoint(center.x + y, center.y - x), currentDrawColor);
	setPixel(pixelPoint(center.x - y, center.y - x), currentDrawColor);

	while (x <= y)
	{
		x++;
		if (f<0)
		{
			f += 2 * x + 1;
		}
		else
		{
			f += 2 * (x - y) + 1;
			y--;
		}
		setPixel(pixelPoint(center.x + x, center.y + y), currentDrawColor);
		setPixel(pixelPoint(center.x + x, center.y - y), currentDrawColor);
		setPixel(pixelPoint(center.x - x, center.y + y), currentDrawColor);
		setPixel(pixelPoint(center.x - x, center.y - y), currentDrawColor);
		setPixel(pixelPoint(center.x + y, center.y + x), currentDrawColor);
		setPixel(pixelPoint(center.x - y, center.y + x), currentDrawColor);
		setPixel(pixelPoint(center.x + y, center.y - x), currentDrawColor);
		setPixel(pixelPoint(center.x - y, center.y - x), currentDrawColor);
	}
	glFlush();
	cerr << "circle drawn." << endl;
}

void paint::mouseClickListener(pixelPoint point)
{
	if (currentMode == PEN_MODE && mouseIsPressed)
		setPixel(point, currentDrawColor);
	else if (!startDrawing && !mouseIsPressed)
		switch (currentMode)
		{
		case POINT_MODE:
			setPixel(point, currentDrawColor);
			cerr << "pixel drawn." << endl;
			break;
		case FLOODFILL_MODE:
			floodFill(point, currentDrawColor);
			cerr << "Floodfill done." << endl;
			break;
		default:
			start.x = point.x;
			start.y = point.y;
			startDrawing = true;
			break;			
		}
	else if (!mouseIsPressed | stickingMode)
		switch (currentMode)
		{
		case LINE_MODE:
			drawLine(start, point);
			startDrawing = false;
			break; 
		case CIRCLE_MODE:
		{
			int radius = sqrt((point.x - start.x) * (point.x - start.x) +
							  (point.y - start.y) * (point.y - start.y));
			drawCircle(start, radius);
			startDrawing = false;
			break;
		}
		case RECTANGLE_MODE:
			drawRectangle(start, point);
			startDrawing = false;
			break;
		}	
}

void paint::initialize()
{
	glClearColor(1., 1., 1., 1.);
	glColor3f(currentDrawColor.r, currentDrawColor.g, currentDrawColor.b);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0., (GLdouble)screenWidth, 0., (GLdouble)screenHeight);
	glPointSize((GLfloat)pixelSize);
}

void paint::redraw(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glFinish();
	startDrawing = false;
}

void paint::reshape(int w, int h)
{
	cerr << "Screen resized to " << w << " " << h << endl;
	screenWidth = w;
	screenHeight = h;
	glViewport(0, 0, screenWidth, screenHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0., (GLdouble)screenWidth, 0., (GLdouble)screenHeight);
	glClear(GL_COLOR_BUFFER_BIT);
	glFinish();
	startDrawing = false;
	pixelsNumX = screenWidth / pixelSize;
	pixelsNumY = screenHeight / pixelSize;
}

void paint::keyPressListener(unsigned char key, int x, int y)
{
	if (key >= '0' && key <= '9')
	{
		int color = key - '0';
		currentDrawColor = colors[color];
		cerr << "Color changed." << endl;
		return;
	}
	key = tolower(key);
	startDrawing = false;
	switch (key)
	{
	case 'p':
		cerr << "Point mode\n";
		currentMode = POINT_MODE;
		break;	
	case 'o':
		cerr << "Pen mode\n";
		currentMode = PEN_MODE;
		break;
	case 'l':
		cerr << "Line mode\n";
		currentMode = LINE_MODE;
		break;	
	case 'c':
		cerr << "Circle mode\n";
		currentMode = CIRCLE_MODE;
		break;	
	case 'r':
		cerr << "Rectangle mode\n";
		currentMode = RECTANGLE_MODE;
		break;	
	case 'f':
		cerr << "Floodfill mode\n";
		currentMode = FLOODFILL_MODE;
		break;	
	case '=': case '-':
		changePixelSize(key);
		glPointSize((GLfloat)pixelSize);
		pixelsNumX = screenWidth / pixelSize;
		pixelsNumY = screenHeight / pixelSize;
		cerr << "Pixel size now: " << pixelSize << endl;
		break;	
	case 'x':
		glClear(GL_COLOR_BUFFER_BIT);
		cerr << "Screen cleared." << endl;
		glFinish();
		break;	
	case 'q':
		exit(1);
		break;
	}
}

void paint::mainMenuClickListener(int value)
{
	startDrawing = false;
	switch (value)
	{
	case 1:
		cerr << "Point mode\n";
		currentMode = POINT_MODE;
		break;
	case 2:
		cerr << "Pen mode\n";
		currentMode = PEN_MODE;
		break;
	case 3:
		cerr << "Line mode\n";
		currentMode = LINE_MODE;
		break;
	case 4:
		cerr << "Circle mode\n";
		currentMode = CIRCLE_MODE;
		break;
	case 5:
		cerr << "Rectangle mode\n";
		currentMode = RECTANGLE_MODE;
		break;
	case 6:
		cerr << "Floodfill mode\n";
		currentMode = FLOODFILL_MODE;
		break;
	case 8:
		changePixelSize('-');
		glPointSize((GLfloat)pixelSize);
		pixelsNumX = screenWidth / pixelSize;
		pixelsNumY = screenHeight / pixelSize;
		cerr << "Pixel size now: " << pixelSize << endl;
		break;
	case 9:
		changePixelSize('=');
		glPointSize((GLfloat)pixelSize);
		pixelsNumX = screenWidth / pixelSize;
		pixelsNumY = screenHeight / pixelSize;
		cerr << "Pixel size now: " << pixelSize << endl;
		break;
	case 10:
		glClear(GL_COLOR_BUFFER_BIT);
		cerr << "Screen cleared." << endl;
		glFinish();
		break;
	case 11:
		stickingMode = !stickingMode;
		if (stickingMode)
			glutChangeToMenuEntry(11, "Off Sticking Mode", 11);
		else
			glutChangeToMenuEntry(11, "On Sticking Mode", 11);
		cerr << "Sticking mode now: " << stickingMode << endl;
		break;
	case 12:
		exit(1);
		break;
	}

}

void paint::colorsMenuClickListener(int value)
{
	currentDrawColor = colors[value];
	cerr << "Color changed.\n";
}

paint::paint()
{
	currentMode = POINT_MODE;
	start.x = 0;
	start.y = 0;
	currentDrawColor = BLACK;
}