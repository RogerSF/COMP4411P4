// The sample robotarm model.  You should build a file
// very similar to this for when you make your model.
#pragma warning (disable : 4305)
#pragma warning (disable : 4244)
#pragma warning(disable : 4786)

#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include "particleSystem.h"


#include <FL/gl.h>
#include <stdlib.h>
#include <math.h>
#include "modelerglobals.h"
#include "modelerui.h"
#include "vec.h"
#include "camera.h"
#include <gl/glu.h>
#include <complex>
#include <algorithm>

#define M_DEFAULT 2.0f
#define M_OFFSET 3.0f
#define P_OFFSET 0.3f
#define MAX_VEL 200
#define MIN_STEP 0.1

#define MEGAMAN_LIGHT 0.455f, 0.7255f, 0.855f
#define MEGAMAN_DARK 0.3098f, 0.4902f, 0.8275f
#define MEGAMAN_RED 1.0f, 0.0f, 0.0f
#define MEGAMAN_SKIN 1.0f, 0.8667f, 0.7647f

#define FIREMAN_LIGHT 0.588f, 0.592f, 0.5608f
#define FIREMAN_DARK 0.9294f, 0.2627f, 0.15686f

#define MIRROR_COLOR 0.8f, 1.0f, 1.0f

// To make a MegamanModel, we inherit off of ModelerView
class MegamanModel : public ModelerView 
{
public:
	MegamanModel(int x, int y, int w, int h, char *label)
        : ModelerView(x,y,w,h,label) {}

    virtual void draw();
	void drawMegamanModel();

	void addCustomLighting();
	// void animateStep();
	void setMegamanColor(int type, bool light);
	void drawFace();

private:
	const int animationTotalFrame = 120;
	int animationFrame = 0;
	float rightArmFlex = 0;
	float rightArmSideFlex = 0;
	float rightForearmFlex = 0;
	float leftArmFlex = 0;
	float leftArmSideFlex = 0;
	float leftForearmFlex = 0;
	float energyPathTransformFactor = 0;
	float laserTransformFactor = 0;
	bool showChargingEnergy = false;
	bool showLaser = false;

	float lerp(float a, float b, float f)
	{
		return a + f * (b - a);
	}
};

// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView* createMegamanModel(int x, int y, int w, int h, char *label)
{
    return new MegamanModel(x,y,w,h,label); 
}

void MegamanModel::setMegamanColor(int type, bool light)
{
	switch(type)
	{
		case 0:
			if(light)
				setDiffuseColor(MEGAMAN_LIGHT);
			else
				setDiffuseColor(MEGAMAN_DARK);
			break;
		case 1:
			if (light)
				setDiffuseColor(FIREMAN_LIGHT);
			else
				setDiffuseColor(FIREMAN_DARK);
			break;
		default:
			break;
	}
}


// We are going to override (is that the right word?) the draw()
// method of ModelerView to draw out MegamanModel
void MegamanModel::draw()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set the background to white

	// This call takes care of a lot of the nasty projection 
	// matrix stuff.  Unless you want to fudge directly with the 
	// projection matrix, don't bother with this ...

	// this->addCustomLighting(); //Whistle No.1
	// this->animateStep();
	ModelerView::draw();

	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);	//Clear the buffers

	// Mirror reflection stuff
	// Disable color and depth buffers
	glDepthMask(false);													//Disable writting in depth buffer

	glEnable(GL_STENCIL_TEST);												//Enable Stencil test
	glStencilFunc(GL_ALWAYS, 1, 1);									//Test always success, value written 1
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);								//Stencil & Depth test passes => replace existing value

	// Draw reflection plane
	glPushMatrix();
		setDiffuseColor(MIRROR_COLOR);
		glTranslated(-5, -5, 10);
		drawBox(10, 10, 0.1);
	glPopMatrix();

	glDepthMask(true);

	glStencilFunc(GL_EQUAL, 1, 1);                   //Draw only where stencil buffer is 1
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);    //Stencil buffer read only

	glPushMatrix();
		glTranslated(0, 0, 10);
		glScalef(1.0f, 1.0f, -1.0f);                        //Mirror Z
		drawMegamanModel();
	glPopMatrix();

	glDisable(GL_STENCIL_TEST);                        //Disable Stencil test

	drawMegamanModel();

	endDraw();
}

