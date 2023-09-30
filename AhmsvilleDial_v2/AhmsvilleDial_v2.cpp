#include "AhmsvilleDial_v2.h"



/*


....By Ahmed Oyenuga (Ahmsville 2019).
*/


//MagRotaryEncoder AhmsvilleDial_Knob = MagRotaryEncoder(A1, A0, 38, 27); // create new encoding instance and specify the arduino pins connected to the hall effect sensors
MagRotaryEncoder AhmsvilleDial_Knob = MagRotaryEncoder(A1, A0);
#if DIAL_VERSION == 3 || DIAL_VERSION == 4
//MagRotaryEncoder AhmsvilleDial_Knob2 = MagRotaryEncoder(A2, A3, 42, 13);
MagRotaryEncoder AhmsvilleDial_Knob2 = MagRotaryEncoder(A2, A3);

#endif


volatile int activesensorInterrupt = 1, knob2activesensorInterrupt = 1;


void mainISR1() {

	AhmsvilleDial_Knob.singleSensor_INT(1);
}
#if DIAL_VERSION == 3 || DIAL_VERSION == 4
void mainISR2() {

	AhmsvilleDial_Knob2.singleSensor_INT(1);
}

#endif
void setknobinterrupt() {
	pinMode(AhmsvilleDial_Knob.get_sensorINTpin(1), INPUT);
	attachInterrupt(digitalPinToInterrupt(AhmsvilleDial_Knob.get_sensorINTpin(1)), mainISR1, RISING);

	pinMode(AhmsvilleDial_Knob.get_sensorINTpin(2), INPUT);
	attachInterrupt(digitalPinToInterrupt(AhmsvilleDial_Knob.get_sensorINTpin(2)), mainISR1, RISING);

#if DIAL_VERSION == 3 || DIAL_VERSION == 4
	pinMode(AhmsvilleDial_Knob2.get_sensorINTpin(1), INPUT);
	attachInterrupt(digitalPinToInterrupt(AhmsvilleDial_Knob2.get_sensorINTpin(1)), mainISR2, RISING);

	pinMode(AhmsvilleDial_Knob2.get_sensorINTpin(2), INPUT);
	attachInterrupt(digitalPinToInterrupt(AhmsvilleDial_Knob2.get_sensorINTpin(2)), mainISR2, RISING);
#endif // DIAL_VERSION == 3 || DIAL_VERSION == 4


}


AhmsvilleDial::AhmsvilleDial() {
	pinMode(30, OUTPUT);
	digitalWrite(30, LOW);
	delay(500);
}

void AhmsvilleDial::resetdial() {

	//AhmsvilleDial_Touch = AdvCapTouch();
	//AhmsvilleDial_Knob = MagRotaryEncoder(A1, A0, 38, 27);
#if DIAL_VERSION == 3 || DIAL_VERSION == 4
	//AhmsvilleDial_Knob2 = MagRotaryEncoder(A2, A3, 42, 13);
	AhmsvilleDial_Spacenav.softreset();
	if(gyro_x_y_radius[2] > 0.0){
          AhmsvilleDial_Spacenav.set_tiltBoundaryRadii(100, gyro_x_y_radius[2]+0.5);
    }
    if(plane_x_y_radius[2] > 0.0){
        AhmsvilleDial_Spacenav.set_slideBoundaryRadii(100, plane_x_y_radius[2]+0.5);
    }
	/*AhmsvilleDial_Spacenav.set_XYSensorPins(A5, A4);
	AhmsvilleDial_Spacenav.set_adaptiveSpaceNavigation(true);
	AhmsvilleDial_Spacenav.set_slideBoundaryRadii(100, 0);
	AhmsvilleDial_Spacenav.set_tiltBoundaryRadii(100, 0);
	AhmsvilleDial_Spacenav.initialize_spaceNav();*/
#endif // DIAL_VERSION == 3 || DIAL_VERSION == 4

#if DIAL_VERSION == 2 || DIAL_VERSION == 4
	//AhmsvilleDial_Mkey = MacroKey();
#endif // DIAL_VERSION == 2 || DIAL_VERSION == 4

}
void AhmsvilleDial::initialize_ahmsvilleDial() {
	//setknobinterrupt();
	set_haptics(4, 60, 20, 5, 200); //int pin, int durationtouch, int durationknob, int durationspacenav, int strength

	analogReadResolution(12);
	setknobresolution(0, 1,false);


#if DIAL_VERSION == 3 || DIAL_VERSION == 4

	setknobresolution(0,2,false);

	AhmsvilleDial_Spacenav.set_XYSensorPins(A5, A4);
	AhmsvilleDial_Spacenav.set_adaptiveSpaceNavigation(false);
	AhmsvilleDial_Spacenav.set_slideBoundaryRadii(100, 0);
	AhmsvilleDial_Spacenav.set_tiltBoundaryRadii(100, 0);

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
	AhmsvilleDial_Touch.set_inputTypeThresholds(st, sp, lp, dt);
}

