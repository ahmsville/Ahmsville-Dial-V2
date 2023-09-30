# Advanced_capacitive_touch_detection

These capacitive touch detection library is an extension of the Arduino capacitive touch library, it offers two detection algorithms that solves two of the main problems associated with detecting capacitive touch on the Arduino. The library also allows advanced touch detections; such as, Double tap, Short press and Long press. Also featured in the library is a haptics controller, this allows you to connect and control a vibration motor as a feedback when you interact with the capacitive touch surface.

```C
#include <AdvCapTouch.h>

AdvCapTouch samplepad  = AdvCapTouch();  //create a new captouch object
int touchtype;

void setup() {
<<<<<<< HEAD
  samplepad.set_inputTypeThresholds(20, 40, 70, 150); // set the thresholds for the four input types  (singletap, shortpress, longpress, doubletapspeed)
  samplepad.set_detectionThreshold(400, 100);  //set touch sensitivity in the form of detection, rejection thresholds values
  samplepad.set_capTouchPins(3,4,0,0,0);   //set arduino pins associated with the pads (sendpin, receivepin1, receivepin2, receivepin3, receivepin4) this example uses just one pad.
  samplepad.initialize_capTouch(1);
  samplepad.set_haptics(6,40,255);  //set haptic feedback variables (arduino pwm pin, duration of haptics(ms), pwn strength from 0-255)
  Serial.begin(9600);
=======
 samplepad.set_capTouchPins(3,4,0,0,0);   //set arduino pins associated with the pads (sendpin, receivepin1, receivepin2, receivepin3, receivepin4) this example uses just one pad.
  samplepad.set_haptics(4, 40, 255); //set haptic feedback variables (arduino pwm pin, duration of haptics(ms), pwn strength from 0-255)------(optional)
  /************************************************************************************************************************************************************/
  samplepad.initialize_capTouch(1);
  Serial.begin(115200);
>>>>>>> 06d6f68bc240875565b51bfb032f9e6a3a2910a0
}
```

The code snippet above shows how you would typically setup the library.
<<<<<<< HEAD
Line 2 is used to create a new capacitive touch object; the object is named “samplepad” but you can give any other name.
In the setup section of the code, the first line calls the function set_inputTypeThresholds(), this function is used to set the different detection levels of the four detectable capacitive touch types (single tap, short press, long press and double tap). Think of the numbers as how long your finger must be on the pad for specific touch types. For example, the code above sets 20 as the single tap threshold; this means any touch input that yields a count less than 20 will be categorized as a single tap, if the touch yields a count greater than 20 but less than 40, the touch is categorized as a short press, the same logic applies to the remaining touch types.
The second line set_detectionThreshold(); is used to set the sensitivity of the capacitive touch surface. The function accepts two integers, the touch detection threshold and the touch rejection threshold, the recommended ratio of detection to rejection threshold is 4:1. The larger the detection thresholds, the lower the pad sensitivity.
Line three is used to configure the Arduino pins connected to the resistor and the touch surfaces. The code sample uses just one pin i:e one touch pad. You can have up to 4 pads connected, all of which will share the same send pin.
Line four is used to initialize the capacitive touch, the function initialize_capTouch() accepts an integer from 1 – 4, representing the number of pads your working with.
Line five is optional, only use this function when you have a haptics circuit setup.
=======

>>>>>>> 06d6f68bc240875565b51bfb032f9e6a3a2910a0

# Detecting touch in the main program loop.

```C
void loop() {
<<<<<<< HEAD
touchtype = samplepad.detect_touch(0);  //function return 1-4 based on the input detected, 1 = singletap, 2 = doubletap, 3 = shortpress, 4 = longpress
Serial.println(touchtype);
  samplepad.update_basevalue(0);
}
```

There are two main functions for running the touch detection algorithm, the first is the detect_touch() function. This function should be used for a low noise touch setup, the function will return an integer value from 0 – 4.
=======
touchtype = samplepad.detect_touchFromNoise(0);  //function return 1-4 based on the input detected, 1 = singletap, 2 = doubletap, 3 = shortpress, 4 = longpress
if (touchtype == 1) {
  Serial.println("Singletap");   
}
else if (touchtype == 2) {
  Serial.println("Doubletap");
}
else if (touchtype == 3) {
  Serial.println("Shortpress");
}
else if (touchtype == 4) {
  Serial.println("longpress");
}
}
```

You call the detect_touchFromNoise() function to detect touch inputs, the function will return an integer value from 0 – 4.
>>>>>>> 06d6f68bc240875565b51bfb032f9e6a3a2910a0

 •	0 means no touch was detected.
 
 •	1 means a single tap was detected.
 
 •	2 means a double tap was detected.
 
 •	3 means a short press was detected.
 
 •	4 means a long press was detected.
 
The function detect_touch() should always be called with a integer value from 0 – 3, this value represents the position of the capacitive touch pad you want to run a touch detection on.
<<<<<<< HEAD
For noisy touch setups, use the detect_touchFromNoise() function instead, this function is more reliable and works in the exact same way as detect_touch(). Because this algorithm involves signal sampling, it is just a little bit slower than the alternative.
The second function update_basevalue() in the loop is used to continuously update the nominal value of the touch pad. This function is integral to making the touch detection adaptable across various touch conditions.
=======

>>>>>>> 06d6f68bc240875565b51bfb032f9e6a3a2910a0

