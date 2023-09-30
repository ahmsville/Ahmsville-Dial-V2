
#include "SpaceNavigator.h"


/*

Copyright (c) Ahmed Oyenuga (Ahmsville 2020).

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.


*/

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high

void dmpDataReady() {
	mpuInterrupt = true;
}
void setinterupt() {
	pinMode(INTERRUPT_PIN, INPUT);
	digitalPinToInterrupt(INTERRUPT_PIN);
	attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
}


SpaceNavigator::SpaceNavigator() {

}

#ifndef _BV
#define _BV(n) (1<<(n))
#endif

void SpaceNavigator::initialize_spaceNav() {
	// join I2C bus (I2Cdev_mod library doesn't do this automatically)
#if I2CDEV_MOD_IMPLEMENTATION == I2CDEV_MOD_ARDUINO_WIRE
	Wire.begin();
	Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
#elif I2CDEV_MOD_IMPLEMENTATION == I2CDEV_MOD_BUILTIN_FASTWIRE
	Fastwire::setup(400, true);
#endif


	// initialize device
	//Serial.println(F("Initializing I2C devices..."));
	mpu.initialize();
	

	// verify connection
	//Serial.println(F("Testing device connections..."));
	//Serial.println(mpu.testConnection() ? F("MPU6050_MOD connection successful") : F("MPU6050_MOD connection failed"));

	// wait for ready
	/*Serial.println(F("\nSend any character to begin DMP programming and demo: "));
	  while (Serial.available() && Serial.read()); // empty buffer
	  while (!Serial.available());                 // wait for data
	  while (Serial.available() && Serial.read()); // empty buffer again
	*/

	// load and configure the DMP
	//Serial.println(F("Initializing DMP..."));
	devStatus = mpu.dmpInitialize();

	// supply your own gyro offsets here, scaled for min sensitivity
	mpu.setXGyroOffset(220);
	mpu.setYGyroOffset(76);
	mpu.setZGyroOffset(-85);
	mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

	// make sure it worked (returns 0 if so)
	if (devStatus == 0) {
		// Calibration Time: generate offsets and calibrate our MPU6050_MOD
		mpu.CalibrateAccel(6);
		mpu.CalibrateGyro(6);
		mpu.PrintActiveOffsets();
		// turn on the DMP, now that it's ready
		//Serial.println(F("Enabling DMP..."));
		mpu.setDMPEnabled(true);

		// enable Arduino interrupt detection
		//Serial.print(F("Enabling interrupt detection (Arduino external interrupt "));
		//digitalPinToInterrupt(INTERRUPT_PIN);
		//Serial.print(digitalPinToInterrupt(INTERRUPT_PIN));
		
		setinterupt();
		
		//Serial.println(F(")..."));
		//attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
		mpuIntStatus = mpu.getIntStatus();

		// set our DMP Ready flag so the main loop() function knows it's okay to use it
		//Serial.println(F("DMP ready! Waiting for first interrupt..."));
		dmpReady = true;

		// get expected DMP packet size for later comparison
		packetSize = mpu.dmpGetFIFOPacketSize();
	}
	else {
		// ERROR!
		// 1 = initial memory load failed
		// 2 = DMP configuration updates failed
		// (if it's going to break, usually the code will be 1)
		//Serial.print(F("DMP Initialization failed (code "));
		//Serial.print(devStatus);
		//Serial.println(F(")"));
	}
	reset_spaceNav();
	
}

void SpaceNavigator::set_adaptiveSpaceNavigation(bool adsn) {
	adaptive_spaceNav = adsn;
}

void SpaceNavigator::softreset() {
	mpu.CalibrateAccel(6);
	mpu.CalibrateGyro(6);

	X_origin = adcVal.Read(x_sensor);
	Y_origin = adcVal.Read(y_sensor);
	recaliberate = false;
}
void SpaceNavigator::set_XYSensorPins(int x_sens, int y_sens) {
	x_sensor = x_sens;
	y_sensor = y_sens;
}

void SpaceNavigator::set_tiltBoundaryRadii(float maxrad, float minrad) {
	if (maxrad > minrad) {
		tilt_maxradius = maxrad;
		tilt_minradius = minrad;
	}
	
}
void SpaceNavigator::set_slideBoundaryRadii(float maxrad, float minrad) {
	if (maxrad > minrad) {
		slide_maxradius = maxrad;
		slide_minradius = minrad;
	}
	
}

