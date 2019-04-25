#ifndef _MODELER_GLOBALS_H
#define _MODELER_GLOBALS_H

#ifndef M_PI
#define M_PI 3.141592653589793238462643383279502
#endif

// This is a list of the controls for the SampleModel
// We'll use these constants to access the values 
// of the controls from the user interface.
enum SampleModelControls
{ 
	XPOS, 
	YPOS, 
	ZPOS,
	HEIGHT, 
	ROTATE, 
	SEAT_ROTATE,
	LIGHT_X,
	LIGHT_Y,
	LIGHT_Z,
	HEAD_NOD,
	HEAD_SHAKE,
	HEAD_TILT,
	RIGHT_ARM_FLEX, 
	RIGHT_ARM_SIDE_FLEX,
	RIGHT_FOREARM_FLEX,
	LEFT_ARM_FLEX,
	LEFT_ARM_SIDE_FLEX,
	LEFT_FOREARM_FLEX,
	RIGHT_LEG_FLEX,
	RIGHT_LEG_SIDE_FLEX,
	RIGHT_LOWER_LEG_FLEX,
	RIGHT_FOOT_FLEX,
	LEFT_LEG_FLEX,
	LEFT_LEG_SIDE_FLEX,
	LEFT_LOWER_LEG_FLEX,
	LEFT_FOOT_FLEX,
	MEGAMAN_TYPE,
	LV_DETAIL,
	NUMCONTROLS,
};

// Colors
#define COLOR_RED			1.0f, 0.0f, 0.0f
#define COLOR_GREEN		0.0f, 1.0f, 0.0f
#define COLOR_BLUE			0.0f, 0.0f, 1.0f
#define COLOR_GOLD			1.0f, 1.0f, 0.0f

// We'll be getting the instance of the application a lot; 
// might as well have it as a macro.
#define VAL(x) (ModelerApplication::Instance()->GetControlValue(x))

#endif