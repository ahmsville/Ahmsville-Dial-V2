#include "AhmsvilleDial_v2.h"



/*


....By Ahmed Oyenuga (Ahmsville 2019).
*/


MagRotaryEncoder AhmsvilleDial_Knob = MagRotaryEncoder(A1, A0, 38, 27); // create new encoding instance and specify the arduino pins connected to the hall effect sensors

#if DIAL_VERSION == 3 || DIAL_VERSION == 4
MagRotaryEncoder AhmsvilleDial_Knob2 = MagRotaryEncoder(A2, A3, 42, 13);
#endif


volatile bool activesensorInterrupt = 0, knob2activesensorInterrupt = 0;
void ISR1() {
	if (activesensorInterrupt == 0) { //sensor 1 interrupt is active.
		AhmsvilleDial_Knob.sensor1_INT();
		activesensorInterrupt = !activesensorInterrupt;
	}
}
void ISR2() {
	if (activesensorInterrupt == 1) { //sensor 1 interrupt is active.
		AhmsvilleDial_Knob.sensor2_INT();
		activesensorInterrupt = !activesensorInterrupt;
	}
}
#if DIAL_VERSION == 3 || DIAL_VERSION == 4
void knob2ISR1() {
	if (knob2activesensorInterrupt == 0) { //sensor 1 interrupt is active.
		AhmsvilleDial_Knob2.sensor1_INT();
		knob2activesensorInterrupt = !knob2activesensorInterrupt;
	}
}
void knob2ISR2() {
	if (knob2activesensorInterrupt == 1) { //sensor 1 interrupt is active.
		AhmsvilleDial_Knob2.sensor2_INT();
		knob2activesensorInterrupt = !knob2activesensorInterrupt;
	}
}
#endif
void setknobinterrupt() {
	pinMode(AhmsvilleDial_Knob.get_sensorINTpin(1), INPUT);
	attachInterrupt(digitalPinToInterrupt(AhmsvilleDial_Knob.get_sensorINTpin(1)), ISR1, RISING);

	pinMode(AhmsvilleDial_Knob.get_sensorINTpin(2), INPUT);
	attachInterrupt(digitalPinToInterrupt(AhmsvilleDial_Knob.get_sensorINTpin(2)), ISR2, RISING);

#if DIAL_VERSION == 3 || DIAL_VERSION == 4
	pinMode(AhmsvilleDial_Knob2.get_sensorINTpin(1), INPUT);
	attachInterrupt(digitalPinToInterrupt(AhmsvilleDial_Knob2.get_sensorINTpin(1)), knob2ISR1, RISING);

	pinMode(AhmsvilleDial_Knob2.get_sensorINTpin(2), INPUT);
	attachInterrupt(digitalPinToInterrupt(AhmsvilleDial_Knob2.get_sensorINTpin(2)), knob2ISR2, RISING);
#endif // DIAL_VERSION == 3 || DIAL_VERSION == 4

	
}


AhmsvilleDial::AhmsvilleDial() {
	pinMode(30, OUTPUT);
digitalWrite(30, LOW); 
delay(500);
}

void AhmsvilleDial::initialize_ahmsvilleDial() {
	setknobinterrupt();

	set_haptics(4, 60, 10, 10, 150);
   
	AhmsvilleDial_Knob.initialize_encoder();

#if DIAL_VERSION == 3 || DIAL_VERSION == 4
	
	AhmsvilleDial_Knob2.initialize_encoder();

	AhmsvilleDial_Spacenav.set_XYSensorPins(A5, A4);
	AhmsvilleDial_Spacenav.set_adaptiveSpaceNavigation(true);
	AhmsvilleDial_Spacenav.set_slideBoundaryRadii(30.0, 3);
	AhmsvilleDial_Spacenav.set_tiltBoundaryRadii(30.0 ,3);
	
	AhmsvilleDial_Spacenav.initialize_spaceNav();
#endif // DIAL_VERSION == 3 || DIAL_VERSION == 4

	AhmsvilleDial_Touch.set_capTouchPins(9, 8, 0, 0, 0);   //sets the arduino pins used for the capacitive touch (sendpin, pad1, pad2, pad3, pad4)
	//AhmsvilleDial_Touch.set_inputTypeThresholds(300, 320, 800, 250);  //sets the thresholds for the four input types  (singletap, shortpress, longpress, doubletapspeed)
	//AhmsvilleDial_Touch.set_adaptiveSensitivity(1, 0.4, true);  //set touch sensitivity to auto
	AhmsvilleDial_Touch.initialize_capTouch(1);

#if DIAL_VERSION == 2 || DIAL_VERSION == 4
	AhmsvilleDial_Mkey.initialize_macroKey();
#endif // DIAL_VERSION == 2 || DIAL_VERSION == 4

	
	
	Keyboard.begin();
	Mouse.begin();
}

void AhmsvilleDial::initialize_ahmsvilleDialCustom(int touchpin) {

}

void AhmsvilleDial::set_inputTypeThresholds(int st, int sp, int lp, int dt) {
	AhmsvilleDial_Touch.set_inputTypeThresholds(st,sp,lp,dt);
}

