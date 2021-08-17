
#ifndef AhmsvilleDial_v2_h
#define AhmsvilleDial_v2_h


#define DIAL_VERSION 4 //v2 Samd21 Absolute


//#define DIAL_VERSION 0 //V1 atmega32u4
//#define DIAL_VERSION 1 //v2 Samd21 Base
//#define DIAL_VERSION 2 //v2 Samd21 Macro
//#define DIAL_VERSION 3 //v2 Samd21 SpaceNav
//#define DIAL_VERSION 4 //v2 Samd21 Absolute


#ifdef DIAL_VERSION 
#if DIAL_VERSION == 1 
#include "Arduino.h"
#include <AdvCapTouch.h>
#include <MagRotaryEncoding.h>
#include "Keyboard.h"
#include "Mouse.h"

#elif DIAL_VERSION == 2 
#include "Arduino.h"
#include <AdvCapTouch.h>
#include <MagRotaryEncoding.h>
#include <MacroKey.h>
#include "Keyboard.h"
#include "Mouse.h"

#elif DIAL_VERSION == 3
#include "Arduino.h"
#include <AdvCapTouch.h>
#include <MagRotaryEncoding.h>
#include <SpaceNavigator.h>
#include "Keyboard.h"
#include "Mouse.h"

#elif DIAL_VERSION == 4 
#include "Arduino.h"
#include <AdvCapTouch.h>
#include <MagRotaryEncoding.h>
#include <SpaceNavigator.h>
#include <MacroKey.h>
#include "Keyboard.h"
#include "Mouse.h"

#elif DIAL_VERSION == 0 
#include "Arduino.h"
#include <AdvCapTouch.h>
#include <MagRotaryEncoding.h>
#include "Keyboard.h"
#include "Mouse.h"

#endif
#endif // DIAL_VERSION  




class AhmsvilleDial {
private:

#if DIAL_VERSION == 3 || DIAL_VERSION == 4
	
	SpaceNavigator AhmsvilleDial_Spacenav;//create a new spacenav instance
	float gyro_and_plane_x_y_radius[6];
	float* gyro_x_y_radius;
	float* plane_x_y_radius;
	//bool tilt_and_slide[2];
	bool* tilt;
	bool* slide;

	int knob2setres = 0;
#endif // 0

#if DIAL_VERSION == 2 || DIAL_VERSION == 4
	MacroKey AhmsvilleDial_Mkey; //create a new macrokey instance
	int* mkeystate;
#endif // DIAL_VERSION == 2 || DIAL_VERSION == 4

	//AdvCapTouch AhmsvilleDial_Touch = AdvCapTouch(true);  //create a new captouch object
	AdvCapTouch AhmsvilleDial_Touch;  //create a new captouch instance
	int knobcount = 0, knob2count = 0;
	int knob1setres = 0; 

public:
	AhmsvilleDial();

#if DIAL_VERSION == 3 || DIAL_VERSION == 4
	int knob2();

	float* spaceNav_raw();
	bool* spaceNav_tilt();
	bool* spaceNav_slide();
#endif // DIAL_VERSION == 3 || DIAL_VERSION == 4

#if DIAL_VERSION == 2 || DIAL_VERSION == 4
	int* Mkey();
#endif // DIAL_VERSION == 2 || DIAL_VERSION == 4


	void set_inputTypeThresholds(int st, int sp, int lp, int dt);   //set capacitive touch sensitivity in the form of detection, rejection thresholds values
	void initialize_ahmsvilleDial();  //initialize ahmsville dial
	void initialize_ahmsvilleDialCustom(int touchpin);  //initialize ahmsville dial with custom settings
	void set_haptics(int pin, int durationtouch, int durationknob, int durationspacenav, int strength);  //set arduino haptics variables (arduino pwm pin connected to viberation motor, duration of haptics(ms), strength from 0-255)
	
	/*---------------------------------------------------------Dial Inputs functions (capacitive touch & Knob rotation)-----------------------------------------------------*/

	int capTouch(int pad);   //detect capacitive touch from specified pad
	int capTouch();   //detect capacitive touch
	int capTouch_single(int pad); //detect single tap alone from specified pad
	int capTouch_single();  //detect single tap alone
	int knob();    //detect rotation
	
	void capTouchInterrupt();
	

	/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

	void setknobresolution(int res_percent, int knobnum);
	void normalize();
	void normalizeCustom(int pad);
	int get_knobState(int sensornum);
	void setsleep(bool slpact);
	bool readsleep();

	

};








#endif // !AhmsvilleDial_v2_h

