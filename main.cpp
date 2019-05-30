#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <glut.h>
#include <math.h>

int WIDTH = 1280;
int HEIGHT = 720;

GLuint tex;
char title[] = "Duck Hunter";

// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 100;
// Level and Views Variables
int level = 1;
bool thirdView = false;
bool firstView = false;
bool gameOn = true;
bool won = false;
// Animation Variables
// Duck 1
float duckTransX1 = 0;
float duckTransY1 = 0;
float duckTransZ1 = 0;
float duckJ1 = 0.0;
// Duck 2
float duckTransZ2 = 0;
float duckRot2 = 0.0;
// Duck 3
float duckTransZ3 = 0;
float duckTransY3 = 0;
float duckRot3 = 0;
bool incRot3 = true; 
// Duck 4
float duckTransZ4 = 0;
float duckTransY4 = 0;
// Duck 5
float duckTransX5 = 0;
float duckTransZ5 = 0;
// Duck 6
float duckTransX6 = 0;
float duckTransY6 = 1.5;
float duckTransZ6 = 0;
float duckRot6 = 0;
// Duck 7
float duckTransZ7 = 0;
bool back = false;
// Duck 8
float duckTransX8 = 0;
float duckTransZ8 = 0;
float duckRot8 = 0;
bool incRot8 = true;
// Hunter
float hunterTransX = 0.0;
float hunterTransZ = 0.0;
int touches = 0;
float hunterStoneZS = 0.0f;
// Stone
bool stoneDrawEnable = false;
bool  stoneThrowEnableS = false;
bool  stoneThrowEnableDR = false;
bool  stoneThrowEnableDL = false;
bool  stoneThrowEnableP = false;
float stoneTransZ = 0.0;
float stoneTransX = 0.0f;
// Duck Touches
bool duck_1, duck_2, duck_3, duck_4, duck_5, duck_6, duck_7, duck_8 , big_duck;
//Boss Duck
bool showBigDuck = false;
float duckTransXB = 0.0;
float duckTransZB = 0.0;
int duckKills = 0;
bool bossBack = false;
int bossKills = 0;
float bossScaleX = 4.0;
float bossScaleY = 4.0;
float bossScaleZ = 2.0;
// Eye Variables
float eyeZ = 30.0;
float eyeY = 5.0;
float eyeX = 0.0f;
// Handlers
void handleDuckTouches();
void movingLights();
//Light Variables
double lightRot = 0;
bool lightRotateSwitch = false;
bool spotLight = true;

class Vector
{
public:
	GLdouble x, y, z;
	Vector() {}
	Vector(GLdouble _x, GLdouble _y, GLdouble _z) : x(_x), y(_y), z(_z) {}
	//================================================================================================//
	// Operator Overloading; In C++ you can override the behavior of operators for you class objects. //
	// Here we are overloading the += operator to add a given value to all vector coordinates.        //
	//================================================================================================//
	void operator +=(float value)
	{
		x += value;
		y += value;
		z += value;
	}
};

Vector Eye(0, 5, 40);
Vector At(0, 0, 0);
Vector Up(0, 1, 0);

int cameraZoom = 0;

// Model Variables
Model_3DS model_house;
Model_3DS model_tree;
Model_3DS model_duck;
Model_3DS model_hunter;
Model_3DS model_stone;

// Textures
GLTexture tex_ground;

//=======================================================================
// Lighting Configuration Function
//=======================================================================
void InitLightSource()
{
	// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	// OpengL has 8 light sources
	glEnable(GL_LIGHT0);

	// Define Light source 0 ambient light
	GLfloat ambient[] = { 0.1f, 0.1f, 0.1, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	// Define Light source 0 diffuse light
	GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	// Define Light source 0 Specular light
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// Finally, define light source 0 position in World Space
	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

void movingLights() {
	GLfloat lmodel_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	GLfloat l0Diffuse[] = { 1.0f, 1.0f, 0.5f, 1.0f };
	GLfloat l0Ambient[] = { 0.1f, 0.0f, 0.0f, 1.0f };
	GLfloat l0Position[] = { 20.0f, 5.0f, 25.0f, spotLight };
	GLfloat l0Direction[] = { -1.0, 0.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l0Diffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, l0Ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, l0Position);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30.0);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 90.0);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, l0Direction);
}

void stationaryLights() {
	GLfloat l1Diffuse[] = { 0.0f, 1.0f, 0.0f, 1.0f };
	GLfloat l1Ambient[] = { 0.0f, 0.1f, 0.0f, 1.0f };
	GLfloat l1Position[] = { 0.0f, 10.0f, 20.0f, spotLight };
	GLfloat l1Direction[] = { 0.0, -1.0, 0.0 };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, l1Diffuse);
	glLightfv(GL_LIGHT1, GL_AMBIENT, l1Ambient);
	glLightfv(GL_LIGHT1, GL_POSITION, l1Position);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 90.0);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, l1Direction);

	GLfloat l2Diffuse[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	GLfloat l2Ambient[] = { 0.0f, 0.0f, 0.1f, 1.0f };
	GLfloat l2Position[] = { 40.0f, 10.0f, 20.0f, spotLight };
	GLfloat l2Direction[] = { 0.0, 0.0, -1.0 };
	glLightfv(GL_LIGHT2, GL_DIFFUSE, l2Diffuse);
	glLightfv(GL_LIGHT2, GL_AMBIENT, l2Ambient);
	glLightfv(GL_LIGHT2, GL_POSITION, l2Position);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 30.0);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 90.0);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, l2Direction);
}

//=======================================================================
// Material Configuration Function
//======================================================================
void InitMaterial()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

//=======================================================================
// OpengGL Configuration Function
//=======================================================================
void myInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(fovy, aspectRatio, zNear, zFar);
	//*******************************************************************************************//
	// fovy:			Angle between the bottom and top of the projectors, in degrees.			 //
	// aspectRatio:		Ratio of width to height of the clipping plane.							 //
	// zNear and zFar:	Specify the front and back clipping planes distances from camera.		 //
	//*******************************************************************************************//

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
	//*******************************************************************************************//
	// EYE (ex, ey, ez): defines the location of the camera.									 //
	// AT (ax, ay, az):	 denotes the direction where the camera is aiming at.					 //
	// UP (ux, uy, uz):  denotes the upward orientation of the camera.							 //
	//*******************************************************************************************//

	InitLightSource();

	InitMaterial();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
}