void AhmsvilleDial::set_haptics(int pin, int durationtouch, int durationknob, int durationspacenav, int strength) {
	AhmsvilleDial_Touch.set_haptics(pin, durationtouch, 255);
	AhmsvilleDial_Knob.set_haptics(pin, durationknob, strength);

#if DIAL_VERSION == 3 || DIAL_VERSION == 4
	AhmsvilleDial_Knob2.set_haptics(pin, durationknob, strength);
	AhmsvilleDial_Spacenav.set_haptics(pin, durationspacenav, 75);
#endif // DIAL_VERSION == 3 || DIAL_VERSION == 4
	
}

int AhmsvilleDial::capTouch(int pad) {
	int touch = AhmsvilleDial_Touch.detect_touchFromNoise(pad);
	return touch;
}

int AhmsvilleDial::capTouch() {
	int touch = AhmsvilleDial_Touch.detect_touchFromNoise(0);
	return touch;
}

int AhmsvilleDial::capTouch_single() {
	int touch = AhmsvilleDial_Touch.detect_touchFromNoise_single(0);
	return touch;
}

int AhmsvilleDial::capTouch_single(int pad) {
	int touch = AhmsvilleDial_Touch.detect_touchFromNoise_single(pad);
	return touch;
}

int AhmsvilleDial::knob() {
	int knob = 0;
	if (knob1setres == 0) {
		knob = AhmsvilleDial_Knob.detect_rotationWithRate();
	}
	else {
		knob = AhmsvilleDial_Knob.detect_rotationHR();
	}
	
	if (knob != 0) {
		AhmsvilleDial_Knob.setsleep(false);
	}

	return knob;
}


void AhmsvilleDial::normalize() {
	
	//AhmsvilleDial_Touch.update_basevalueFromNoise(0);
	//AhmsvilleDial_Knob.recaliberate_startPosition();
	
	
#if DIAL_VERSION == 2 || DIAL_VERSION == 4
	AhmsvilleDial_Mkey.reset_macroKey();
#endif // 0	

#if DIAL_VERSION == 3 || DIAL_VERSION == 4
	//AhmsvilleDial_Knob2.recaliberate_startPosition();
	
	AhmsvilleDial_Spacenav.reset_spaceNav();
#endif // DIAL_VERSION == 3 || DIAL_VERSION == 4
}

void AhmsvilleDial::setknobresolution(int res_percent, int knobnum) {
	if (knobnum == 1) {
		AhmsvilleDial_Knob.set_resolution(res_percent);
		knob1setres = res_percent;
	}
	else if (knobnum = 2) {
#if DIAL_VERSION == 3 || DIAL_VERSION == 4
		AhmsvilleDial_Knob2.set_resolution(res_percent);
		knob2setres = res_percent;
#endif // DIAL_VERSION == 3 || DIAL_VERSION == 4
	}
	
}

void AhmsvilleDial::normalizeCustom(int pad) {
	AhmsvilleDial_Knob.recaliberate_startPosition();
	AhmsvilleDial_Touch.update_basevalueFromNoise(pad);
}

int AhmsvilleDial::get_knobState(int sensornum) {
	int state = AhmsvilleDial_Knob.get_currentSensorValue(sensornum);
	return state;
}


#if DIAL_VERSION == 3 || DIAL_VERSION == 4
int AhmsvilleDial::knob2() {
	int knob2 = 0;
	if (knob2setres == 0) {
		knob2 = AhmsvilleDial_Knob2.detect_rotationWithRate();
	}
	else {
		knob2 = AhmsvilleDial_Knob2.detect_rotationHR();
	}

	return knob2;
}

float *AhmsvilleDial::spaceNav_raw() {
	gyro_x_y_radius = AhmsvilleDial_Spacenav.get_gyroCoordinates();
	plane_x_y_radius = AhmsvilleDial_Spacenav.get_planarCoordinates();

	gyro_and_plane_x_y_radius[0] = gyro_x_y_radius[0];
	gyro_and_plane_x_y_radius[1] = gyro_x_y_radius[1];
	gyro_and_plane_x_y_radius[2] = gyro_x_y_radius[2];
	gyro_and_plane_x_y_radius[3] = plane_x_y_radius[0];
	gyro_and_plane_x_y_radius[4] = plane_x_y_radius[1];
	gyro_and_plane_x_y_radius[5] = plane_x_y_radius[2];
	
	return gyro_and_plane_x_y_radius;
}

bool *AhmsvilleDial::spaceNav_tilt() {
	tilt = AhmsvilleDial_Spacenav.get_Tilt();
	return tilt;
}
bool* AhmsvilleDial::spaceNav_slide() {
	slide = AhmsvilleDial_Spacenav.get_Slide();
	return slide;
}
#endif // DIAL_VERSION == 3 || DIAL_VERSION == 4

#if DIAL_VERSION == 2 || DIAL_VERSION == 4
int *AhmsvilleDial::Mkey() {
	mkeystate = AhmsvilleDial_Mkey.get_keyPress();

	return mkeystate;

	
}
#endif // DIAL_VERSION == 2 || DIAL_VERSION == 4

void AhmsvilleDial::setsleep(bool slpact) {
	
	AhmsvilleDial_Knob.setsleep(slpact);

}

bool AhmsvilleDial::readsleep() {
	return AhmsvilleDial_Knob.readsleep();
}