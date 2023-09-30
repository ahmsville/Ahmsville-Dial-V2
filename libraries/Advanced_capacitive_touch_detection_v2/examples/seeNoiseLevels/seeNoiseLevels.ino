#include <AdvCapTouch.h>
/*Example sketch to examine the noise levels of your setup

   connect 1M resistor between pin 3 & 4 on the arduino
   connect your conductive surface to pin 4
   upload sketch and view resulting signal from the serial plotter
   use "read_valueFromNoise(0)" instead of  "read_value(0)"   to cleanup the signal if touch is not distinguishable from noise

  ....By Ahmsville...
*/

AdvCapTouch samplepad  = AdvCapTouch();  //create a new captouch instance
double capvalue;

void setup() {
  samplepad.set_capTouchPins(3,4,0,0,0);   //set arduino pins associated with the pads (sendpin, receivepin1, receivepin2, receivepin3, receivepin4) this example uses just one pad.
  /************************************************************************************************************************************************************/
  /************************************************************************************************************************************************************/
  samplepad.initialize_capTouch(1);
  Serial.begin(9600);

}
void loop() {
  capvalue = samplepad.read_value(0); //noisey signal
  //capvalue = samplepad.read_valueFromNoise(0); //cleaned up signal
  Serial.println(capvalue); 

}