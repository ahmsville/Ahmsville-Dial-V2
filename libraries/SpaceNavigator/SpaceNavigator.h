

#ifndef SpaceNavigator_h
#define SpaceNavigator_h

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

#include "Arduino.h"

#include "ADCRead.h"
#include "I2Cdev_mod.h"

#include "MPU6050_MOD_DMP.h"

#if I2CDEV_MOD_IMPLEMENTATION == I2CDEV_MOD_ARDUINO_WIRE
#include "Wire.h"
#endif




#define INTERRUPT_PIN 2  // use pin 2 on Arduino Uno & most boards
//#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)

#define INVERTSPACENAV false  


class SpaceNavigator {
private:
	
	
	MPU6050_MOD mpu = MPU6050_MOD(0x69);
	//MPU6050_MOD mpu = MPU6050_MOD();
	// MPU control/status vars
	bool dmpReady = false;  // set true if DMP init was successful
	uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
	uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
	uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
	uint16_t fifoCount;     // count of all bytes currently in FIFO
	uint8_t fifoBuffer[64]; // FIFO storage buffer

	// orientation/motion vars
	Quaternion q;           // [w, x, y, z]         quaternion container
	VectorFloat gravity;    // [x, y, z]            gravity vector
	float euler[3];         // [psi, theta, phi]    Euler angle container
	float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
/******************************************************************************************************************************************/
	float adcresolution = 4095;
	float gyro_x = 0, gyro_y = 0, flatplane_x = 0, flatplane_y = 0, Orientation_angle = 0, Orientation_angleoffset = 0;  // holds raw values from gyro and hall sensors
	float X_origin = adcresolution/2, Y_origin = adcresolution / 2, ADCPosX = 0, ADCPosY = 0, tempread, tempADC = 0, idealorigin = 2047, bound = 200;  //defines center for hall effect sensors.
	float scalingfactor = 7, gyroscalingfactor = 6; //maximum distance(analog value) from origin / desired x,y scale
	float tiltmin = 8;
	float slidemin = 10;
	float rate = 0.07;
	float tempADCPosX = 0, tempADCPosY = 0;
	float prevADCPosX = 0, prevADCPosY = 0;
	float S_alpha = 0.09, S_alpha2 = 0.005, smallvaluerange = 10, diff = 0;


	int x_sensor, y_sensor;  //hall effect sensor arduino pins
	float Planar_XY_coordinate[3] , Gyro_XY_coordinate[3];  //holds adjusted x,y values for gyro, hall sensor and corresponding calculated radius. 
	bool tilt[4], slide[4]; //holds tilt and slide identifiers
	bool adaptive_spaceNav = false;
	int tiltandslide_range = 30;
	float tilt_maxradius = 0, tilt_minradius = 0, slide_maxradius = 0, slide_minradius = 0;
	int haptics_pin = 0, haptics_duration = 0, haptics_strength = 0, haptics_ontime, haptics_offtime, haptics_state;
	bool recaliberate = true;
	
	float gyrocenterrange = 4;

	ADCRead adcVal = ADCRead();

public:

	SpaceNavigator();
	void initialize_spaceNav();
	void set_XYSensorPins(int x_sens,int y_sens);
	void softreset();
	void set_adaptiveSpaceNavigation(bool adsn);
	void set_tiltBoundaryRadii(float maxrad, float minrad);
	void set_slideBoundaryRadii(float maxrad, float minrad);
	void set_orientationOffset();
	float *get_gyroCoordinates();
	float *get_planarCoordinates();
	bool *get_Tilt();
	bool *get_Slide();
	void reset_spaceNav();
	void set_haptics(int pin, int duration, int strength);   //use to set haptics variables (arduino pwm pin, duration of haptics(ms), strength from 0-255)
	void haptics(int state);
	void recaliberateOrigin(bool rec);
	int getadc(int pin);
	float showplanecoordinats();

};



#endif
