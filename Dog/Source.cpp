#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

int action = 0;   // Current action
int startTime = 0;   // Starting time of animation
bool sideView = false;    // Side/front camera view
GLUquadricObj* qobj; // Quadric object

// GLUT callback for window resizing
void reshape(int width, int height)
{
	// Updating viewport
	glViewport(0, 0, width, height);

	// Setting projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 1000, 0, 1000, -1000, 1000);
	glMatrixMode(GL_MODELVIEW);
}

// Draws a cylinder
void drawCylinder(double radius, double height)
{
	// Drawing 1st cap
	glPushMatrix();
	gluDisk(qobj, 0, radius, 30, 1);

	// Drawing cylinder
	gluCylinder(qobj, radius, radius, height, 30, 1);

	// Drawing 2nd cap
	glTranslatef(0, 0, height);
	gluDisk(qobj, 0, radius, 30, 1);
	glPopMatrix();
}

// Draws a limb of the dog
void drawLimb(double lowerAngle, double footAngle)
{
	// Upper limb
	glPushMatrix();
	drawCylinder(40, 200);

	// Lower limb
	glTranslatef(0, 0, 180);
	glRotatef(lowerAngle, 1, 0, 0);
	drawCylinder(40, 200);
	
	// Foot
	glTranslatef(0, 0, 180);
	glRotatef(-90 + footAngle, 1, 0, 0);
	drawCylinder(20, 100);
	glPopMatrix();
}

// Draws the dog
void drawDog()
{
	// Time since animation start
	double time = (glutGet(GLUT_ELAPSED_TIME) - startTime) * 0.05;

	// Angles in neutral position
	double frontLegAngle1 = 0;
	double frontLegAngle2 = 0;
	double hindLegAngle = 0;
	double neckHeadAngle = 0;
	double tailAngle = 0;

	// Animating angles
	switch(action)
	{
	case 1: // Bend front leg
		// Decrease 1st front leg angle with time
		frontLegAngle1 = -time;
		if (frontLegAngle1 < -45) frontLegAngle1 = -45;
		break;
	case 2: // Lift on hind legs
		// Decrease both front leg angles with time
		frontLegAngle1 = -time;
		if (frontLegAngle1 < -30) frontLegAngle1 = -30;
		frontLegAngle2 = frontLegAngle1;

		// Decrease both hind leg angles with time
		hindLegAngle = time;
		if (hindLegAngle > 30) hindLegAngle = 30;
		break;
	case 3: // Bend neck
		// Increase neck-head angle with time
		neckHeadAngle = time;
		if (neckHeadAngle > 30) neckHeadAngle = 30;
		break;
	case 4: // Wiggle tail
		// Shake tail and stop after 2 times
		tailAngle = 30 * sin(time * 0.1);
		break;
	}

	// Drawing torso
	glRotatef(-hindLegAngle, 1, 0, 0);
	drawCylinder(100, 400);

	// Drawing tail
	glPushMatrix();
	glTranslatef(0, 50, 20);
	glRotatef(150 + tailAngle, 1, 0, 0);
	drawCylinder(30, 200);
	glPopMatrix();

	// Drawing 1st hind leg
	glPushMatrix();
	glTranslatef(-60, 0, 50);
	glRotatef(90 + hindLegAngle * 0.5, 1, 0, 0);
	drawLimb(hindLegAngle, -hindLegAngle * 0.5);
	glPopMatrix();

	// Drawing 2nd hind leg
	glPushMatrix();
	glTranslatef(60, 0, 50);
	glRotatef(90 + hindLegAngle * 0.5, 1, 0, 0);
	drawLimb(hindLegAngle, -hindLegAngle * 0.5);
	glPopMatrix();

	// Drawing 1st front leg
	glPushMatrix();
	glTranslatef(-60, 0, 350);
	glRotatef(90 + frontLegAngle1, 1, 0, 0);
	drawLimb(-frontLegAngle1, 0);
	glPopMatrix();

	// Drawing 2nd front leg
	glPushMatrix();
	glTranslatef(60, 0, 350);
	glRotatef(90 + frontLegAngle2, 1, 0, 0);
	drawLimb(-frontLegAngle2, 0);
	glPopMatrix();

	// Drawing neck
	glTranslatef(0, 30, 380);
	glRotatef(-20 + neckHeadAngle, 1, 0, 0);
	drawCylinder(40, 200);

	// Drawing head
	glTranslatef(0, 0, 200);
	gluSphere(qobj, 80, 30, 30);
}

// GLUT callback for window drawing
void display(void)
{
	// Clear window
	glClearColor(0.1f, 0.2f, 0.3f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// Enabling lighting to achieve additional realism
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Setting side/front camera view
	glLoadIdentity();
	if (sideView)
		glTranslatef(400, 500, 0);
	else
	{
		glTranslatef(200, 500, 0);
		glRotatef(90, 0, 1, 0);
	}

	// Drawing dog
	drawDog();
	glutSwapBuffers();
	glutPostRedisplay();
}

// GLUT callback for menu selections
void menuOptions(int selection)
{
	if (selection == 6)
	{
		// Change side/front camera view
		sideView = !sideView;
	}
	else if (selection == 7)
		exit(0);
	else
	{
		// Start selected animation
		action = selection;
		startTime = glutGet(GLUT_ELAPSED_TIME);
	}

	glutPostRedisplay();
}

// Creating a menu
void menu()
{
	int menu;
	glutCreateMenu(menuOptions);
	glutAddMenuEntry("Bend Front Leg", 1);
	glutAddMenuEntry("Lift on Hind Legs", 2);
	glutAddMenuEntry("Bend Neck-Head", 3);
	glutAddMenuEntry("Wiggle Tail", 4);
	glutAddMenuEntry("Reset Position", 5);
	glutAddMenuEntry("Change Camera", 6);
	glutAddMenuEntry("Shut Down", 7);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Main
int main(int argc, char** argv)
{
	// Creating the main window
	glutInit(&argc, argv);
	glutInitWindowPosition(200, 100);
	glutInitWindowSize(1024, 768);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Dog");

	// Setting callbacks
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	// Creating the menu
	menu();

	// Creating a new Quadric Object
	qobj = gluNewQuadric();

	// Starting the event processing loop
	glutMainLoop();
	return 0;
}