void MegamanModel::drawMegamanModel()
{
	int megamanType = VAL(MEGAMAN_TYPE);

	// draw the sample model
	setDiffuseColor(COLOR_GREEN);
	glPushMatrix();
	glTranslated(VAL(XPOS), VAL(YPOS), VAL(ZPOS));

		// MEGAMAN: BODY
		setMegamanColor(megamanType, true);
		glPushMatrix();
		glTranslated(-2, -2.5, -1.25);
		drawBox(4, 5, 2.5);

			if(VAL(LV_DETAIL) >= 1){
			// MEGAMAN: HEAD
			setDiffuseColor(MEGAMAN_SKIN);
			glPushMatrix();
			glTranslated(2, 5.8, 1.25);
			glRotated(VAL(HEAD_NOD), 1, 0, 0);
			glRotated(VAL(HEAD_SHAKE), 0, 1, 0);
			glRotated(VAL(HEAD_TILT), 0, 0, 1);
			glTranslated(0, 1.5, 0.2);
			drawSphere(2.4);
			// drawFace();

			if(megamanType == 1) {
				// Draw torus and diamond when megaman type is fireman
				setDiffuseColor(COLOR_GOLD);
				// drawDiamond(0, 4.5, 0, 1, 1, 1);
				// drawTorus(0, 4.5, 0, 1.5, 2);
			}

				// MEGAMAN: HELMET TOP
				setMegamanColor(megamanType, false);
				glPushMatrix();
				glTranslated(0, 0.5, -0.2);
				drawSphere(2.5);
				glPopMatrix();

				// MEGAMAN: HELMET LEFT
				// setDiffuseColor(MEGAMAN_SKIN);
				glPushMatrix();
				glTranslated(-0.2, 0, -0.28);
				drawSphere(2.5);
				glPopMatrix();

				// MEGAMAN: HELMET LEFT
				glPushMatrix();
				glTranslated(0.2, 0, -0.28);
				drawSphere(2.5);
				glPopMatrix();

				// MEGAMAN: HELMET LEFT EAR
				setMegamanColor(megamanType, true);
				glPushMatrix();
				glTranslated(2.45, 0, -0.2);
				glRotated(90, 0, 1, 0);
				drawCylinder(0.5, 1, 0.8);
				glPopMatrix();

				// MEGAMAN: HELMET RIGHT EAR
				glPushMatrix();
				glTranslated(-2.45, 0, -0.2);
				glRotated(90, 0, -1, 0);
				drawCylinder(0.5, 1, 0.8);
				glPopMatrix();

				// MEGAMAN: HELMET TOP
				glPushMatrix();
				glTranslated(0, 1.4, -0.2);
				glScaled(0.7, 1.8, 2.15);
				drawSphere(1);
				glPopMatrix();

				// MEGAMAN: HELMET TOP
				glPushMatrix();
				glTranslated(-0.35, 1, 2.15);
				glRotated(21, -1, 0, 0);
				drawTextureBox(0.7, 0.7, 0.2);
				glPopMatrix();

			glPopMatrix();
			}

			
			// MEGAMAN: RIGHT UPPER ARM
			setMegamanColor(megamanType, true);
			glPushMatrix();
			glTranslated(0, 4, 1.25);
			glRotated(-90, 0, 1, 0);
			glRotated(rightArmFlex, 0, 1, 0);
			glRotated(rightArmSideFlex, 1, 0, 0);
			if (VAL(LV_DETAIL) >= 1) {
				drawCylinder(3, 1, 1);
			}

			
			switch (megamanType) {
			default:
			case 0:
				// MEGAMAN: RIGHT FORE ARM
				setMegamanColor(megamanType, false);
				glPushMatrix();
				glTranslated(0, 0, 3);
				glRotated(rightForearmFlex, 0, 1, 0);
				if (VAL(LV_DETAIL) >= 2) {
					drawCylinder(3, 1.2, 1.2);
				}

					// MEGAMAN: RIGHT HAND
					glPushMatrix();
					glTranslated(0, 0, 3);
					if (VAL(LV_DETAIL) >= 3) {
						drawSphere(1);
					}
					glPopMatrix();

				glPopMatrix();
				break;
			case 1:
				// FIREMAN: LEFT FORE ARM
				setMegamanColor(megamanType, false);
				glPushMatrix();
				glTranslated(0, 0, 3);
				glRotated(leftForearmFlex, 0, -1, 0);
				if (VAL(LV_DETAIL) >= 2) {
					drawCylinder(2.5, 1.2, 1.8);
				}

					// FIREMAN: LEFT HAND CANNON
					glPushMatrix();
					glTranslated(0, 0, 2.5);
					glRotated(90, 1, 0, 0);
					if (VAL(LV_DETAIL) >= 3) {
						// drawTorus(0, 0, 0, 1, 1.5);
					}
					glRotated(-90, 1, 0, 0);

						setDiffuseColor(MEGAMAN_RED);
						glPushMatrix();
						glTranslated(0, 0, -0.5);
						if (VAL(LV_DETAIL) >= 3) {
							drawCylinder(0.55, 0.4, 0.4);
						}
						glPopMatrix();

					glPopMatrix();

				glPopMatrix();
				break;
			}

			glPopMatrix();

			// MEGAMAN: LEFT UPPER ARM
			setMegamanColor(megamanType, true);
			glPushMatrix();
			glTranslated(4, 4, 1.25);
			glRotated(90, 0, 1, 0);
			glRotated(leftArmFlex, 0, -1, 0);
			glRotated(leftArmSideFlex, 1, 0, 0);
			if (VAL(LV_DETAIL) >= 1) {
				drawCylinder(3, 1, 1);
			}

			switch (megamanType) {
			default:
			case 0:
				// MEGAMAN: LEFT FORE ARM
				setMegamanColor(megamanType, false);
				glPushMatrix();
				glTranslated(0, 0, 3);
				glRotated(leftForearmFlex, 0, -1, 0);
				if (VAL(LV_DETAIL) >= 2) {
					drawCylinder(2.5, 1.2, 1.5);
				}

						// MEGAMAN: LEFT HAND CANNON
						glPushMatrix();
						glTranslated(0, 0, 2.4);
						if (VAL(LV_DETAIL) >= 3) {
							drawCylinder(1, 1.4, 0.9);
						}

							glPushMatrix();
							glTranslated(0, 0, 0.5);
							if (VAL(LV_DETAIL) >= 3) {
								drawCylinder(1, 0.9, 0.5);
							}

								setDiffuseColor(MEGAMAN_RED);
								glPushMatrix();
								glTranslated(0, 0, 0.5);
								if (VAL(LV_DETAIL) >= 3) {
									drawCylinder(0.55, 0.4, 0.4);
								}
				break;
			case 1:
				// FIREMAN: LEFT FORE ARM
				setMegamanColor(megamanType, false);
				glPushMatrix();
				glTranslated(0, 0, 3);
				glRotated(leftForearmFlex, 0, -1, 0);
				if (VAL(LV_DETAIL) >= 2) {
					drawCylinder(2.5, 1.2, 1.8);
				}

						// FIREMAN: LEFT HAND CANNON
						glPushMatrix();
						glTranslated(0, 0, 2.5);
						glRotated(90, 1, 0, 0);
						if (VAL(LV_DETAIL) >= 3) {
							// drawTorus(0, 0, 0, 1, 1.5);
						}
						glRotated(-90, 1, 0, 0);
							
							glPushMatrix();

								setDiffuseColor(MEGAMAN_RED);
								glPushMatrix();
								glTranslated(0, 0, -0.5);
								if (VAL(LV_DETAIL) >= 3) {
									drawCylinder(0.55, 0.4, 0.4);
								}
				break;
			}

							// comment out animation code
							// if (VAL(LV_DETAIL) >= 3) {
							// 	// Charging energy
							// 	if (ModelerUserInterface::m_controlsAnimOnMenu->value() && this->showChargingEnergy) {

							// 		for (int i = 0; i < 30; i++)
							// 		{
							// 			float r = float(rand() % 20) / float(100);
							// 			float dx = float((rand() % 80) - 40) / float(20);
							// 			float dy = float((rand() % 80) - 40) / float(20);
							// 			float dz = float(rand() % 80) / float(20);
							// 			glPushMatrix();
							// 			glTranslated(dx*energyPathTransformFactor, dy*energyPathTransformFactor, dz*energyPathTransformFactor + 0.5);
							// 			drawSphere(r);
							// 			glPopMatrix();
							// 		}
							// 	}
							// 	else if (ModelerUserInterface::m_controlsAnimOnMenu->value() && this->showLaser)
							// 	{
							// 		glPushMatrix();
							// 		glTranslated(0, 0, laserTransformFactor * 3);
							// 		drawCylinder(2, 0.2, 0.2);
							// 		glPopMatrix();
							// 	}
							// }

							glPopMatrix();

						glPopMatrix();

					glPopMatrix();

				glPopMatrix();

			glPopMatrix();

			// MEGAMAN: PANTS
			setMegamanColor(megamanType, false);
			glPushMatrix();
			glTranslated(-0.25, -1, -0.25);
			if (VAL(LV_DETAIL) >= 1) {
				drawBox(4.5, 2, 3);
			}
			glPopMatrix();

			// MEGAMAN: RIGHT LEG
			setMegamanColor(megamanType, true);
			glPushMatrix();
			glTranslated(0.8, -0.5, 1.25);
			glRotated(90, 1, 0, 0);
			glRotated(VAL(RIGHT_LEG_FLEX), -1, 0, 0);
			glRotated(VAL(RIGHT_LEG_SIDE_FLEX), 0, -1, 0);
			if (VAL(LV_DETAIL) >= 1) {
				drawCylinder(2.5, 1, 1);
			}

				// MEGAMAN: RIGHT LOWER LEG
				setMegamanColor(megamanType, false);
				glPushMatrix();
				glTranslated(0, 0, 2.5);
				glRotated(VAL(RIGHT_LOWER_LEG_FLEX), 1, 0, 0);
				if (VAL(LV_DETAIL) >= 2) {
					drawCylinder(3, 1.2, 1.5);
				}

					// MEGAMAN: RIGHT FOOT
					glPushMatrix();
					glTranslated(0, 0.2, 3);
					glScaled(1, 1.2, 1);
					glRotated(VAL(RIGHT_FOOT_FLEX), 1, 0, 0);
					if (VAL(LV_DETAIL) >= 3) {
						drawCylinder(1.5, 1.2, 1.8);
					}
					glPopMatrix();

				glPopMatrix();

			glPopMatrix();

			// MEGAMAN: LEFT LEG
			setMegamanColor(megamanType, true);
			glPushMatrix();
			glTranslated(3.2, -0.5, 1.25);
			glRotated(90, 1, 0, 0);
			glRotated(VAL(LEFT_LEG_FLEX), 1, 0, 0);
			glRotated(VAL(LEFT_LEG_SIDE_FLEX), 0, 1, 0);
			if (VAL(LV_DETAIL) >= 1) {
				drawCylinder(2.5, 1, 1);
			}

				// MEGAMAN: LEFT LOWER LEG
				setMegamanColor(megamanType, false);
				glPushMatrix();
				glTranslated(0, 0, 2.5);
				glRotated(VAL(LEFT_LOWER_LEG_FLEX), 1, 0, 0);
				if (VAL(LV_DETAIL) >= 2) {
					drawCylinder(3, 1.2, 1.5);
				}

					// MEGAMAN: RIGHT FOOT
					glPushMatrix();
					glTranslated(0, 0.2, 3);
					glScaled(1, 1.2, 1);
					glRotated(VAL(LEFT_FOOT_FLEX), 1, 0, 0);
					if (VAL(LV_DETAIL) >= 3) {
						drawCylinder(1.5, 1.2, 1.8);
					}
					glPopMatrix();

				glPopMatrix();

			glPopMatrix();

		glPopMatrix();

	glPopMatrix();
}