//=======================================================================
// Render Ground Function
//=======================================================================
void RenderGround()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-20, 0, -20);
	glTexCoord2f(5, 0);
	glVertex3f(20, 0, -20);
	glTexCoord2f(5, 5);
	glVertex3f(20, 0, 20);
	glTexCoord2f(0, 5);
	glVertex3f(-20, 0, 20);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

void resetAllVariables() {
	// Duck 1
	duckTransX1 = 0;
	duckTransY1 = 0;
	duckTransZ1 = 0;
	duckJ1 = 0.0;
	// Duck 2
	duckTransZ2 = 0;
	duckRot2 = 0.0;
	// Duck 3
	duckTransZ3 = 0;
	duckTransY3 = 0;
	duckRot3 = 0;
	incRot3 = true;
	// Duck 4
	duckTransZ4 = 0;
	duckTransY4 = 0;
	// Duck 5
	duckTransX5 = 0;
	duckTransZ5 = 0;
	// Duck 6
	duckTransX6 = 0;
	duckTransY6 = 1.5;
	duckTransZ6 = 0;
	duckRot6 = 0;
	// Duck 7
	duckTransZ7 = 0;
	back = false;
	// Duck 8
	duckTransX8 = 0;
	duckTransZ8 = 0;
	duckRot8 = 0;
	incRot8 = true;
	// Hunter 
	hunterTransX = 0.0;
	hunterTransZ = 0.0;
	touches = 0;
	hunterStoneZS = 0.0f;
	// Stone
	stoneDrawEnable = false;
	stoneThrowEnableS = false;
	stoneThrowEnableDR = false;
	stoneThrowEnableDL = false;
	stoneThrowEnableP = false;
	stoneTransZ = 0.0;
	stoneTransX = 0.0f;
	// Duck Touches
	duck_1 = false;
	duck_2 = false;
	duck_3 = false;
	duck_4 = false;
	duck_5 = false;
	duck_6 = false;
	duck_7 = false;
	duck_8 = false;
	//Boss Duck
	showBigDuck = false;
	duckTransXB = 0.0;
	duckTransZB = 0.0;
	duckKills = 0;
	bossBack = false;
	bossKills = 0;
	bossScaleX = 4.0;
	bossScaleY = 4.0;
	bossScaleZ = 2.0;
}

void drawDuck(int x, int y, int z) {
	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(0.02, 0.02, 0.02);
	model_duck.Draw();
	glPopMatrix();
}

void stopAllDucks() {
	// Duck 1
	duckTransX1 = 0;
	duckTransY1 = 0;
	duckTransZ1 = 0;
	duckJ1 = 0.0;
	// Duck 2
	duckTransZ2 = 0;
	duckRot2 = 0.0;
	// Duck 3
	duckTransZ3 = 0;
	duckTransY3 = 0;
	duckRot3 = 0;
	incRot3 = true;
	// Duck 4
	duckTransZ4 = 0;
	duckTransY4 = 0;
	// Duck 5
	duckTransX5 = 0;
	duckTransZ5 = 0;
	// Duck 6
	duckTransX6 = 0;
	duckTransY6 = 0.0;
	duckTransZ6 = 0;
	duckRot6 = 0;
	// Duck 7
	duckTransZ7 = 0;
	back = false;
	// Duck 8
	duckTransX8 = 0;
	duckTransZ8 = 0;
	duckRot8 = 0;
	incRot8 = true;
	// Duck Touches
	duck_1 = false;
	duck_2 = false;
	duck_3 = false;
	duck_4 = false;
	duck_5 = false;
	duck_6 = false;
	duck_7 = false;
	duck_8 = false;
}

//=======================================================================
// Print Function
//=======================================================================
void print(int x, int y, char *string)
{
	int len, i;

	//set the position of the text in the window using the x and y coordinates
	glRasterPos2f(x, y);

	//get the length of the string to display
	len = (int)strlen(string);

	//loop to display character by character
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string[i]);
	}
}