void SpaceNavigator::set_orientationOffset() {
	float angles[50];
	float radius = pow((pow(gyro_x, 2) + pow(gyro_y, 2)), 0.5);
	if (radius >= tilt_minradius) {
		for (int i = 0; i < 50; i++) {
			reset_spaceNav();
			if (gyro_x > 0 && gyro_y > 0) {  //quadrant 1
		  //calc angle
				angles[i] = acos((gyro_x / radius)) * (180 / 3.142);
			}
			else if (gyro_x < 0 && gyro_y > 0) {  //quadrant 2
			  //calc angle
				angles[i] = 90 - (asin((gyro_x / radius)) * (180 / 3.142));
			}
			else if (gyro_x < 0 && gyro_y < 0) {  //quadrant 3
			  //calc angle
				angles[i] = 360 - (acos((gyro_x / radius)) * (180 / 3.142));
			}
			else if (gyro_x > 0 && gyro_y < 0) {  //quadrant 4
			  //calc angle
				angles[i] = 270 + (asin((gyro_x / radius)) * (180 / 3.142));
			}
			Orientation_angleoffset = Orientation_angleoffset + (angles[i] / 50);
		}
	}
	else {
		Orientation_angleoffset = 0;
	}
	
}

float *SpaceNavigator::get_gyroCoordinates() {
	
	//find radius of received pitch and roll
	float radius = pow((pow(gyro_x, 2) + pow(gyro_y, 2)), 0.5);
	float angle = 0;

	if (gyro_x > 0 && gyro_y > 0) {  //quadrant 1
	  //calc angle
		angle = acos((gyro_x / radius)) * (180 / 3.142);
	}
	else if (gyro_x < 0 && gyro_y > 0) {  //quadrant 2
	  //calc angle
		angle = 90 - (asin((gyro_x / radius)) * (180 / 3.142));
	}
	else if (gyro_x < 0 && gyro_y < 0) {  //quadrant 3
	  //calc angle
		angle = 360 - (acos((gyro_x / radius)) * (180 / 3.142));
	}
	else if (gyro_x > 0 && gyro_y < 0) {  //quadrant 4
	  //calc angle
		angle = 270 + (asin((gyro_x / radius)) * (180 / 3.142));
	}

	if (adaptive_spaceNav) {
		
		// adjust calculated angle with yaw angle
		int adjustedangle = angle;
		for (int i = 0; i < Orientation_angle; i++)
		{
			if (adjustedangle == 360)
			{
				adjustedangle = 0;
			}
			else
			{
				adjustedangle += 1;
			}

		}
		// Second level adjustment when Orientation_angleoffset is set
		if (Orientation_angleoffset > 0) {
			for (int i = 0; i < Orientation_angleoffset; i++)
			{
				if (adjustedangle == 360)
				{
					adjustedangle = 0;
				}
				else
				{
					adjustedangle += 1;
				}

			}
		}
		//calculate a new x,y coordinate from adjusted angle
		float newangle = adjustedangle;

		//checks if variables satisfy set boundaries.
		if (radius >= tilt_minradius && radius <= tilt_maxradius) {
			Gyro_XY_coordinate[0] = (radius * cos(newangle * 0.01745));
			Gyro_XY_coordinate[1] = (radius * sin(newangle * 0.01745));
			Gyro_XY_coordinate[2] = radius;
		}
		else if (radius > tilt_maxradius) {
			radius = tilt_maxradius;
			Gyro_XY_coordinate[0] = (radius * cos(newangle * 0.01745));
			Gyro_XY_coordinate[1] = (radius * sin(newangle * 0.01745));
			Gyro_XY_coordinate[2] = radius;
		}
		else {
			Gyro_XY_coordinate[0] = 0;
			Gyro_XY_coordinate[1] = 0;
			Gyro_XY_coordinate[2] = 0;
		}

		
	}
	else {
		int adjustedangle = angle;
		// Second level adjustment when Orientation_angleoffset is set
		if (Orientation_angleoffset > 0) {
			for (int i = 0; i < Orientation_angleoffset; i++)
			{
				if (adjustedangle == 360)
				{
					adjustedangle = 0;
				}
				else
				{
					adjustedangle += 1;
				}

			}
		}
		//checks if variables satisfy set boundaries.
		if (radius >= tilt_minradius && radius <= tilt_maxradius) {
			Gyro_XY_coordinate[0] = (radius * cos(adjustedangle * 0.01745));
			Gyro_XY_coordinate[1] = (radius * sin(adjustedangle * 0.01745));
			Gyro_XY_coordinate[2] = radius;
		}
		else if (radius > tilt_maxradius) {
			radius = tilt_maxradius;
			Gyro_XY_coordinate[0] = (radius * cos(adjustedangle * 0.01745));
			Gyro_XY_coordinate[1] = (radius * sin(adjustedangle * 0.01745));
			Gyro_XY_coordinate[2] = radius;
		}
		else {
			Gyro_XY_coordinate[0] = 0;
			Gyro_XY_coordinate[1] = 0;
			Gyro_XY_coordinate[2] = 0;
		}
	}
return Gyro_XY_coordinate;
}