// void MegamanModel::animateStep()
// {
// 	if(ModelerUserInterface::m_controlsAnimOnMenu->value())
// 	{
// 		if(this->animationFrame >= this->animationTotalFrame)
// 		{
// 			this->animationFrame = 0;
// 		} else
// 		{
// 			this->animationFrame += 1;
// 		}


// 		if (animationFrame < 30)
// 		{
// 			float percentage = float(this->animationFrame) / float(30);
// 			rightArmFlex = lerp(0, 90, percentage);
// 			rightArmSideFlex = lerp(80, -10, percentage);
// 			rightForearmFlex = lerp(0, 80, percentage);
// 			leftArmFlex = lerp(0, 90, percentage);
// 			leftArmSideFlex = lerp(80, 0, percentage);
// 			leftForearmFlex = 0;
// 		} else if (animationFrame < 55)
// 		{
// 			// Laser charge up
// 			this->showChargingEnergy = true;
// 			float percentage = float(this->animationFrame - 30) / float(25);
// 			energyPathTransformFactor = lerp(1, 0, percentage);
// 		} else if (animationFrame < 60)
// 		{
// 			this->showChargingEnergy = false;
// 			this->showLaser = true;
// 			float percentage = float(this->animationFrame - 55) / float(5);
// 			rightArmFlex = lerp(90, 50, percentage);
// 			rightArmSideFlex = lerp(-10, -30, percentage);
// 			leftArmSideFlex = lerp(0, -30, percentage);
// 			laserTransformFactor = lerp(0, 1, percentage);
// 		} else if (animationFrame < 65)
// 		{
// 			this->showLaser = false;
// 			float percentage = float(this->animationFrame - 60) / float(5);
// 			rightArmFlex = lerp(50, 20, percentage);
// 			rightArmSideFlex = lerp(-30, 0, percentage);
// 			leftArmSideFlex = lerp(-30, -10, percentage);
// 		} else if (animationFrame < 75)
// 		{
// 			// Pause
// 		} else if (animationFrame < 105)
// 		{
// 			float percentage = float(this->animationFrame - 75) / float(30);
// 			rightArmFlex = lerp(20, 0, percentage);
// 			rightArmSideFlex = lerp(0, 80, percentage);
// 			rightForearmFlex = lerp(80, 0, percentage);
// 			leftArmSideFlex = lerp(-10, 80, percentage);
// 			leftArmFlex = lerp(90, 0, percentage);
// 		} else
// 		{
// 			// Pause
// 		}
		