void AhmsvilleDial::set_haptics(int pin, int durationtouch, int durationknob, int durationspacenav, int strength) {
	AhmsvilleDial_Touch.set_haptics(pin, durationtouch, 255);
	AhmsvilleDial_Knob.set_haptics(pin, durationknob, strength);
	haptics_pin = pin;

#if DIAL_VERSION == 3 || DIAL_VERSION == 4
	AhmsvilleDial_Knob2.set_haptics(pin, durationknob, 255);
	AhmsvilleDial_Spacenav.set_haptics(pin, durationspacenav, 75);
#endif // DIAL_VERSION == 3 || DIAL_VERSION == 4

}

int AhmsvilleDial::capTouch(int pad) {

	int touch = AhmsvilleDial_Touch.detect_touchFromNoise(pad);
	return touch;


}

void AhmsvilleDial::capTouchInterrupt() {

	AhmsvilleDial_Touch.read_valueFromNoise_interrupt();

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
	knobcount = 0;
	if (knob1setres > 0)
	{
		knobcount = int(AhmsvilleDial_Knob.detect_rotationHR());
	}
	else {
		knobcount = AhmsvilleDial_Knob.detect_rotation();
	}
	if (knobcount != 0) {
		AhmsvilleDial_Knob.setsleep(false);
	}
	//SerialUSB.println(knobcount);
	return knobcount;
}

float AhmsvilleDial::knobAbsolute() {
	float knobfloatcount = 0;
	if (knob1setres == 0) {
		knobfloatcount = float(AhmsvilleDial_Knob.detect_rotation());
	}
	else {
		knobfloatcount = AhmsvilleDial_Knob.detect_rotationHR();
	}

	if (knobfloatcount != 0) {
		AhmsvilleDial_Knob.setsleep(false);
	}

	return knobfloatcount;
}


void AhmsvilleDial::normalize() {

	//AhmsvilleDial_Touch.update_basevalueFromNoise(0);
	//AhmsvilleDial_Knob.recaliberate_startPosition();
	eventnotification(0);

#if DIAL_VERSION == 2 || DIAL_VERSION == 4
	//AhmsvilleDial_Mkey.reset_macroKey();
#endif // 0	

#if DIAL_VERSION == 3 || DIAL_VERSION == 4
	//AhmsvilleDial_Knob2.recaliberate_startPosition();

	AhmsvilleDial_Spacenav.reset_spaceNav();
#endif // DIAL_VERSION == 3 || DIAL_VERSION == 4
}