float *SpaceNavigator::get_planarCoordinates() {
	float angle = 0;
	//find radius of received x , y planar coordinates
	float radius = pow((pow(flatplane_x, 2) + pow(flatplane_y, 2)), 0.5);

	if (flatplane_x > 0 && flatplane_y > 0) {  //quadrant 1
	  //calc angle
		angle = acos((flatplane_x / radius)) * (180 / 3.142);
	}
	else if (flatplane_x < 0 && flatplane_y > 0) {  //quadrant 2
	  //calc angle
		angle = 90 - (asin((flatplane_x / radius)) * (180 / 3.142));
	}
	else if (flatplane_x < 0 && flatplane_y < 0) {  //quadrant 3
	  //calc angle
		angle = 360 - (acos((flatplane_x / radius)) * (180 / 3.142));
	}
	else if (flatplane_x > 0 && flatplane_y < 0) {  //quadrant 4
	  //calc angle
		angle = 270 + (asin((flatplane_x / radius)) * (180 / 3.142));
	}

	if (adaptive_spaceNav) {	
		// adjust calculated angle with yaw angle
		int adjustedangle = angle;
		for (int i = 0; i < Orientation_angle; i++)
		{
			if (adjustedangle == 360)
			{
				adjustedangle = 0;
			}
			else
			{
				adjustedangle += 1;
			}

		}
		// Second level adjustment when Orientation_angleoffset is set
		if (Orientation_angleoffset > 0) {
			for (int i = 0; i < Orientation_angleoffset; i++)
			{
				if (adjustedangle == 360)
				{
					adjustedangle = 0;
				}
				else
				{
					adjustedangle += 1;
				}

			}
		}
		//calculate a new x,y coordinate from adjusted angle
		float newangle = adjustedangle;

		//checks if variables satisfy set boundaries.
		if (radius >= slide_minradius && radius <= slide_maxradius) {
			Planar_XY_coordinate[0] = (radius * cos(newangle * 0.01745));
			Planar_XY_coordinate[1] = (radius * sin(newangle * 0.01745));
			Planar_XY_coordinate[2] = radius;
		}
		else if (radius > slide_maxradius) {
			radius = slide_maxradius;
			Planar_XY_coordinate[0] = (radius * cos(newangle * 0.01745));
			Planar_XY_coordinate[1] = (radius * sin(newangle * 0.01745));
			Planar_XY_coordinate[2] = radius;
		}
		else {
			Planar_XY_coordinate[0] = 0;
			Planar_XY_coordinate[1] = 0;
			Planar_XY_coordinate[2] = 0;
		}
	}
	else {
		int adjustedangle = angle;
		// Second level adjustment when Orientation_angleoffset is set
		if (Orientation_angleoffset > 0) {
			for (int i = 0; i < Orientation_angleoffset; i++)
			{
				if (adjustedangle == 360)
				{
					adjustedangle = 0;
				}
				else
				{
					adjustedangle += 1;
				}

			}
		}
		//checks if variables satisfy set boundaries.
		if (radius >= slide_minradius && radius <= slide_maxradius) {
			Planar_XY_coordinate[0] = (radius * cos(adjustedangle * 0.01745));
			Planar_XY_coordinate[1] = (radius * sin(adjustedangle * 0.01745));
			Planar_XY_coordinate[2] = radius;
		}
		else if (radius > slide_maxradius) {
			radius = slide_maxradius;
			Planar_XY_coordinate[0] = (radius * cos(adjustedangle * 0.01745));
			Planar_XY_coordinate[1] = (radius * sin(adjustedangle * 0.01745));
			Planar_XY_coordinate[2] = radius;
		}
		else {
			Planar_XY_coordinate[0] = 0;
			Planar_XY_coordinate[1] = 0;
			Planar_XY_coordinate[2] = 0;
		}
	}

	return Planar_XY_coordinate;
}

