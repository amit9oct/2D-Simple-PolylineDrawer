/************************************************/
/* @Author:									*/
/* Amitayush Thakur 2012B4A7819P				*/
/************************************************/
#include<GL/glut.h>
#include<cstdio>
#include<cmath>
#include<vector>
#include<iostream>
#define LEFT 0
#define TOP 0
using namespace std;
//////////////////////////////// GLOBAL DECLARATIONS///////////////////////////////
int WORLD_HEIGHT = 700;
int WORLD_WIDTH = 700;
int WINDOW_HEIGHT = 700;
int WINDOW_WIDTH = 700;
int N = 13;						// NUMBER OF SIDES IN POLYGON,STAR.
int POLYGON_SIDE_SIZE = 3;		// LENGTH OF EACH SIDE OF POLYGON
int SPIRAL_SCALE = 1;
double POINT_SIZE	= 4.0;
double PI = 3.1415926535897932384626433832795;
double SPIRAL_ANGLE = PI/2 ;	// THE ANGLE OF ROTATION IN SPIRAL
double THETHA = PI / 15;		// ANGLE OF ROTATION ON PRESSING 't'
double SCALE_UP_FACTOR = 1.5;
double SCALE_DOWN_FACTOR = 0.5;
bool IS_MOUSE_CLICK = false;	// FLAG VARIABLE TO REDUCE THE REFRESHES

///////////////////////////////////////////////////////////////////////////////////

typedef enum clr{
	BLUE,
	GREEN,
	RED,
	YELLOW,
	BLACK
}Color;

///////////////////////////////////POINT CLASS//////////////////////////////////////
/*
THIS CLASS ENCPSULATES THE POINT ON THE SCREEN ALONG WITH THEIR COLOR. IT IS INSTANTIATED
ON MOUSE CLICK
*/
class Point {
public:
	float x;
	float y;
	Color color;
	Point() {
		this->x = 0.0;
		this->y = 0.0;
		this->color = BLACK;
	}
	Point(float x,float y) {
		this->x = x;
		this->y = y;
		this->color = BLACK;
	}
	Point(const Point &oldObj) {
		this->x = oldObj.x;
		this->y = oldObj.y;
		this->color = oldObj.color;
	}
	void update(float newX,float newY) {
		this->x = newX;
		this->y = newY;
	}
};

///////////////////////////////////LINE CLASS//////////////////////////////////////
/*
THIS CLASS ENCPSULATES THE LINE DRAWN ON THE SCREEN ALONG WITH IT'S COLOR.
IT IS INSTANTIATED WHEN 'f' IS PRESSED OR A POLYGON OR STAR IS DRAWN.
*/
class Line {
public:
	Point start;
	Point end;
	Color color;
	Line(Point start,Point end) {
		this->start = start;
		this->end = end;
	}
};

////////////////////////////////////////////////////////////////////////////////////

vector<Point> pointBuffer;			// BUFFER TO STORE THE POINTS, THE BUFFER IS USED TO REDRAW THE POINTS WHILE RESIZING THE WINDOW
vector<Line> lineBuffer;			// BUFFER TO STORE THE LINE, THE BUFFER IS USED TO REDRAW THE LINES WHILE RESIZING THE WINDOW

Point universalPoint = Point(0,5);	//INITIAL VALUE OF (u,v) ACCORDING TO THE QUESTION
Point curPoint = Point(0, 5);		//USED TO STORE LOCATION OF THE SECOND END-POINT AFTER A SEQ OF OPEARATIONS
Color COLOR = BLACK;				// DEFAULT COLOR IS BLACK


///////////////// USED BY POP-UP MENU TO CHANGE COLOR //////////////////////////////
void changeColor(Color color) {
	switch (color) {
	case BLUE:	glColor3f(0.0, 0.0, 1.0);
				break;
	case GREEN: glColor3f(0.0, 1.0, 0.0);
				break;
	case RED: glColor3f(1.0, 0.0, 0.0);
			  break;
	case BLACK: glColor3f(0.0, 0.0, 0.0);
				break;
	case YELLOW: glColor3f(1.0, 1.0, 0.0);
				break;
	default: glColor3f(0.0, 0.0, 0.0);
	}
}
/////////////////////////////////////////////////////////////////////////////////////