void AhmsvilleDial::setknobresolution(int res_percent, int knobnum, bool abs) {
	if (knobnum == 1) {
		
		if (abs)
		{
			AhmsvilleDial_Knob.set_adcresolution(12);
			AhmsvilleDial_Knob.setExtendedAbsolutePositioning(true);
			AhmsvilleDial_Knob.set_resolution(res_percent);
			AhmsvilleDial_Knob.set_encoderResolution(80);
			AhmsvilleDial_Knob.set_bound(100);
			AhmsvilleDial_Knob.initialize_encoder();
			AhmsvilleDial_Knob.setToStart();
		}
		else {	
			if (res_percent > 0)
			{
				AhmsvilleDial_Knob.set_adcresolution(12);
				AhmsvilleDial_Knob.setExtendedAbsolutePositioning(false);
				AhmsvilleDial_Knob.set_resolution(res_percent);
				AhmsvilleDial_Knob.set_encoderResolution(80);
				AhmsvilleDial_Knob.set_bound(100);
				AhmsvilleDial_Knob.initialize_encoder();
				AhmsvilleDial_Knob.setToStart();
			}
			else {
				AhmsvilleDial_Knob.set_adcresolution(12);
				AhmsvilleDial_Knob.setExtendedAbsolutePositioning(false);
				AhmsvilleDial_Knob.set_encoderResolution(0);
				AhmsvilleDial_Knob.set_bound(150);
				AhmsvilleDial_Knob.set_resolution(0);
				AhmsvilleDial_Knob.initialize_encoder();
				
			}
		}
		knob1setres = res_percent;
	}
	else if (knobnum == 2) {
#if DIAL_VERSION == 3 || DIAL_VERSION == 4
		if (abs)
		{
			AhmsvilleDial_Knob2.set_bound(100);
			AhmsvilleDial_Knob2.setExtendedAbsolutePositioning(true);
			AhmsvilleDial_Knob2.set_encoderResolution(100);
			AhmsvilleDial_Knob2.initialize_encoder();
			AhmsvilleDial_Knob2.setToStart();
		}
		else {
			if (res_percent > 0)
			{
				AhmsvilleDial_Knob2.set_adcresolution(12);
				AhmsvilleDial_Knob2.setExtendedAbsolutePositioning(false);
				AhmsvilleDial_Knob2.set_resolution(res_percent);
				AhmsvilleDial_Knob2.set_encoderResolution(100);
				AhmsvilleDial_Knob2.set_bound(100);
				AhmsvilleDial_Knob2.initialize_encoder();
				AhmsvilleDial_Knob2.setToStart();
			}
			else {
				AhmsvilleDial_Knob2.set_adcresolution(12);
				AhmsvilleDial_Knob2.setExtendedAbsolutePositioning(false);
				AhmsvilleDial_Knob2.set_encoderResolution(0);
				AhmsvilleDial_Knob2.set_bound(150);
				AhmsvilleDial_Knob2.set_resolution(0);
				AhmsvilleDial_Knob2.initialize_encoder();

			}
		}
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
	knob2count = 0;
	if (knob2setres > 0)
	{
		knob2count = int(AhmsvilleDial_Knob2.detect_rotationHR());
	}
	else {
		knob2count = AhmsvilleDial_Knob2.detect_rotation();
	}
	if (knob2count != 0) {
		AhmsvilleDial_Knob2.setsleep(false);
	}

	return knob2count;
}

float* AhmsvilleDial::spaceNav_raw() {
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

bool* AhmsvilleDial::spaceNav_tilt() {
	tilt = AhmsvilleDial_Spacenav.get_Tilt();
	return tilt;
}
bool* AhmsvilleDial::spaceNav_slide() {
	slide = AhmsvilleDial_Spacenav.get_Slide();
	return slide;
}
#endif // DIAL_VERSION == 3 || DIAL_VERSION == 4

#if DIAL_VERSION == 2 || DIAL_VERSION == 4
int* AhmsvilleDial::Mkey() {
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

void AhmsvilleDial::eventnotification(int state) {
	if (state == 1) {
		analogWrite(haptics_pin, haptics_strength);
		haptics_ontime = millis();
		haptics_state = 1;
	}
	else if (state == 3) {
		analogWrite(haptics_pin, (haptics_strength * 0.7));
	}
	else {
		haptics_offtime = millis();
		if (((haptics_offtime - haptics_ontime) >= haptics_duration) && haptics_state == 1) {
			analogWrite(haptics_pin, 0);
			haptics_state = 0;
		}

	}
}