bool *SpaceNavigator::get_Tilt() {
	//find radius of received pitch and roll
	float radius = pow((pow(gyro_x, 2) + pow(gyro_y, 2)), 0.5);
	float angle = 0;
		if (gyro_x > 0 && gyro_y > 0) {  //quadrant 1
	  //calc angle
			angle = acos((gyro_x / radius)) * (180 / 3.142);
		}
		else if (gyro_x < 0 && gyro_y > 0) {  //quadrant 2
		  //calc angle
			angle = 90 - (asin((gyro_x / radius)) * (180 / 3.142));
		}
		else if (gyro_x < 0 && gyro_y < 0) {  //quadrant 3
		  //calc angle
			angle = 360 - (acos((gyro_x / radius)) * (180 / 3.142));
		}
		else if (gyro_x > 0 && gyro_y < 0) {  //quadrant 4
		  //calc angle
			angle = 270 + (asin((gyro_x / radius)) * (180 / 3.142));
		}
	if (adaptive_spaceNav) {
		// adjust calculated angle with yaw angle
		int adjustedangle = angle;
		for (int i = 0; i < Orientation_angle; i++)
		{
			if (adjustedangle == 360)
			{
				adjustedangle = 0;
			}
			else
			{
				adjustedangle += 1;
			}

		}

		// Second level adjustment when Orientation_angleoffset is set
		if (Orientation_angleoffset > 0) {
			for (int i = 0; i < Orientation_angleoffset; i++)
			{
				if (adjustedangle == 360)
				{
					adjustedangle = 0;
				}
				else
				{
					adjustedangle += 1;
				}

			}
		}

		//translate adjustedangle to tilt movements
		if (radius >= (tiltmin)) {
			if (adjustedangle <= tiltandslide_range && adjustedangle >= 0 || adjustedangle >= (360 - tiltandslide_range) && adjustedangle <= 360) {  //tilt right
				haptics(1);
				tilt[0] = 1;
				tilt[1] = 0;
				tilt[2] = 0;
				tilt[3] = 0;

			}
			else if (adjustedangle <= (90 + tiltandslide_range) && adjustedangle >= (90 - tiltandslide_range)) {  //tilt up
				haptics(1);
				tilt[1] = 1;
				tilt[0] = 0;
				tilt[2] = 0;
				tilt[3] = 0;

			}
			else if (adjustedangle <= (180 + tiltandslide_range) && adjustedangle >= (180 - tiltandslide_range)) {  //tilt left
				haptics(1);
				tilt[2] = 1;
				tilt[0] = 0;
				tilt[1] = 0;
				tilt[3] = 0;

			}
			else if (adjustedangle <= (270 + tiltandslide_range) && adjustedangle >= (270 - tiltandslide_range)) {  //tilt down
				haptics(1);
				tilt[3] = 1;
				tilt[0] = 0;
				tilt[1] = 0;
				tilt[2] = 0;

			}
			else if (adjustedangle > tiltandslide_range && adjustedangle < (90 - tiltandslide_range)) { //tilt up and right
				haptics(1);
				tilt[1] = 1;
				tilt[0] = 1;
				tilt[2] = 0;
				tilt[3] = 0;
			}
			else if (adjustedangle > (90 + tiltandslide_range) && adjustedangle < (180 - tiltandslide_range)) { //tilt up and left
				haptics(1);
				tilt[1] = 1;
				tilt[2] = 1;
				tilt[0] = 0;
				tilt[3] = 0;
			}
			else if (adjustedangle > (180 + tiltandslide_range) && adjustedangle < (270 - tiltandslide_range)) { //tilt down and left
				haptics(1);
				tilt[3] = 1;
				tilt[2] = 1;
				tilt[0] = 0;
				tilt[1] = 0;
			}
			else if (adjustedangle > (270 + tiltandslide_range) && adjustedangle < (360 - tiltandslide_range)) { //tilt down and right
				haptics(1);
				tilt[3] = 1;
				tilt[0] = 1;
				tilt[1] = 0;
				tilt[2] = 0;
			}
			return tilt;
		}
		else {
			tilt[0] = 0;
			tilt[1] = 0;
			tilt[2] = 0;
			tilt[3] = 0;
			return tilt;
		}
		
	}
	else {//translate angle to tilt movements
		int adjustedangle = angle;
		// Second level adjustment when Orientation_angleoffset is set
		if (Orientation_angleoffset > 0) {
			for (int i = 0; i < Orientation_angleoffset; i++)
			{
				if (adjustedangle == 360)
				{
					adjustedangle = 0;
				}
				else
				{
					adjustedangle += 1;
				}

			}
		}
		//translate adjustedangle to tilt movements
		if (radius >= (tiltmin)) {
			if (adjustedangle <= tiltandslide_range && adjustedangle >= 0 || adjustedangle >= (360 - tiltandslide_range) && adjustedangle <= 360) {  //tilt right
				haptics(1);
				tilt[0] = 1;
				tilt[1] = 0;
				tilt[2] = 0;
				tilt[3] = 0;

			}
			else if (adjustedangle <= (90 + tiltandslide_range) && adjustedangle >= (90 - tiltandslide_range)) {  //tilt up
				haptics(1);
				tilt[1] = 1;
				tilt[0] = 0;
				tilt[2] = 0;
				tilt[3] = 0;

			}
			else if (adjustedangle <= (180 + tiltandslide_range) && adjustedangle >= (180 - tiltandslide_range)) {  //tilt left
				haptics(1);
				tilt[2] = 1;
				tilt[0] = 0;
				tilt[1] = 0;
				tilt[3] = 0;

			}
			else if (adjustedangle <= (270 + tiltandslide_range) && adjustedangle >= (270 - tiltandslide_range)) {  //tilt down
				haptics(1);
				tilt[3] = 1;
				tilt[0] = 0;
				tilt[1] = 0;
				tilt[2] = 0;
				
			}
			else if (adjustedangle > tiltandslide_range && adjustedangle < (90 - tiltandslide_range)) { //tilt up and right
				haptics(1);
				tilt[1] = 1;
				tilt[0] = 1;
				tilt[2] = 0;
				tilt[3] = 0;
			}
			else if (adjustedangle > (90 + tiltandslide_range) && adjustedangle < (180 - tiltandslide_range)) { //tilt up and left
				haptics(1);
				tilt[1] = 1;
				tilt[2] = 1;
				tilt[0] = 0;
				tilt[3] = 0;
			}
			else if (adjustedangle > (180 + tiltandslide_range) && adjustedangle < (270 - tiltandslide_range)) { //tilt down and left
				haptics(1);
				tilt[3] = 1;
				tilt[2] = 1;
				tilt[0] = 0;
				tilt[1] = 0;
			}
			else if (adjustedangle > (270 + tiltandslide_range) && adjustedangle < (360 - tiltandslide_range)) { //tilt down and right
				haptics(1);
				tilt[3] = 1;
				tilt[0] = 1;
				tilt[1] = 0;
				tilt[2] = 0;
			}
			return tilt;
		}
		else {
			tilt[0] = 0;
			tilt[1] = 0;
			tilt[2] = 0;
			tilt[3] = 0;
			return tilt;
		}
	}
}