//=======================================================================
// Display Function
//=======================================================================
void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat lightIntensity[] = { 0.7, 0.7, 0.7, 1.0f };
	GLfloat lightPosition[] = { 0.0f, 100.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);

	if (!gameOn) {
		if (won) {
			print(-5, 10, "You Win! Press P to Replay.");
		}
		else {
			print(-5, 10, "You Lose! Press P to Replay.");
		}
	}

	// Draw Ground
	glPushMatrix();	
	RenderGround();
	glPopMatrix();

	// Draw Tree Model
	glPushMatrix();
	glTranslatef(18, 0, -15);
	model_tree.Draw();
	glPopMatrix();

	// Draw Duck Models
	//Duck 1
	glPushMatrix();
	glTranslated(0, duckTransY1, duckTransZ1);
	drawDuck(-16, 0, -20);
	glPopMatrix();

	//Duck 2
	glPushMatrix();
	glTranslated(0, 0, duckTransZ2);
	glTranslated(-12, 0, -20);
	glRotated(duckRot2, 0, 1, 0);
	glTranslated(12, 0, 20);
	drawDuck(-12, 0, -20);
	glPopMatrix();
	
	//Duck 3 
	glPushMatrix();
	glTranslated(0, 0, duckTransZ3);
	glTranslated(-8, .3, -20);
	glRotated(duckRot3, 0, 0, 1);
	glTranslated(8, -0.3, 20);
	drawDuck(-8, 0.3, -20);
	glPopMatrix();

	//Duck 4
	glPushMatrix();
	glTranslated(0, duckTransY4, duckTransZ4);
	drawDuck(-4, 0, -20);
	glPopMatrix();

	//Duck 5
	glPushMatrix();
	glTranslated(duckTransX5, 0, duckTransZ5);
	drawDuck(4, 0,-20);
	glPopMatrix();

	////Duck 6
	glPushMatrix();
	glTranslated(0, 0, duckTransZ6);
	glTranslated(8, duckTransY6, -20);
	glRotated(duckRot6, 1, 0, 0);
	glTranslated(-8, -duckTransY6, 20);
	drawDuck(8, duckTransY6, -20);
	glPopMatrix();

	////Duck 7
	glPushMatrix();
	glTranslated(0, 0, duckTransZ7);
	drawDuck(12, 0, -20);
	glPopMatrix();

    //Duck 8 
	glPushMatrix();
	glTranslated(0, 0, duckTransZ8);
	glTranslated(16, 0, -20);
	glRotated(duckRot8, 1, 0, 0);
	glTranslated(-16, 0, 20);
	drawDuck(16, 0, -20);
	glPopMatrix();

	//Duck Boss
	if (showBigDuck) {
		glPushMatrix();
		glTranslated(duckTransXB, 0, duckTransZB);
		glScalef(bossScaleX, bossScaleY, bossScaleZ);
		drawDuck(0, 0.0, -10);
		glPopMatrix();
	}
	
	// Draw Hunter Model
	glPushMatrix();
	glTranslatef(hunterTransX, 0, hunterTransZ);
	glTranslatef(0, 1, 20);
	glRotatef(-180.0f, 0, 1, 0);
	glTranslatef(-20, -1, -20);

	glTranslatef(20, 1, 20);
	glScalef(0.02, 0.02, 0.02);
	model_hunter.Draw();
	glPopMatrix();

	// Draw Stone Model
	if (stoneDrawEnable) {
		glPushMatrix();
		glTranslatef(hunterTransX + stoneTransX, 0, stoneTransZ+ hunterStoneZS);
		glTranslatef(0, 1.5, 19);
		glScalef(0.01, 0.01, 0.01);
		model_stone.Draw();
		glPopMatrix();
	}
	// Sky Box
	glPushMatrix();
	GLUquadricObj * qobj;
	qobj = gluNewQuadric();
	glScaled(0.5, 0.5, 0.5);
	glTranslated(50,0,0);
	glRotated(90,1,0,1);
	glBindTexture(GL_TEXTURE_2D, tex);
	gluQuadricTexture(qobj,true);
	gluQuadricNormals(qobj,GL_SMOOTH);
	gluSphere(qobj,100,100,100);
	gluDeleteQuadric(qobj);
	glPopMatrix();

	//Light
	glPushMatrix();
	if (lightRotateSwitch)
		glRotated(lightRot, 1, 1, 1);
	movingLights();
	glPopMatrix();

	stationaryLights();

	glutSwapBuffers();
}