// 	} else
// 	{
// 		rightArmFlex = VAL(RIGHT_ARM_FLEX);
// 		rightArmSideFlex = VAL(RIGHT_ARM_SIDE_FLEX);
// 		rightForearmFlex = VAL(RIGHT_FOREARM_FLEX);
// 		leftArmFlex = VAL(LEFT_ARM_FLEX);
// 		leftArmSideFlex = VAL(LEFT_ARM_SIDE_FLEX);
// 		leftForearmFlex = VAL(LEFT_FOREARM_FLEX);
// 	}
// }

// void MegamanModel::addCustomLighting()
// {
// 	//Configuring the customized light
// 	glEnable(GL_LIGHTING);
// 	GLfloat* lightPos = new GLfloat[4];
// 	lightPos[0] = VAL(LIGHT_X);
// 	lightPos[1] = VAL(LIGHT_Y);
// 	lightPos[2] = VAL(LIGHT_Z);
// 	lightPos[3] = 1; //w value of light - not alpha! let lightsource be a point in space
// 	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
// 	//reference cube to show the position of light
// 	glPushMatrix();
// 	glTranslated(lightPos[0], lightPos[1], lightPos[2]);
	
// 	drawTextureBox(0.5, 0.5, 0.5);
// 	setDiffuseColor(1.0, 1.0, 0.0);
// 		glBegin( GL_LINES );  
//         glVertex3d( -0.1, 0, 0 );
//         glVertex3d( -1.7, 0, 0 );     
//         glVertex3d( -0.1, 0, 0 );
//         glVertex3d( -1.7, 0.5, 0 );
//         glVertex3d( -0.1, 0, 0 );
//         glVertex3d( -1.7, -0.5, 0 );
//         glVertex3d( -0.1, 0, 0 );
//         glVertex3d( -1.7, 0, 0.5 );
//         glVertex3d( -0.1, 0, 0 );
//         glVertex3d( -1.7, 0, -0.5 );       
//         glEnd();


