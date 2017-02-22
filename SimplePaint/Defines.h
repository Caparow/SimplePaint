//Class that creates pixelPoint variable (contains cordinates of the pixel)
class pixelPoint
{
public:
	int x, y;

	pixelPoint() { x = y = 0; }

	pixelPoint(const int& x, const int& y)
	{
		this->x = x;
		this->y = y;
	}
};

//Class that creates color typed variable (contains RGB color variables)
class RGBColor
{
public:
	float r, g, b;

	// A default constructor, makes the color white
	RGBColor() { r = g = b = 1.0; }

	RGBColor(const float& r, const float& g, const float& b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}
};


//Default screen size
static int screenHeight = 480;
static int screenWidth = 640;

// GLOBAL constants:
static const char *controlsMessage = "mypaint Controls:\n"
"  Modes:\n"
"    'P' points \n"
"	 'O' pen \n"
"    'L' lines \n"
"    'C' circles \n"
"    'R' rectangles\n"
"    'F' floodfill\n"
"  \n"
"  Change color:\n"
"    [0-9]\n"
"  \n"
"  Other Controls:\n"
"    'X' clear screen\n"
"    '-' decrease brush size\n"
"	 '+' increase brush size"
"    'Q' quit\n\n";

// Setup simple colors
static const RGBColor WHITE(1.0, 1.0, 1.0);
static const RGBColor BLACK(0.0, 0.0, 0.0);
static const RGBColor RED(1.0, 0.0, 0.0);
static const RGBColor GREEN(0.0, 1.0, 0.0);
static const RGBColor BLUE(0.0, 0.0, 1.0);
static const RGBColor CYAN(0.0, 1.0, 1.0);
static const RGBColor YELLOW(1.0, 1.0, 0.0);
static const RGBColor MAGENTA(1.0, 0.0, 1.0);
static const RGBColor ORANGE(1.0, 0.5, 0.0);
static const RGBColor PURPLE(1.0, 0.0, 1.0);

//Colors array
static const RGBColor colors[10] =
{
	WHITE,BLACK,RED,GREEN,BLUE,
	CYAN,YELLOW,MAGENTA, ORANGE, PURPLE
};