void handleStoneCollision() {
	//Duck 1
	if (((-16 >= -1.9 + hunterTransX+ stoneTransX) && (-16 <= 1.9 + hunterTransX+stoneTransX))
		|| ((-16 + 1.5 >= -1.9 + hunterTransX+stoneTransX) && (-16 + 1.5 <= 1.9 + hunterTransX+stoneTransX)))
	{

		if ((-20 + duckTransZ1 + 1.5 >= 19 + hunterStoneZS + stoneTransZ) && (-20 + duckTransZ1 + 1.5 <= 20 + hunterStoneZS + stoneTransZ)) {
		
				duckTransZ1 = 0;
				stoneTransZ = 0;
				stoneTransX = 0;
				stoneThrowEnableS = false;
				stoneThrowEnableDR = false;
				stoneThrowEnableDL = false;
				stoneDrawEnable = false;
				duckKills++;
			
		}
	}
	//Duck 2
	if (((-12 >= -1.9 + hunterTransX + stoneTransX) && (-12 <= 1.9 + hunterTransX + stoneTransX))
		|| ((-12 + 1.5 >= -1.9 + hunterTransX + stoneTransX) && (-12 + 1.5 <= 1.9 + hunterTransX + stoneTransX)))
	{

		if ((-20 + duckTransZ2 + 1.5 >= 19 + hunterStoneZS+stoneTransZ) && (-20 + duckTransZ2 + 1.5 <= 20 + hunterStoneZS + stoneTransZ)) {
			duckTransZ2 = 0;
			stoneTransZ = 0;
			stoneTransX = 0;
			stoneThrowEnableS = false;
			stoneThrowEnableDR = false;
			stoneThrowEnableDL = false;
			stoneDrawEnable = false;
			duckKills++;
		}
	}
	//Duck 3
	if (((-8 >= -1.9 + hunterTransX + stoneTransX) && (-8 <= 1.9 + hunterTransX + stoneTransX))
		|| ((-8 + 1.5 >= -1.9 + hunterTransX + stoneTransX) && (-8 + 1.5 <= 1.9 + hunterTransX + stoneTransX)))
	{

		if ((-20 + duckTransZ3 + 1.5 >= 19 + hunterStoneZS + stoneTransZ) && (-20 + duckTransZ3 + 1.5 <= 20 + hunterStoneZS + stoneTransZ)) {
			duckTransZ3 = 0;
			stoneTransZ = 0;
			stoneTransX = 0;
			stoneThrowEnableS = false;
			stoneThrowEnableDR = false;
			stoneThrowEnableDL = false;
			stoneDrawEnable = false;
			duckKills++;
		}
	}
	//Duck 4
	if (((-4 >= -1.9 + hunterTransX + stoneTransX) && (-4 <= 1.9 + hunterTransX + stoneTransX))
		|| ((-4 + 1.5 >= -1.9 + hunterTransX + stoneTransX) && (-4 + 1.5 <= 1.9 + hunterTransX + stoneTransX)))
	{

		if ((-20 + duckTransZ4 + 1.5 >= 19 + hunterStoneZS + stoneTransZ) && (-20 + duckTransZ4 + 1.5 <= 20 + hunterStoneZS + stoneTransZ)) {
			duckTransZ4 = 0;
			stoneTransZ = 0;
			stoneTransX = 0;
			stoneThrowEnableS = false;
			stoneThrowEnableDR = false;
			stoneThrowEnableDL = false;
			stoneDrawEnable = false;
			duckKills++;
		}
	}
	//Duck 5
	if (((4 >= -1.9 + hunterTransX + stoneTransX) && (4 <= 1.9 + hunterTransX + stoneTransX))
		|| ((4 + 1.5 >= -1.9 + hunterTransX + stoneTransX) && (4 + 1.5 <= 1.9 + hunterTransX + stoneTransX)))
	{

		if ((-20 + duckTransZ5 + 1.5 >= 19 + hunterStoneZS + stoneTransZ) && (-20 + duckTransZ5 + 1.5 <= 20 + hunterStoneZS + stoneTransZ)) {
			duckTransZ5 = 0;
			duckTransX5 = 0;
			stoneTransZ = 0;
			stoneTransX = 0;
			stoneThrowEnableS = false;
			stoneThrowEnableDR = false;
			stoneThrowEnableDL = false;
			stoneDrawEnable = false;
			duckKills++;
		}
	}
	//Duck 6
	if (((8 >= -1.9 + hunterTransX + stoneTransX) && (8 <= 1.9 + hunterTransX + stoneTransX))
		|| ((8 + 1.5 >= -1.9 + hunterTransX + stoneTransX) && (8 + 1.5 <= 1.9 + hunterTransX + stoneTransX)))
	{

		if ((-20 + duckTransZ6 + 1.5 >= 19 + hunterStoneZS + stoneTransZ) && (-20 + duckTransZ6 + 1.5 <= 20 + hunterStoneZS + stoneTransZ)) {
			duckTransZ6 = 0;
			stoneTransZ = 0;
			stoneTransX = 0;
			stoneThrowEnableS = false;
			stoneThrowEnableDR = false;
			stoneThrowEnableDL = false;
			stoneDrawEnable = false;
			duckKills++;
		}
	}

	//Duck 7
	if (((12 >= -1.9 + hunterTransX + stoneTransX) && (12 <= 1.9 + hunterTransX + stoneTransX))
		|| ((12 + 1.5 >= -1.9 + hunterTransX + stoneTransX) && (12 + 1.5 <= 1.9 + hunterTransX + stoneTransX)))
	{

		if ((-20 + duckTransZ7 + 1.5 >= 19 + hunterStoneZS + stoneTransZ) && (-20 + duckTransZ7 + 1.5 <= 20 + hunterStoneZS + stoneTransZ)) {
			duckTransZ7 = 0;
			stoneTransZ = 0;
			stoneTransX = 0;
			stoneThrowEnableS = false;
			stoneThrowEnableDR = false;
			stoneThrowEnableDL = false;
			stoneDrawEnable = false;
			duckKills++;
		}
	}
	//Duck 8
	if (((16 >= -1.9 + hunterTransX + stoneTransX) && (16 <= 1.9 + hunterTransX + stoneTransX))
		|| ((16 + 1.5 >= -1.9 + hunterTransX + stoneTransX) && (16 + 1.5 <= 1.9 + hunterTransX + stoneTransX)))
	{

		if ((-20 + duckTransZ8 + 1.5 >= 19 + hunterStoneZS + stoneTransZ) && (-20 + duckTransZ8 + 1.5 <= 20 + hunterStoneZS + stoneTransZ)) {
			duckTransZ8 = 0;
			stoneTransZ = 0;
			stoneTransX = 0;
			stoneThrowEnableS = false;
			stoneThrowEnableDR = false;
			stoneThrowEnableDL = false;
			stoneDrawEnable = false;
			duckKills++;
		}
	}
	//Duck Boss
	if (((0 + duckTransXB >= -1.9 + hunterTransX + stoneTransX) && (0 + duckTransXB <= 1.9 + hunterTransX + stoneTransX))
		|| ((0 + duckTransXB + 1.5 >= -1.9 + hunterTransX + stoneTransX) && (0 + duckTransXB + 1.5 <= 1.9 + hunterTransX + stoneTransX)))
	{

		if ((-20 + duckTransZB + 1.5 >= 19 + hunterStoneZS + stoneTransZ) && (-20 + duckTransZB + 1.5 <= 20 + hunterStoneZS + stoneTransZ)) {
			stoneTransZ = 0;
			stoneTransX = 0;
			stoneThrowEnableS = false;
			stoneThrowEnableDR = false;
			stoneThrowEnableDL = false;
			stoneDrawEnable = false;
			bossKills++;
		}
	}
}