bool *SpaceNavigator::get_Slide() {
		float angle = 0;
		//find radius of received x , y planar coordinates
		float radius = pow((pow(flatplane_x, 2) + pow(flatplane_y, 2)), 0.5);
			if (flatplane_x > 0 && flatplane_y > 0) {  //quadrant 1
		  //calc angle
				angle = acos((flatplane_x / radius)) * (180 / 3.142);
			}
			else if (flatplane_x < 0 && flatplane_y > 0) {  //quadrant 2
			  //calc angle
				angle = 90 - (asin((flatplane_x / radius)) * (180 / 3.142));
			}
			else if (flatplane_x < 0 && flatplane_y < 0) {  //quadrant 3
			  //calc angle
				angle = 360 - (acos((flatplane_x / radius)) * (180 / 3.142));
			}
			else if (flatplane_x > 0 && flatplane_y < 0) {  //quadrant 4
			  //calc angle
				angle = 270 + (asin((flatplane_x / radius)) * (180 / 3.142));
			}

	if (adaptive_spaceNav) {
		// adjust calculated angle with yaw angle
		int adjustedangle = angle;
		for (int i = 0; i < Orientation_angle; i++)
		{
			if (adjustedangle == 360)
			{
				adjustedangle = 0;
			}
			else
			{
				adjustedangle += 1;
			}

		}
		// Second level adjustment when Orientation_angleoffset is set
		if (Orientation_angleoffset > 0) {
			for (int i = 0; i < Orientation_angleoffset; i++)
			{
				if (adjustedangle == 360)
				{
					adjustedangle = 0;
				}
				else
				{
					adjustedangle += 1;
				}

			}
		}
		//translate adjustedangle to slide movements
		if (radius >= (slidemin)) {
			if (adjustedangle <= tiltandslide_range && adjustedangle >= 0 || adjustedangle >= (360 - tiltandslide_range) && adjustedangle <= 360) {  //slide right
				haptics(1);
				slide[0] = 1;
				slide[1] = 0;
				slide[2] = 0;
				slide[3] = 0;

			}
			else if (adjustedangle <= (90 + tiltandslide_range) && adjustedangle >= (90 - tiltandslide_range)) {  //slide up
				haptics(1);
				slide[1] = 1;
				slide[0] = 0;
				slide[2] = 0;
				slide[3] = 0;

			}
			else if (adjustedangle <= (180 + tiltandslide_range) && adjustedangle >= (180 - tiltandslide_range)) {  //slide left
				haptics(1);
				slide[2] = 1;
				slide[0] = 0;
				slide[1] = 0;
				slide[3] = 0;

			}
			else if (adjustedangle <= (270 + tiltandslide_range) && adjustedangle >= (270 - tiltandslide_range)) {  //slide down
				haptics(1);
				slide[3] = 1;
				slide[0] = 0;
				slide[1] = 0;
				slide[2] = 0;

			}
			else if (adjustedangle > tiltandslide_range && adjustedangle < (90 - tiltandslide_range)) { //slide up and right
				haptics(1);
				slide[1] = 1;
				slide[0] = 1;
				slide[2] = 0;
				slide[3] = 0;
			}
			else if (adjustedangle > (90 + tiltandslide_range) && adjustedangle < (180 - tiltandslide_range)) { //slide up and left
				haptics(1);
				slide[1] = 1;
				slide[2] = 1;
				slide[0] = 0;
				slide[3] = 0;
			}
			else if (adjustedangle > (180 + tiltandslide_range) && adjustedangle < (270 - tiltandslide_range)) { //slide down and left
				haptics(1);
				slide[3] = 1;
				slide[2] = 1;
				slide[0] = 0;
				slide[1] = 0;
			}
			else if (adjustedangle > (270 + tiltandslide_range) && adjustedangle < (360 - tiltandslide_range)) { //slide down and right
				haptics(1);
				slide[3] = 1;
				slide[0] = 1;
				slide[1] = 0;
				slide[2] = 0;
			}
			return slide;
		}
		else {
			slide[0] = 0;
			slide[1] = 0;
			slide[2] = 0;
			slide[3] = 0;
			return slide;
		}
		
	}
	else {  //translate angle to slide movements
		int adjustedangle = angle;
		// Second level adjustment when Orientation_angleoffset is set
		if (Orientation_angleoffset > 0) {
			for (int i = 0; i < Orientation_angleoffset; i++)
			{
				if (adjustedangle == 360)
				{
					adjustedangle = 0;
				}
				else
				{
					adjustedangle += 1;
				}

			}
		}
		//translate adjustedangle to slide movements
		if (radius >= (slidemin)) {
			if (adjustedangle <= tiltandslide_range && adjustedangle >= 0 || adjustedangle >= (360 - tiltandslide_range) && adjustedangle <= 360) {  //slide right
				haptics(1);
				slide[0] = 1;
				slide[1] = 0;
				slide[2] = 0;
				slide[3] = 0;

			}
			else if (adjustedangle <= (90 + tiltandslide_range) && adjustedangle >= (90 - tiltandslide_range)) {  //slide up
				haptics(1);
				slide[1] = 1;
				slide[0] = 0;
				slide[2] = 0;
				slide[3] = 0;

			}
			else if (adjustedangle <= (180 + tiltandslide_range) && adjustedangle >= (180 - tiltandslide_range)) {  //slide left
				haptics(1);
				slide[2] = 1;
				slide[0] = 0;
				slide[1] = 0;
				slide[3] = 0;

			}
			else if (adjustedangle <= (270 + tiltandslide_range) && adjustedangle >= (270 - tiltandslide_range)) {  //slide down
				haptics(1);
				slide[3] = 1;
				slide[0] = 0;
				slide[1] = 0;
				slide[2] = 0;

			}
			else if (adjustedangle > tiltandslide_range && adjustedangle < (90 - tiltandslide_range)) { //slide up and right
				haptics(1);
				slide[1] = 1;
				slide[0] = 1;
				slide[2] = 0;
				slide[3] = 0;
			}
			else if (adjustedangle > (90 + tiltandslide_range) && adjustedangle < (180 - tiltandslide_range)) { //slide up and left
				haptics(1);
				slide[1] = 1;
				slide[2] = 1;
				slide[0] = 0;
				slide[3] = 0;
			}
			else if (adjustedangle > (180 + tiltandslide_range) && adjustedangle < (270 - tiltandslide_range)) { //slide down and left
				haptics(1);
				slide[3] = 1;
				slide[2] = 1;
				slide[0] = 0;
				slide[1] = 0;
			}
			else if (adjustedangle > (270 + tiltandslide_range) && adjustedangle < (360 - tiltandslide_range)) { //slide down and right
				haptics(1);
				slide[3] = 1;
				slide[0] = 1;
				slide[1] = 0;
				slide[2] = 0;
			}
			return slide;
		}
		else {
			slide[0] = 0;
			slide[1] = 0;
			slide[2] = 0;
			slide[3] = 0;
			return slide;
		}
		
	}
}