// 	glPopMatrix();
// }

void MegamanModel::drawFace() {
	setDiffuseColor(0,0,0);
	glBegin( GL_LINE_STRIP );  
        glVertex3d(-1.1, -1.1, 2.05 );
        glVertex3d(-0.9, -1.35, 1.88 );
        glVertex3d(-0.45, -1.55, 1.84 );
        glVertex3d(0.1, -1.8, 1.8);  
        glVertex3d(0.45, -1.55, 1.84 );   
        glVertex3d(0.9, -1.35, 1.88 );
       
        glVertex3d(1.1, -1.1, 2.05 );
        // glVertex3d( -0.1, 0, 0 );
        // glVertex3d( -1.7, 0.5, 0 );
        // glVertex3d( -0.1, 0, 0 );
        // glVertex3d( -1.7, -0.5, 0 );
        // glVertex3d( -0.1, 0, 0 );
        // glVertex3d( -1.7, 0, 0.5 );
        // glVertex3d( -0.1, 0, 0 );
        // glVertex3d( -1.7, 0, -0.5 );       
        glEnd();

        glBegin( GL_LINE_STRIP );  
        glVertex3d(0, -0.2, 2.4 );   
        glVertex3d(0, -0.4, 2.36 );
        glEnd();
}

int main()
{
	// Initialize the controls
	// Constructor is ModelerControl(name, minimumvalue, maximumvalue, 
	// stepsize, defaultvalue)
    ModelerControl controls[NUMCONTROLS];
    controls[XPOS] = ModelerControl("X Position", -5, 5, 0.1f, 0);
    controls[YPOS] = ModelerControl("Y Position", 0, 5, 0.1f, 0);
    controls[ZPOS] = ModelerControl("Z Position", -5, 5, 0.1f, 0);
    controls[HEIGHT] = ModelerControl("Height", 1, 2.5, 0.1f, 1);
	controls[ROTATE] = ModelerControl("Rotate", -180, 180, 1, 0);
	controls[SEAT_ROTATE] = ModelerControl("Seat Rotate", -180, 180, 1, 0);
	controls[LIGHT_X] = ModelerControl("Light PosX", 0, 10, 0.1f, 5);
	controls[LIGHT_Y] = ModelerControl("Light PosY", 0, 10, 0.1f, 5);
	controls[LIGHT_Z] = ModelerControl("Light PosZ", 0, 10, 0.1f, 0);
	controls[HEAD_NOD] = ModelerControl("Head Nod", -45, 45, 1, 0);
	controls[HEAD_SHAKE] = ModelerControl("Head Shake", -90, 90, 1, 0);
	controls[HEAD_TILT] = ModelerControl("Head Tilt", -45, 45, 1, 0);
	controls[RIGHT_ARM_FLEX] = ModelerControl("Right Arm Flex", -30, 90, 1, 0);
	controls[RIGHT_ARM_SIDE_FLEX] = ModelerControl("Right Arm Side Flex", -30, 90, 1, 80);
	controls[RIGHT_FOREARM_FLEX] = ModelerControl("Right Forearm Flex", 0, 170, 1, 0);
	controls[LEFT_ARM_FLEX] = ModelerControl("Left Arm Flex", -30, 90, 1, 0);
	controls[LEFT_ARM_SIDE_FLEX] = ModelerControl("Left Arm Side Flex", -30, 90, 1, 80);
	controls[LEFT_FOREARM_FLEX] = ModelerControl("Left Fore Arm Flex", 0, 170, 1, 0);
	controls[RIGHT_LEG_FLEX] = ModelerControl("Right Leg Flex", -45, 45, 1, 0);
	controls[RIGHT_LEG_SIDE_FLEX] = ModelerControl("Right Leg Side Flex", -20, 80, 1, 0);
	controls[RIGHT_LOWER_LEG_FLEX] = ModelerControl("Right Lower Leg Flex", 0, 90, 1, 0);
	controls[RIGHT_FOOT_FLEX] = ModelerControl("Right Foot Flex", 0, 90, 1, 0);
	controls[LEFT_LEG_FLEX] = ModelerControl("Left Leg Flex", -45, 45, 1, 0);
	controls[LEFT_LEG_SIDE_FLEX] = ModelerControl("Left Leg Side Flex", -20, 80, 1, 0);
	controls[LEFT_LOWER_LEG_FLEX] = ModelerControl("Left Lower Leg Flex", 0, 90, 1, 0);
	controls[LEFT_FOOT_FLEX] = ModelerControl("Left Foot Flex", 0, 90, 1, 0);
	controls[MEGAMAN_TYPE] = ModelerControl("Change Megaman Type", 0, 1, 1, 0);
	controls[LV_DETAIL] = ModelerControl("Level of details", 0, 3, 1, 3);

    ModelerApplication::Instance()->Init(&createMegamanModel, controls, NUMCONTROLS);
    // ModelerApplication::Instance()->SetParticleSystem(ps)
    return ModelerApplication::Instance()->Run();
}