void handleDuckCollision() {
	//Duck 1
	if (((-16 >= -1.9 + hunterTransX) && (-16  <= 1.9 + hunterTransX))
		|| ((-16 + 1.5 >= -1.9 + hunterTransX) && (-16 + 1.5 <= 1.9 + hunterTransX)))
	{
		
		if (((-20 + duckTransZ1 + 1.5 >= 19 + hunterTransZ) && (-20 + duckTransZ1 + 1.5 <= 20 + hunterTransZ))
			|| ((-20 + duckTransZ1 >= 19 + hunterTransZ) && (-20 + duckTransZ1 <= 20 + hunterTransZ))) {
			if (duckTransY1 <= 3.4)
			{
				touches++;
				duck_1 = true;
				eyeX = 0.0f;
				eyeZ = (firstView)? 19: (thirdView)? 30: 40;
				if (thirdView)
				{
					glLoadIdentity();	//Clear Model_View Matrix
					gluLookAt(eyeX, eyeY, eyeZ, At.x + eyeX, At.y, At.z + eyeZ - 30, Up.x, Up.y, Up.z);
				}
				else if (firstView) {
					glLoadIdentity();	//Clear Model_View Matrix
					gluLookAt(eyeX, eyeY, eyeZ, At.x + eyeX, At.y, At.z + eyeZ - 19, Up.x, Up.y, Up.z);
				}
			}
		}	
	}
	//Duck 2
	if (((-12 >= -1.9 + hunterTransX) && (-12 <= 1.9 + hunterTransX))
		|| ((-12 + 1.5 >= -1.9 + hunterTransX) && (-12 + 1.5 <= 1.9 + hunterTransX))) {
		if (((-20 + duckTransZ2 + 1.5 >= 19 + hunterTransZ) && (-20 + duckTransZ2 + 1.5 <= 20 + hunterTransZ))
			|| ((-20 + duckTransZ2 >= 19 + hunterTransZ) && (-20 + duckTransZ2 <= 20 + hunterTransZ))) {
				touches++;
				duck_2 = true;
				eyeX = 0.0f;
				eyeZ = (firstView) ? 19 : (thirdView) ? 30 : 40;
				if (thirdView)
				{
					glLoadIdentity();	//Clear Model_View Matrix
					gluLookAt(eyeX, eyeY, eyeZ, At.x + eyeX, At.y, At.z + eyeZ - 30, Up.x, Up.y, Up.z);
				}
				else if (firstView) {
					glLoadIdentity();	//Clear Model_View Matrix
					gluLookAt(eyeX, eyeY, eyeZ, At.x + eyeX, At.y, At.z + eyeZ - 19, Up.x, Up.y, Up.z);
				}
		}
	}
	////Duck 3
	if (((-8 >= -1.9 + hunterTransX) && (-8 <= 1.9 + hunterTransX))
		|| ((-8 + 1.5 >= -1.9 + hunterTransX) && (-8 + 1.5 <= 1.9 + hunterTransX)))
	{
		if (((-20 + duckTransZ3 + 1.5 >= 19 + hunterTransZ) && (-20 + duckTransZ3 + 1.5 <= 20 + hunterTransZ))
			|| ((-20 + duckTransZ3 >= 19 + hunterTransZ) && (-20 + duckTransZ3 <= 20 + hunterTransZ))) {
				touches++;
				duck_3 = true;
				eyeX = 0.0f;
				eyeZ = (firstView) ? 19 : (thirdView) ? 30 : 40;
				if (thirdView)
				{
					glLoadIdentity();	//Clear Model_View Matrix
					gluLookAt(eyeX, eyeY, eyeZ, At.x + eyeX, At.y, At.z + eyeZ - 30, Up.x, Up.y, Up.z);
				}
				else if (firstView) {
					glLoadIdentity();	//Clear Model_View Matrix
					gluLookAt(eyeX, eyeY, eyeZ, At.x + eyeX, At.y, At.z + eyeZ - 19, Up.x, Up.y, Up.z);
				}
		}	
	}
	//Duck 4
	if (((-4 >= -1.9 + hunterTransX) && (-4 <= 1.9 + hunterTransX))
		|| ((-4 + 1.5 >= -1.9 + hunterTransX) && (-4 + 1.5 <= 1.9 + hunterTransX)))
	{
		
		if (((-20 + duckTransZ4 + 1.5 >= 19 + hunterTransZ) && (-20 + duckTransZ4 + 1.5 <= 20 + hunterTransZ))
			|| ((-20 + duckTransZ4 >= 19 + hunterTransZ) && (-20 + duckTransZ4 <= 20 + hunterTransZ))) {
			
				touches++;
				duck_4 = true;
				eyeX = 0.0f;
				eyeZ = (firstView) ? 19 : (thirdView) ? 30 : 40;
				if (thirdView)
				{
					glLoadIdentity();	//Clear Model_View Matrix
					gluLookAt(eyeX, eyeY, eyeZ, At.x + eyeX, At.y, At.z + eyeZ - 30, Up.x, Up.y, Up.z);
				}
				else if (firstView) {
					glLoadIdentity();	//Clear Model_View Matrix
					gluLookAt(eyeX, eyeY, eyeZ, At.x + eyeX, At.y, At.z + eyeZ - 19, Up.x, Up.y, Up.z);
				}
		}
	}
	//Duck 5
	if (((4 >= -1.9 + hunterTransX) && (4 <= 1.9 + hunterTransX))
		|| ((4 + 1.5 >= -1.9 + hunterTransX) && (4 + 1.5 <= 1.9 + hunterTransX))) 
	{
		if (((-20 + duckTransZ5 + 1.5 >= 19 + hunterTransZ) && (-20 + duckTransZ5 + 1.5 <= 20 + hunterTransZ))
			|| ((-20 + duckTransZ5 >= 19 + hunterTransZ) && (-20 + duckTransZ5 <= 20 + hunterTransZ))) {
			touches++;
			duck_5 = true;
			eyeX = 0.0f;
			eyeZ = (firstView) ? 19 : (thirdView) ? 30 : 40;
			if (thirdView)
			{
				glLoadIdentity();	//Clear Model_View Matrix
				gluLookAt(eyeX, eyeY, eyeZ, At.x + eyeX, At.y, At.z + eyeZ - 30, Up.x, Up.y, Up.z);
			}
			else if (firstView) {
				glLoadIdentity();	//Clear Model_View Matrix
				gluLookAt(eyeX, eyeY, eyeZ, At.x + eyeX, At.y, At.z + eyeZ - 19, Up.x, Up.y, Up.z);
			}
		}
	}
	//Duck 6
	if (((8 >= -1.9 + hunterTransX) && (8 <= 1.9 + hunterTransX))
		|| ((8 + 1.5 >= -1.9 + hunterTransX) && (8 + 1.5 <= 1.9 + hunterTransX)))
	{

		if (((-20 + duckTransZ6 + 1.5 >= 19 + hunterTransZ) && (-20 + duckTransZ6 + 1.5 <= 20 + hunterTransZ))
			|| ((-20 + duckTransZ6 >= 19 + hunterTransZ) && (-20 + duckTransZ6 <= 20 + hunterTransZ))) {
			touches++;
			duck_6 = true;
			eyeX = 0.0f;
			eyeZ = (firstView) ? 19 : (thirdView) ? 30 : 40;
			if (thirdView)
			{
				glLoadIdentity();	//Clear Model_View Matrix
				gluLookAt(eyeX, eyeY, eyeZ, At.x + eyeX, At.y, At.z + eyeZ - 30, Up.x, Up.y, Up.z);
			}
			else if (firstView) {
				glLoadIdentity();	//Clear Model_View Matrix
				gluLookAt(eyeX, eyeY, eyeZ, At.x + eyeX, At.y, At.z + eyeZ - 19, Up.x, Up.y, Up.z);
			}
		}
	}
	//Duck 7
	if (((12 >= -1.9 + hunterTransX) && (12 <= 1.9 + hunterTransX))
		|| ((12 + 1.5 >= -1.9 + hunterTransX) && (12 + 1.5 <= 1.9 + hunterTransX)))
	{
		if (((-20 + duckTransZ7 + 1.5 >= 19 + hunterTransZ) && (-20 + duckTransZ7 + 1.5 <= 20 + hunterTransZ))
			|| ((-20 + duckTransZ7 >= 19 + hunterTransZ) && (-20 + duckTransZ7 <= 20 + hunterTransZ))) {
				touches++;
				duck_7 = true;
				eyeX = 0.0f;
				eyeZ = (firstView) ? 19 : (thirdView) ? 30 : 40;
				if (thirdView)
				{
					glLoadIdentity();	//Clear Model_View Matrix
					gluLookAt(eyeX, eyeY, eyeZ, At.x + eyeX, At.y, At.z + eyeZ - 30, Up.x, Up.y, Up.z);
				}
				else if (firstView) {
					glLoadIdentity();	//Clear Model_View Matrix
					gluLookAt(eyeX, eyeY, eyeZ, At.x + eyeX, At.y, At.z + eyeZ - 19, Up.x, Up.y, Up.z);
				}
		}
	}
	//Duck 8
	if (((16 >= -1.9 + hunterTransX) && (16 <= 1.9 + hunterTransX))
		|| ((16 + 1.5 >= -1.9 + hunterTransX) && (16 + 1.5 <= 1.9 + hunterTransX)))
	{
		if (((-20 + duckTransZ8 + 1.5 >= 19 + hunterTransZ) && (-20 + duckTransZ8 + 1.5 <= 20 + hunterTransZ))
			|| ((-20 + duckTransZ8 >= 19 + hunterTransZ) && (-20 + duckTransZ8 <= 20 + hunterTransZ))) {
				touches++;
				duck_8 = true;
				eyeX = 0.0f;
				eyeZ = (firstView) ? 19 : (thirdView) ? 30 : 40;
				if (thirdView)
				{
					glLoadIdentity();	//Clear Model_View Matrix
					gluLookAt(eyeX, eyeY, eyeZ, At.x + eyeX, At.y, At.z + eyeZ - 30, Up.x, Up.y, Up.z);
				}
				else if (firstView) {
					glLoadIdentity();	//Clear Model_View Matrix
					gluLookAt(eyeX, eyeY, eyeZ, At.x + eyeX, At.y, At.z + eyeZ - 19, Up.x, Up.y, Up.z);
				}
		}
	}
	//Duck Boss
	if (((0 + duckTransXB>= -1.9 + hunterTransX) && (0 + duckTransXB <= 1.9 + hunterTransX))
		|| ((0 + 1.5 + duckTransXB >= -1.9 + hunterTransX) && (0 + 1.5 + duckTransXB <= 1.9 + hunterTransX)))
	{
		if (((-20 + duckTransZB + 1.5 >= 19 + hunterTransZ) && (-20 + duckTransZB + 1.5 <= 20 + hunterTransZ))
			|| ((-20 + duckTransZB >= 19 + hunterTransZ) && (-20 + duckTransZB <= 20 + hunterTransZ))) {
			tex_ground.Load("Textures/ground/grass.bmp");
			loadBMP(&tex, "Textures/weather/clear.bmp", true);
			level = 1;
			resetAllVariables();
			gameOn = false;
			eyeX = 0.0f;
			eyeZ = (firstView) ? 19 : (thirdView) ? 30 : 40;
			duckTransY6 = 0.0;
		}
	}
}