void SpaceNavigator::reset_spaceNav() {
	haptics(0);
	
	
	// if programming failed, don't try to do anything
	if (!dmpReady) return;

	if (mpuInterrupt && fifoCount < packetSize) {
		// try to get out of the infinite loop 
		//fifoCount = mpu.getFIFOCount();
		// reset interrupt flag and get INT_STATUS byte
		mpuInterrupt = false;
		mpuIntStatus = mpu.getIntStatus();
/*
		Serial.print(packetSize);
		Serial.print("\t");
		Serial.print(fifoCount);
		Serial.print("\t");
		Serial.print(Orientation_angle);
		Serial.print("\t");*/
		// get current FIFO count
		fifoCount = mpu.getFIFOCount();
		if (fifoCount < packetSize) {
			//Lets go back and wait for another interrupt. We shouldn't be here, we got an interrupt from another event
			// This is blocking so don't do it   while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
		}
		// check for overflow (this should never happen unless our code is too inefficient)
		else if ((mpuIntStatus & _BV(MPU6050_MOD_INTERRUPT_FIFO_OFLOW_BIT)) || fifoCount >= 1024) {
			// reset so we can continue cleanly
			mpu.resetFIFO();

			//  fifoCount = mpu.getFIFOCount();  // will be zero after reset no need to ask
			//SerialUSB.println(F("FIFO overflow!"));
			//delay(1);
			// otherwise, check for DMP data ready interrupt (this should happen frequently)
		}
		else if (mpuIntStatus & _BV(MPU6050_MOD_INTERRUPT_DMP_INT_BIT)) {

			// read a packet from FIFO
			while (fifoCount >= packetSize) { // Lets catch up to NOW, someone is using the dreaded delay()!
				mpu.getFIFOBytes(fifoBuffer, packetSize);
				// track FIFO count here in case there is > 1 packet available
				// (this lets us immediately read more without waiting for an interrupt)
				fifoCount -= packetSize;
				
			}

			mpu.dmpGetQuaternion(&q, fifoBuffer);
			mpu.dmpGetGravity(&gravity, &q);
			mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
			gyro_x = ypr[1] * 180 / M_PI * gyroscalingfactor;
			gyro_y = ypr[2] * 180 / M_PI * gyroscalingfactor;
			Orientation_angle = ypr[0] * 180 / M_PI;

			
			//read and tamper x
			tempADCPosX = float(adcVal.Read(x_sensor));
			diff = prevADCPosX - tempADCPosX;
			if (diff < 0)
			{
				diff *= -1;
			}
			if (diff < smallvaluerange)
			{
				ADCPosX = (tempADCPosX * S_alpha2) + (prevADCPosX * (1 - S_alpha2));
				prevADCPosX = ADCPosX;
			}
			else {
				ADCPosX = (tempADCPosX * S_alpha) + (prevADCPosX * (1 - S_alpha));
				prevADCPosX = ADCPosX;
			}
			
		
			//read and tamper y
			tempADCPosY = float(adcVal.Read(y_sensor));
			diff = prevADCPosY - tempADCPosY;
			if (diff < 0)
			{
				diff *= -1;
			}
			if (diff < smallvaluerange)
			{
				ADCPosY = (tempADCPosY * S_alpha2) + (prevADCPosY * (1 - S_alpha2));
				prevADCPosY = ADCPosY;
			}
			else {
				ADCPosY = (tempADCPosY * S_alpha) + (prevADCPosY * (1 - S_alpha));
				prevADCPosY = ADCPosY;
			}
			

			if (recaliberate) {
				if (gyro_x < gyrocenterrange && gyro_x > -1 * gyrocenterrange || gyro_y < gyrocenterrange && gyro_y > -1 * gyrocenterrange) { //update planar origin
					tempread = adcVal.Read(x_sensor);
					if (tempread < (idealorigin + bound) && tempread >(idealorigin - bound)) {//value is within allowed range
						X_origin = tempread;
					}
					tempread = adcVal.Read(y_sensor);
					if (tempread < (idealorigin + bound) && tempread >(idealorigin - bound)) {//value is within allowed range
						Y_origin = tempread;
					}
					recaliberate = false;
				}
			}
			
#if INVERTSPACENAV == true
			flatplane_x = (ADCPosX - X_origin) / scalingfactor;
			flatplane_y = (Y_origin - ADCPosY) / scalingfactor;
#elif INVERTSPACENAV == false
			flatplane_x = (X_origin - ADCPosX) / scalingfactor;
			flatplane_y = (ADCPosY - Y_origin) / scalingfactor;
#endif

			if (Orientation_angle < 0) {
				Orientation_angle = Orientation_angle * (-1);
			}
			else if (Orientation_angle <= 180 && Orientation_angle >= 1) {
				Orientation_angle = 360 - Orientation_angle;
			}
			else {
				Orientation_angle = 0;
			}
		}
	}
	else {
		if (fifoCount >= packetSize ) {
			fifoCount = 0;
		}

		//read and tamper x
		tempADCPosX = float(adcVal.Read(x_sensor));
		diff = prevADCPosX - tempADCPosX;
		if (diff < 0)
		{
			diff *= -1;
		}
		if (diff < smallvaluerange)
		{
			ADCPosX = (tempADCPosX * S_alpha2) + (prevADCPosX * (1 - S_alpha2));
			prevADCPosX = ADCPosX;
		}
		else {
			ADCPosX = (tempADCPosX * S_alpha) + (prevADCPosX * (1 - S_alpha));
			prevADCPosX = ADCPosX;
		}
		

		//read and tamper y
		tempADCPosY = float(adcVal.Read(y_sensor));
		diff = prevADCPosY - tempADCPosY;
		if (diff < 0)
		{
			diff *= -1;
		}
		if (diff < smallvaluerange)
		{
			ADCPosY = (tempADCPosY * S_alpha2) + (prevADCPosY * (1 - S_alpha2));
			prevADCPosY = ADCPosY;
		}
		else {
			ADCPosY = (tempADCPosY * S_alpha) + (prevADCPosY * (1 - S_alpha));
			prevADCPosY = ADCPosY;
		}

		if (recaliberate) {
			if (gyro_x < gyrocenterrange && gyro_x > -1*gyrocenterrange || gyro_y < gyrocenterrange && gyro_y > -1* gyrocenterrange) { //update planar origin
				tempread = adcVal.Read(x_sensor);
				if (tempread < (idealorigin + bound) && tempread >(idealorigin - bound)) {//value is within allowed range
					X_origin = tempread;
				}
				tempread = adcVal.Read(y_sensor);
				if (tempread < (idealorigin + bound) && tempread >(idealorigin - bound)) {//value is within allowed range
					Y_origin = tempread;
				}
				recaliberate = false;
			}
		}

#if INVERTSPACENAV == true
		flatplane_x = (ADCPosX - X_origin) / scalingfactor;
		flatplane_y = (Y_origin - ADCPosY) / scalingfactor;
#elif INVERTSPACENAV == false
		flatplane_x = (X_origin - ADCPosX) / scalingfactor;
		flatplane_y = (ADCPosY - Y_origin) / scalingfactor;
#endif

		



		
	}
	
	
	//SerialUSB.print(X_origin);
	//SerialUSB.print("\t");
	//SerialUSB.print(ypr[1] * 180 / M_PI);
	
	//SerialUSB.print("\t");
	//SerialUSB.println(ypr[2] * 180 / M_PI);
	/*
	SerialUSB.print("\t");
	SerialUSB.println(flatplane_y);
	*/
}

void SpaceNavigator::set_haptics(int pin, int duration, int strength) {  //use to set haptics variables (arduino pwm pin, duration of haptics(ms), strength from 0-255)
	haptics_pin = pin;
	haptics_duration = duration;
	haptics_strength = strength;
}

void SpaceNavigator::haptics(int state) {   //viberation feedback function
	if (state == 1) {
		analogWrite(haptics_pin, haptics_strength);
		haptics_ontime = millis();
		haptics_state = 1;
	}
	else {
		haptics_offtime = millis();
		if (((haptics_offtime - haptics_ontime) >= haptics_duration) && haptics_state == 1) {
			analogWrite(haptics_pin, 0);
			haptics_state = 0;
		}

	}
}
void SpaceNavigator::recaliberateOrigin(bool rec) {
	recaliberate = rec;
}
int SpaceNavigator::getadc(int pin) {
	return adcVal.Read(pin);
}
float SpaceNavigator::showplanecoordinats() {
	//SerialUSB.print(ADCPosX);
	//SerialUSB.print("\t");
	//SerialUSB.println(ADCPosY);
}