// This is a list of the controls for the RobotArm
// We'll use these constants to access the values 
// of the controls from the user interface.

// enum RobotArmControls
// { 
//     BASE_ROTATION=0, LOWER_TILT, UPPER_TILT, CLAW_ROTATION,
//         BASE_LENGTH, LOWER_LENGTH, UPPER_LENGTH, PARTICLE_COUNT, NUMCONTROLS, 
// };

// void ground(float h);
// void base(float h);
// void rotation_base(float h);
// void lower_arm(float h);
// void upper_arm(float h);
// void claw(float h);
// void y_box(float h);

// // To make a RobotArm, we inherit off of ModelerView
// class RobotArm : public ModelerView 
// {
// public:
//     RobotArm(int x, int y, int w, int h, char *label) 
//         : ModelerView(x,y,w,h,label) {}
//     virtual void draw();
// };

// // We need to make a creator function, mostly because of
// // nasty API stuff that we'd rather stay away from.
// ModelerView* createRobotArm(int x, int y, int w, int h, char *label)
// { 
//     return new RobotArm(x,y,w,h,label); 
// }

// // We'll be getting the instance of the application a lot; 
// // might as well have it as a macro.
// #define VAL(x) (ModelerApplication::Instance()->GetControlValue(x))





// // We are going to override (is that the right word?) the draw()
// // method of ModelerView to draw out RobotArm
// void RobotArm::draw()
// {
// 	/* pick up the slider values */

// 	float theta = VAL( BASE_ROTATION );
// 	float phi = VAL( LOWER_TILT );
// 	float psi = VAL( UPPER_TILT );
// 	float cr = VAL( CLAW_ROTATION );
// 	float h1 = VAL( BASE_LENGTH );
// 	float h2 = VAL( LOWER_LENGTH );
// 	float h3 = VAL( UPPER_LENGTH );
// 	float pc = VAL( PARTICLE_COUNT );


//     // This call takes care of a lot of the nasty projection 
//     // matrix stuff
//     ModelerView::draw();

// 	static GLfloat lmodel_ambient[] = {0.4,0.4,0.4,1.0};

// 	// define the model

// 	ground(-0.2);

// 	base(0.8);