void handleDuckTouches() {
	if (duck_1) {
		hunterTransX = 0;
		hunterTransZ = 0;
		duck_1 = false;
	}
	if (duck_2) {
		hunterTransX = 0;
		hunterTransZ = 0;
		duck_2 = false;
	}
	if (duck_3) {
		hunterTransX = 0;
		hunterTransZ = 0;
		duck_3 = false;
	}
	if (duck_4) {
		hunterTransX = 0;
		hunterTransZ = 0;
		duck_4 = false;
	}
	if (duck_5) {
		hunterTransX = 0;
		hunterTransZ = 0;
		duck_5 = false;
	}
	if (duck_6) {
		hunterTransX = 0;
		hunterTransZ = 0;
		duck_6 = false;
	}
	if (duck_7) {
		hunterTransX = 0;
		hunterTransZ = 0;
		duck_7 = false;
	}
	if (duck_8) {
		hunterTransX = 0;
		hunterTransZ = 0;
		duck_8 = false;
	}
	
	if (touches >= 3) {
		tex_ground.Load("Textures/ground/grass.bmp");
		loadBMP(&tex, "Textures/weather/clear.bmp", true);
		level = 1;
		resetAllVariables();
		gameOn = false;
		duckTransY6 = 0.0;
	}
}

void handleStoneThrow() {
	if (stoneThrowEnableS) {
		if (stoneTransZ + hunterStoneZS > -40)
			stoneTransZ -= 1;
		else {
			stoneTransZ = 0;
			stoneThrowEnableS = false;
			stoneDrawEnable = false;
		}
	}
	else if (stoneThrowEnableDR)
	{
		if (stoneTransZ + hunterTransZ > -40 && stoneTransX < 20)
		{
			stoneTransX += 0.5;
			stoneTransZ -= 1.0;
		}
		else {
			stoneTransX = 0;
			stoneTransZ = 0;
			stoneThrowEnableDR = false;
			stoneDrawEnable = false;
		}
	}
	else if (stoneThrowEnableDL)
	{
		if (stoneTransZ + hunterTransZ > -40 && stoneTransX > -20)
		{
			stoneTransX -= 0.5;
			stoneTransZ -= 1.0;
		}
		else {
			stoneTransX = 0;
			stoneTransZ = 0;
			stoneThrowEnableDL = false;
			stoneDrawEnable = false;
		}
	}
}