//////////////// USED FOR INITIALIZING GLUT /////////////////////////////////////////

void initializeGlut(void){
	WORLD_HEIGHT = WINDOW_HEIGHT = glutGet(GLUT_SCREEN_HEIGHT);
	WORLD_WIDTH = WINDOW_WIDTH = glutGet(GLUT_SCREEN_WIDTH);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(TOP, LEFT);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("PolylineDrawer");
	glClearColor(1.0, 1.0, 1.0, 0.0);	//Setting background color white
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, WORLD_WIDTH, 0.0, WORLD_HEIGHT);
}
///////////////////////////////////////////////////////////////////////////////////////



////////////////USED TO DISPLAY POINTS AFTER RESIZE////////////////////////////////////
void displayPoints() {
	int pointBuffsize = pointBuffer.size();
	glBegin(GL_POINTS);
	for (int i = 0; i < pointBuffsize; i++) {
		Point temp = pointBuffer[i];
		changeColor(temp.color);
		glVertex2f(temp.x, temp.y);
	}
	glEnd();
	glFlush();
}
///////////////////////////////////////////////////////////////////////////////////////


///////////////////////USED TO DISPLAY LINE AFTER RESIZING/////////////////////////////
void displayLines() {
	int lineBuffSize = lineBuffer.size();
	for (int i = 0; i < lineBuffSize; i++) {
		Line temp = lineBuffer[i];
		changeColor(temp.color);
		glBegin(GL_LINE_STRIP);
			glVertex2f(temp.start.x, temp.start.y);
			glVertex2f(temp.end.x, temp.end.y);
		glEnd();
		glFlush();
	}
}
///////////////////////////////////////////////////////////////////////////////////////

////////////////////////USED TO CLEAR SCREEN WHEN USER PRESSED 'c'/////////////////////
void clearScreen(void){
	glClear(GL_COLOR_BUFFER_BIT);
	pointBuffer.clear();
	lineBuffer.clear();
	COLOR = BLACK;
	curPoint.color = BLACK;
	universalPoint.update(0.0, 5.0);
	glFlush();
}

///////////////////////////////////////////////////////////////////////////////////////


////////////////////////////DISPLAY FUNCTION FOR glutDisplayFunc() ////////////////////
void render(){

	if (!IS_MOUSE_CLICK) { 			// IF THE EVENT IS NOT A MOUSE CLICK THEN REDRAW
		glClear(GL_COLOR_BUFFER_BIT);
		displayPoints();			// REDRAW POINTS
		displayLines();				// REDRAW LINES
	}
	IS_MOUSE_CLICK = false;
}

///////////////////////////////////////////////////////////////////////////////////////


//////////////////////FUNCTION CALLED AFTER MOUSE CLICK ///////////////////////////////
/*
IT DRAWS A POINT ON THE SCREEN WHERE THE LEFT MOUSE BUTTON IS CLICKED
*/
void createPoint(float x, float y) {
	float yMax = (float)glutGet(GLUT_WINDOW_HEIGHT);
	glPointSize(POINT_SIZE);
	changeColor(COLOR);
	curPoint.color = COLOR;
	curPoint.update(x, yMax - y);
	Point newPoint = Point(curPoint);
	pointBuffer.push_back(newPoint);
	glBegin(GL_POINTS);
		glVertex2f(x,yMax-y);
	glEnd();
	glFlush();
}

/////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////USED FOR DRAWING LINE////////////////////////////////////////
void Forward() {

	changeColor(COLOR);
	Point start = Point(curPoint);
	glBegin(GL_LINES);
		glVertex2f(curPoint.x,curPoint.y);
		glVertex2f(curPoint.x + universalPoint.x, curPoint.y + universalPoint.y);
	glEnd();
	glFlush();
	curPoint.update(curPoint.x + universalPoint.x, curPoint.y + universalPoint.y);
	Point end = Point(curPoint);
	Line line = Line(start, end);
	line.color = COLOR;
	lineBuffer.push_back(line);
}