//     glTranslatef( 0.0, 0.8, 0.0 );			// move to the top of the base
//     glRotatef( theta, 0.0, 1.0, 0.0 );		// turn the whole assembly around the y-axis. 
// 	rotation_base(h1);						// draw the rotation base

//     glTranslatef( 0.0, h1, 0.0 );			// move to the top of the base
//     glRotatef( phi, 0.0, 0.0, 1.0 );		// rotate around the z-axis for the lower arm
// 	glTranslatef( -0.1, 0.0, 0.4 );
// 	lower_arm(h2);							// draw the lower arm

//     glTranslatef( 0.0, h2, 0.0 );			// move to the top of the lower arm
//     glRotatef( psi, 0.0, 0.0, 1.0 );		// rotate  around z-axis for the upper arm
// 	upper_arm(h3);							// draw the upper arm

// 	glTranslatef( 0.0, h3, 0.0 );
// 	glRotatef( cr, 0.0, 0.0, 1.0 );
// 	claw(1.0);

// 	//*** DON'T FORGET TO PUT THIS IN YOUR OWN CODE **/
// 	endDraw();
// }

// void ground(float h) 
// {
// 	glDisable(GL_LIGHTING);
// 	glColor3f(0.65,0.45,0.2);
// 	glPushMatrix();
// 	glScalef(30,0,30);
// 	y_box(h);
// 	glPopMatrix();
// 	glEnable(GL_LIGHTING);
// }

// void base(float h) {
// 	setDiffuseColor( 0.25, 0.25, 0.25 );
// 	setAmbientColor( 0.25, 0.25, 0.25 );
// 	glPushMatrix();
// 		glPushMatrix();
// 			glTranslatef(1.0, h / 2.0, 0.75);
// 			drawCylinder(0.25, h / 2.0, h / 2.0);
// 		glPopMatrix();
// 		glPushMatrix();
// 			glTranslatef(1.0, h / 2.0, -1.0);
// 			drawCylinder(0.25, h / 2.0, h / 2.0);
// 		glPopMatrix();
// 		glPushMatrix();
// 			glTranslatef(-1.0, h / 2.0, 0.75);
// 			drawCylinder(0.25, h / 2.0, h / 2.0);
// 		glPopMatrix();
// 		glPushMatrix();
// 			glTranslatef(-1.0, h / 2.0, -1.0);
// 			drawCylinder(0.25, h / 2.0, h / 2.0);
// 		glPopMatrix();
// 	glScalef(4.0f, h, 4.0f);
// 	y_box(1.0f);
// 	glPopMatrix();
// }

// void rotation_base(float h) {
// 	setDiffuseColor( 0.85, 0.75, 0.25 );
// 	setAmbientColor( 0.95, 0.75, 0.25 );
// 	glPushMatrix();
// 		glPushMatrix();
// 			glScalef(4.0, h, 4.0);
// 			y_box(1.0f); // the rotation base
// 		glPopMatrix();
// 		setDiffuseColor( 0.15, 0.15, 0.65 );
// 		setAmbientColor( 0.15, 0.15, 0.65 );
// 		glPushMatrix();
// 			glTranslatef(-0.5, h, -0.6);
// 			glScalef(2.0, h, 1.6);
// 			y_box(1.0f); // the console
// 		glPopMatrix();
// 		setDiffuseColor( 0.65, 0.65, 0.65 );
// 		setAmbientColor( 0.65, 0.65, 0.65 );
// 		glPushMatrix();
// 			glTranslatef( 0.5, h, 0.6 );
// 			glRotatef( -90.0, 1.0, 0.0, 0.0 );
// 			drawCylinder( h, 0.05, 0.05 ); // the pipe
// 		glPopMatrix();
// 	glPopMatrix();
// }

// void lower_arm(float h) {					// draw the lower arm
// 	setDiffuseColor( 0.85, 0.75, 0.25 );
// 	setAmbientColor( 0.95, 0.75, 0.25 );
// 	y_box(h);
// }

// void upper_arm(float h) {					// draw the upper arm
// 	setDiffuseColor( 0.85, 0.75, 0.25 );
// 	setAmbientColor( 0.95, 0.75, 0.25 );
// 	glPushMatrix();
// 	glScalef( 1.0, 1.0, 0.7 );
// 	y_box(h);
// 	glPopMatrix();
// }

// void claw(float h) {
// 	setDiffuseColor( 0.25, 0.25, 0.85 );
// 	setAmbientColor( 0.25, 0.25, 0.85 );

// 	glBegin( GL_TRIANGLES );

// 	glNormal3d( 0.0, 0.0, 1.0);		// +z side
// 	glVertex3d( 0.5, 0.0, 0.5);
// 	glVertex3d(-0.5, 0.0, 0.5);
// 	glVertex3d( 0.5,   h, 0.5);