//=======================================================================
// Keyboard Function
//=======================================================================
void Anim() {

	if (!gameOn) {
		return;
	}

	if (!showBigDuck) {
		//Duck 1
		if (duckTransZ1 < 40)
			duckTransZ1 += 0.13;
		else {
			duckTransZ1 = 0;
			duckJ1 = 0;
		}
	
		if (duckJ1 < 3.14 )
			duckJ1 += 0.01;
		else
			duckJ1 = 0;

		duckTransY1 = 2*sin(duckJ1);
		//Duck 2
		if (duckTransZ2 < 40)
			duckTransZ2 += 0.2;
		else 
			duckTransZ2 = 0;

		duckRot2 += 45;
		//Duck 3
		if (duckTransZ3 < 40)
			duckTransZ3 += 0.08;
		else 
			duckTransZ3 = 0;

		if (duckRot3 < 30 && incRot3)
			duckRot3++;
		else if (duckRot3 >= 30)
			incRot3 = false;

		if (duckRot3 > -30 && !incRot3)
			duckRot3--;
		else if (duckRot3 <= -30)
			incRot3 = true;
		//Duck 4
		if (duckTransZ4 < 40)
			duckTransZ4 += 0.1;
		else 
			duckTransZ4 = 0;
		duckTransY4 = (1.5*sin(duckTransZ4)>0)? 1.5*sin(duckTransZ4) : -1.5*sin(duckTransZ4);
		//Duck 5
		if (duckTransZ5 < 40)
			duckTransZ5 += 0.1;
		else 
			duckTransZ5 = 0;
		duckTransX5 = 1.5*sin(duckTransZ5);
		//Duck 6
		if (duckTransZ6 < 40)
			duckTransZ6 += 0.1;
		else 
			duckTransZ6 = 0;
		
		duckRot6 += 10;
		//Duck 7
		if (duckTransZ7 < 38 && !back) 
			duckTransZ7 += 0.1;
		else 
			back = true;
		if (duckTransZ7 > 0 && back) 
			duckTransZ7 -= 0.1;
		else 
			back = false;
		//Duck 8 
		if (duckTransZ8 < 40)
			duckTransZ8 += 0.08;
		else
			duckTransZ8 = 0;

		if (duckRot8 < 30 && incRot8)
			duckRot8++;
		else if (duckRot8 >= 30)
			incRot8 = false;

		if (duckRot8 > -30 && !incRot8)
			duckRot8--;
		else if (duckRot8 <= -30)
			incRot8 = true;
	}
	//Duck Boss
	if (showBigDuck) {
		if (duckTransZB < 38 && !bossBack)
			duckTransZB += 0.1;
		else
			bossBack = true;

		if (duckTransZB > 0 && bossBack)
			duckTransZB -= 0.1;
		else
			bossBack = false;
		duckTransXB = 10*sin(duckTransZB);
	}
	
	handleDuckCollision();

	handleDuckTouches();

	handleStoneThrow();

	handleStoneCollision();

	//Duck Kills
	if (duckKills == 4) {
		showBigDuck = true;
		stopAllDucks();
	}
	//Boss Kills
	if (bossKills == 1) {
		bossScaleX = 3.5;
		bossScaleY = 3.5;
	} 
	else if (bossKills == 2) {
		bossScaleX = 3.0;
		bossScaleY = 3.0;
	}
	else if (bossKills == 3) {
		bossScaleX = 2.5;
		bossScaleY = 2.5;
	}
	else if (bossKills == 4) {
		bossScaleX = 2.0;
		bossScaleY = 2.0;
		level++;

		if (level == 2) {
			tex_ground.Load("Textures/ground/snow.bmp");
			loadBMP(&tex, "Textures/weather/milky.bmp", true);
		}
		else {
			if (level == 3) {
				tex_ground.Load("Textures/ground/muddy.bmp");
				loadBMP(&tex, "Textures/weather/lightning.bmp", true);
			}
			else {
				tex_ground.Load("Textures/ground/grass.bmp");
				loadBMP(&tex, "Textures/weather/clear.bmp", true);
				level = 1;
				resetAllVariables();
				gameOn = false;
				duckTransY6 = 0.0;
				won = true;
			}
		}
		resetAllVariables();
	} 
	//Light
	lightRot += 5.0;

	for (int i = 0; i < 10000000; i++);
	glutPostRedisplay();
}