/////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////USED FOR TURNING UNIVERSAL POINT/////////////////////////////
void Turn(double theta){
	float x = universalPoint.x;
	float y = universalPoint.y;
	float xNew = x*cos(theta) - y*sin(theta);
	float yNew = x*sin(theta) + y*cos(theta);
	universalPoint.update(xNew,yNew);
}
/////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////USED FOR SCALING UP//////////////////////////////////////////
void Scaleup() {
	float x = universalPoint.x;
	float y = universalPoint.y;
	float xNew = SCALE_UP_FACTOR*x;
	float yNew = SCALE_UP_FACTOR*y;
	universalPoint.update(xNew, yNew);
}
/////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////USED FOR SCALING DOWN////////////////////////////////////////

void Scaledown() {
	float x = universalPoint.x;
	float y = universalPoint.y;
	float xNew = SCALE_DOWN_FACTOR*x;
	float yNew = SCALE_DOWN_FACTOR*y;
	universalPoint.update(xNew, yNew);
}

/////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////USED FOR DRAWING POLYGON/////////////////////////////////////
void Polygon(int n) {
	int sides = 0;
	while (sides < n) {
		for (int j = 0; j < POLYGON_SIDE_SIZE; j++) {
			Forward();
		}
		Turn(2 * PI / n);
		sides++;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////USED FOR DRAWING STAR////////////////////////////////////////
void Star(int n) {
	int sides = 0;
	while (sides < n) {
		for (int j = 0; j < POLYGON_SIDE_SIZE; j++) {
			Forward();
		}
		Turn(4 * PI / n);
		sides++;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////USED FOR DRAWING SPIRAL//////////////////////////////////////
void Spiral(int n, double angle, int scaleFactor) {
	int sides = 0;
	while (sides < n) {
		Forward();
		Turn(angle);
		for (int j = 0; j < scaleFactor; j++) {
			Scaleup();
		}
		sides++;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////FUNCTIONPASSED IN glutMouseFunc()/////////////////////////////
void mouseEventHandler(int button,int state,int x,int y){
	universalPoint.update(0.0, 5.0);
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_UP) {
			createPoint((float)x,(float)y);			// DRAWS A POINT ON SCREEN
		}
	}
	IS_MOUSE_CLICK = true;
}

//////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////FUNCTIONPASSED IN glutKeyboardFunc()/////////////////////////////
void onKeyPress(unsigned char key,int x,int y){
	switch (key) {
		case 'f': Forward();
				  break;
		case 'c': clearScreen();
				  break;
		case 't': Turn(THETHA); // Here THETA = PI/15.0
				  break;
		case 'S': Scaleup();
				  break;
		case 's': Scaledown();
				  break;
		case 'p': Polygon(N);
				  break;
		case 'a': Star(N);
				  break;
		case 'r': Spiral(N, SPIRAL_ANGLE, SPIRAL_SCALE);
				  break;
		default: break;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////FUNCTIONPASSED IN glutReshapeFunc()//////////////////////////
void onReshape(int w,int h) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	WINDOW_HEIGHT = h;
	WINDOW_WIDTH = w;
	WORLD_HEIGHT = h;
	WORLD_WIDTH = w;
}
/////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////FUNCTION TO SET COLOR SELECTED FROM MENU ///////////////////
/*FUNCTION IS CALLED WHENEVER AN OPTION IS SELECTED FROM POP UP MENU */
void menu(int press) {
	switch (press) {
		case BLACK	: COLOR = BLACK;
				  break;
		case RED	: COLOR = RED;
				  break;
		case GREEN	: COLOR = GREEN;
			      break;
		case BLUE	: COLOR = BLUE;
				  break;
		case YELLOW	: COLOR = YELLOW;
				  break;
		default		: break;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////FUNCTION TO CREATE POP UP MENU///////////////////////////////
void createMenu() {
	glutCreateMenu(menu);
	glutAddMenuEntry("BLACK", BLACK);
	glutAddMenuEntry("RED", RED);
	glutAddMenuEntry("GREEN", GREEN);
	glutAddMenuEntry("BLUE", BLUE);
	glutAddMenuEntry("YELLOW", YELLOW);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

//////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
	//Initialize GLUT.
	glutInit(&argc, argv);
	initializeGlut( );
	createMenu();	// TO ADD THE POP UP MENU

	// REGISTERING FUNCTIONS FOR VARIOUS EVENTS
	glutDisplayFunc(render);
	glutReshapeFunc(onReshape);
	glutMouseFunc(mouseEventHandler);
	glutKeyboardFunc(onKeyPress);
	glutMainLoop ( );
	return 0;
}