// 	glNormal3d( 0.0, 0.0, -1.0);	// -z side
// 	glVertex3d( 0.5, 0.0, -0.5);
// 	glVertex3d(-0.5, 0.0, -0.5);
// 	glVertex3d( 0.5,   h, -0.5);

// 	glEnd();

// 	glBegin( GL_QUADS );

// 	glNormal3d( 1.0,  0.0,  0.0);	// +x side
// 	glVertex3d( 0.5, 0.0,-0.5);
// 	glVertex3d( 0.5, 0.0, 0.5);
// 	glVertex3d( 0.5,   h, 0.5);
// 	glVertex3d( 0.5,   h,-0.5);

// 	glNormal3d( 0.0,-1.0, 0.0);		// -y side
// 	glVertex3d( 0.5, 0.0, 0.5);
// 	glVertex3d( 0.5, 0.0,-0.5);
// 	glVertex3d(-0.5, 0.0,-0.5);
// 	glVertex3d(-0.5, 0.0, 0.5);

// 	glEnd();
// }

// void y_box(float h) {

// 	glBegin( GL_QUADS );

// 	glNormal3d( 1.0 ,0.0, 0.0);			// +x side
// 	glVertex3d( 0.25,0.0, 0.25);
// 	glVertex3d( 0.25,0.0,-0.25);
// 	glVertex3d( 0.25,  h,-0.25);
// 	glVertex3d( 0.25,  h, 0.25);

// 	glNormal3d( 0.0 ,0.0, -1.0);		// -z side
// 	glVertex3d( 0.25,0.0,-0.25);
// 	glVertex3d(-0.25,0.0,-0.25);
// 	glVertex3d(-0.25,  h,-0.25);
// 	glVertex3d( 0.25,  h,-0.25);

// 	glNormal3d(-1.0, 0.0, 0.0);			// -x side
// 	glVertex3d(-0.25,0.0,-0.25);
// 	glVertex3d(-0.25,0.0, 0.25);
// 	glVertex3d(-0.25,  h, 0.25);
// 	glVertex3d(-0.25,  h,-0.25);

// 	glNormal3d( 0.0, 0.0, 1.0);			// +z side
// 	glVertex3d(-0.25,0.0, 0.25);
// 	glVertex3d( 0.25,0.0, 0.25);
// 	glVertex3d( 0.25,  h, 0.25);
// 	glVertex3d(-0.25,  h, 0.25);

// 	glNormal3d( 0.0, 1.0, 0.0);			// top (+y)
// 	glVertex3d( 0.25,  h, 0.25);
// 	glVertex3d( 0.25,  h,-0.25);
// 	glVertex3d(-0.25,  h,-0.25);
// 	glVertex3d(-0.25,  h, 0.25);

// 	glNormal3d( 0.0,-1.0, 0.0);			// bottom (-y)
// 	glVertex3d( 0.25,0.0, 0.25);
// 	glVertex3d(-0.25,0.0, 0.25);
// 	glVertex3d(-0.25,0.0,-0.25);
// 	glVertex3d( 0.25,0.0,-0.25);

// 	glEnd();
// }

// int main()
// {
//     ModelerControl controls[NUMCONTROLS ];

// 	controls[BASE_ROTATION] = ModelerControl("base rotation (theta)", -180.0, 180.0, 0.1, 0.0 );
//     controls[LOWER_TILT] = ModelerControl("lower arm tilt (phi)", 15.0, 95.0, 0.1, 55.0 );
//     controls[UPPER_TILT] = ModelerControl("upper arm tilt (psi)", 0.0, 135.0, 0.1, 30.0 );
// 	controls[CLAW_ROTATION] = ModelerControl("claw rotation (cr)", -30.0, 180.0, 0.1, 0.0 );
//     controls[BASE_LENGTH] = ModelerControl("base height (h1)", 0.5, 10.0, 0.1, 0.8 );
//     controls[LOWER_LENGTH] = ModelerControl("lower arm length (h2)", 1, 10.0, 0.1, 3.0 );
//     controls[UPPER_LENGTH] = ModelerControl("upper arm length (h3)", 1, 10.0, 0.1, 2.5 );
//     controls[PARTICLE_COUNT] = ModelerControl("particle count (pc)", 0.0, 5.0, 0.1, 5.0 );
    
// 	// You should create a ParticleSystem object ps here and then
// 	ModelerApplication::Instance()->SetParticleSystem(ps)
// 	// to hook it up to the animator interface.

//     ModelerApplication::Instance()->Init(&createRobotArm, controls, NUMCONTROLS);

//     return ModelerApplication::Instance()->Run();
// }