void myKeyboard(unsigned char button, int x, int y)
{
	switch (button)
	{
	case 'w':
		if (!stoneThrowEnableDR && !stoneThrowEnableDL) {
			stoneDrawEnable = true;
			stoneThrowEnableS = true;
			hunterStoneZS = hunterTransZ;
		}
		
		break;
	case 'q':
		if (!stoneThrowEnableS && !stoneThrowEnableDR) {
			stoneDrawEnable = true;
			stoneThrowEnableDL = true;
			hunterStoneZS = hunterTransZ;
		}
		break;
	case 'e':
		if (!stoneThrowEnableS && !stoneThrowEnableDL) {
			stoneDrawEnable = true;
			stoneThrowEnableDR = true;
			hunterStoneZS = hunterTransZ;
		}
		break;
	case 't':
		thirdView = firstView ? false : !thirdView;
		if (thirdView)
		{
			if (! firstView) {
				eyeZ = 30 + hunterTransZ;
				eyeX = 0 + hunterTransX;
				eyeY = 3;
				glLoadIdentity();	//Clear Model_View Matrix
				gluLookAt(eyeX, eyeY, eyeZ, At.x + eyeX, At.y, At.z + eyeZ - 30, Up.x, Up.y, Up.z);
			}
		}
		else if( !firstView) {
			eyeZ = 40;
			eyeX = 0;
			eyeY = 5;
			glLoadIdentity();	//Clear Model_View Matrix
			gluLookAt(eyeX, eyeY, eyeZ, At.x, At.y, At.z, Up.x, Up.y, Up.z);
		}
		
		break;
	case 'f':
		firstView = thirdView ? false: !firstView;
		if (firstView)
		{
			if (!thirdView)
			{
				eyeZ = 19 + hunterTransZ;
				eyeX = 0 + hunterTransX;
				eyeY = 3;
				glLoadIdentity();	//Clear Model_View Matrix
				gluLookAt(eyeX, eyeY, eyeZ, At.x + eyeX, At.y, At.z + eyeZ - 19, Up.x, Up.y, Up.z);
			}
		}
		else if(! thirdView) {
			eyeZ = 40;
			eyeX = 0;
			eyeY = 5;
			glLoadIdentity();	//Clear Model_View Matrix
			gluLookAt(eyeX, eyeY, eyeZ, At.x, At.y, At.z, Up.x, Up.y, Up.z);
		}
		
		break;
	case 'p':
		duckTransY6 = 1.5;
		gameOn = true;
		won = false;
		break;
	case '[':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case ']':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 'r'://rotating the light
		lightRotateSwitch = !lightRotateSwitch;
		lightRot = 0;
			break;
	case 's'://spot light or directional
		spotLight = !spotLight;
			break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

//=======================================================================
// Motion Function
//=======================================================================
void myMotion(int x, int y)
{
	y = HEIGHT - y;

	if (cameraZoom - y > 0)
	{
		Eye.x += -0.1;
		Eye.z += -0.1;
	}
	else
	{
		Eye.x += 0.1;
		Eye.z += 0.1;
	}

	cameraZoom = y;

	glLoadIdentity();	//Clear Model_View Matrix

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);	//Setup Camera with modified paramters

	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glutPostRedisplay();	//Re-draw scene 
}

//=======================================================================
// Mouse Function
//=======================================================================
void myMouse(int button, int state, int x, int y)
{
	y = HEIGHT - y;

	if (state == GLUT_DOWN)
	{
		cameraZoom = y;
	}
}


//=======================================================================
// Reshape Function
//=======================================================================
void myReshape(int w, int h)
{
	if (h == 0) {
		h = 1;
	}

	WIDTH = w;
	HEIGHT = h;

	// set the drawable region of the window
	glViewport(0, 0, w, h);

	// set up the projection matrix 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (GLdouble)WIDTH / (GLdouble)HEIGHT, zNear, zFar);

	// go back to modelview matrix so we can move the objects about
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
}

//=======================================================================
// Assets Loading Function
//=======================================================================
void LoadAssets()
{
	// Loading Model files
	model_house.Load("Models/house/house.3DS");
	model_tree.Load("Models/tree/Tree1.3ds");
	model_duck.Load("Models/duck/duck.3DS");
	model_hunter.Load("Models/hunter/rapper.3DS");
	model_stone.Load("Models/stone/Mesh.3DS");

	// Loading texture files
	tex_ground.Load("Textures/ground/grass.bmp");
	loadBMP(&tex, "Textures/weather/clear.bmp", true);
}

//=======================================================================
// Special Function
//=======================================================================
void Special(int key, int x, int y) {

	switch (key) {
		case GLUT_KEY_UP:
			if (hunterTransZ > -35) {
				
				hunterTransZ -= 0.5;
				if (thirdView)
				{
					eyeZ -= 0.5;
					glLoadIdentity();
					gluLookAt(eyeX, eyeY, eyeZ, At.x + eyeX, At.y, At.z + eyeZ - 30, Up.x, Up.y, Up.z);
				}
				else if (firstView)
				{
					if(eyeZ > -20)
						eyeZ -= 0.5;
					glLoadIdentity();
					gluLookAt(eyeX, eyeY, eyeZ, At.x + eyeX, At.y, At.z + eyeZ - 19, Up.x, Up.y, Up.z);
				}
				
			}
			break;
		case GLUT_KEY_DOWN:
			if (hunterTransZ < 1.0) {
				hunterTransZ += 0.5;
				if (thirdView)
				{
					eyeZ += 0.5;
					glLoadIdentity();
					gluLookAt(eyeX, eyeY, At.z + eyeZ - 20, At.x + eyeX, At.y, eyeZ + 10, Up.x, Up.y, Up.z);
				}
				else if (firstView)
				{
					if (eyeZ < 20)
						eyeZ += 0.5;
					glLoadIdentity();
					gluLookAt(eyeX, eyeY, eyeZ, At.x + eyeX, At.y, At.z + eyeZ - 19, Up.x, Up.y, Up.z);
				}
			}
			break;
		case GLUT_KEY_LEFT:
			if (hunterTransX > -16) {
				hunterTransX -= 0.5;
				if (thirdView)
				{
					eyeX -= 0.5;
					glLoadIdentity();
					gluLookAt(eyeX, eyeY, eyeZ, At.x + eyeX, At.y, At.z + eyeZ - 30, Up.x, Up.y, Up.z);
				}
				else if (firstView)
				{
					if (eyeX > -20)
						eyeX -= 0.5;
					glLoadIdentity();
					gluLookAt(eyeX, eyeY, eyeZ, At.x + eyeX, At.y, At.z + eyeZ - 19, Up.x, Up.y, Up.z);
				}
			}
			break;
		case GLUT_KEY_RIGHT:
			if (hunterTransX < 16) {
				hunterTransX += 0.5;
				if (thirdView)
				{
					eyeX += 0.5;
					glLoadIdentity();
					gluLookAt(eyeX, eyeY, eyeZ, At.x + eyeX, At.y, At.z + eyeZ - 30, Up.x, Up.y, Up.z);
				}
				else if (firstView)
				{
					if (eyeX > -20)
						eyeX += 0.5;
					glLoadIdentity();
					gluLookAt(eyeX, eyeY, eyeZ, At.x + eyeX, At.y, At.z + eyeZ - 19, Up.x, Up.y, Up.z);
				}
			}
			break;
	}
	
	glutPostRedisplay();
}

//=======================================================================
// Main Function
//=======================================================================
void main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);

	glutInitWindowPosition(100, 50);

	glutCreateWindow(title);

	glutDisplayFunc(myDisplay);

	glutIdleFunc(Anim);

	glutKeyboardFunc(myKeyboard);

	glutSpecialFunc(Special);

	glutMotionFunc(myMotion);

	glutMouseFunc(myMouse);

	glutReshapeFunc(myReshape);

	myInit();

	LoadAssets();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}