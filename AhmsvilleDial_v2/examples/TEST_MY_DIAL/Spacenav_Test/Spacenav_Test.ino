#include <SpaceNavigator.h>


SpaceNavigator spacenav;

float *gyro_x_y_radius;
float *plane_x_y_radius;
bool *tilt;
bool *slide;


long timetaken = 0;

void setup() {
  SerialUSB.begin(115200);
  while (!SerialUSB) {

  }
  analogReadResolution(12);
  SerialUSB.println("started");
  pinMode(30, OUTPUT);
  digitalWrite(30, LOW);
  delay(500);
  spacenav.set_XYSensorPins(A5, A4);
  spacenav.set_adaptiveSpaceNavigation(true);
  spacenav.set_tiltBoundaryRadii(100, 0);
  spacenav.set_slideBoundaryRadii(100, 0);
  spacenav.initialize_spaceNav();
  SerialUSB.println("ended");
  // put your setup code here, to run once:

pinMode(12, INPUT);
}

void loop() {

  if(digitalRead(12)){ //press mk5 to set center position
    if(gyro_x_y_radius[2] > 0.0){
          spacenav.set_tiltBoundaryRadii(100, gyro_x_y_radius[2]+0.5);
    }
    if(plane_x_y_radius[2] > 0.0){
        spacenav.set_slideBoundaryRadii(100, plane_x_y_radius[2]+0.5);
    }
  }
  
timetaken = millis();
  gyro_x_y_radius = spacenav.get_gyroCoordinates();
  plane_x_y_radius = spacenav.get_planarCoordinates();
  tilt = spacenav.get_Tilt();
  slide = spacenav.get_Slide();
  // SerialUSB.println(millis() - timetaken);
  // spacenav.get_Slide();

  SerialUSB.print(gyro_x_y_radius[0]);
  SerialUSB.print("\t");
  SerialUSB.print(gyro_x_y_radius[1]);
  SerialUSB.print("\t");
  SerialUSB.print(gyro_x_y_radius[2]);
  SerialUSB.print("\t");
  SerialUSB.print(plane_x_y_radius[0]);
  SerialUSB.print("\t");
  SerialUSB.print(plane_x_y_radius[1]);
  SerialUSB.print("\t");
  SerialUSB.print(plane_x_y_radius[2]);
  SerialUSB.print("\t");

/*
SerialUSB.print(tilt[0]);
  SerialUSB.print("\t");
  SerialUSB.print(tilt[1]);
  SerialUSB.print("\t");
  SerialUSB.print(tilt[2]);
  SerialUSB.print("\t");
  SerialUSB.print(tilt[3]);
SerialUSB.print("\t");
  SerialUSB.print(slide[0]);
  SerialUSB.print("\t");
  SerialUSB.print(slide[1]);
  SerialUSB.print("\t");
  SerialUSB.print(slide[2]);
  SerialUSB.print("\t");
  SerialUSB.print(slide[3]);
  SerialUSB.print("\t");

*/

  if (tilt[1] == 1 && tilt[0] == 1) {
    SerialUSB.print("tilt up and right");
    SerialUSB.print("\t");
  }
  else if (tilt[1] == 1 && tilt[2] == 1) {
    SerialUSB.print("tilt up and left");
    SerialUSB.print("\t");
  }
  else if (tilt[3] == 1 && tilt[0] == 1) {
    SerialUSB.print("tilt down and right");
    SerialUSB.print("\t");
  }
  else if (tilt[3] == 1 && tilt[2] == 1) {
    SerialUSB.print("tilt down and left");
    SerialUSB.print("\t");
  } else if (tilt[0] == 1) {
    SerialUSB.print("tilt right");
    SerialUSB.print("\t");
  }
  else if (tilt[1] == 1) {
    SerialUSB.print("tilt up");
    SerialUSB.print("\t");
  }
  else if (tilt[2] == 1) {
    SerialUSB.print("tilt left");
    SerialUSB.print("\t");
  }
  else if (tilt[3] == 1) {
    SerialUSB.print("tilt down");
    SerialUSB.print("\t");
  } else {
    SerialUSB.print("tilt center");
    SerialUSB.print("\t");
  }

  if (slide[1] == 1 && slide[0] == 1) {
    SerialUSB.println("slide up and right");

  }
  else if (slide[1] == 1 && slide[2] == 1) {
    SerialUSB.println("slide up and left");

  }
  else if (slide[3] == 1 && slide[0] == 1) {
    SerialUSB.println("slide down and right");

  }
  else if (slide[3] == 1 && slide[2] == 1) {
    SerialUSB.println("slide down and left");
 
  } 
 else if (slide[0] == 1) {
    SerialUSB.println("slide right");
  }
  else if (slide[1] == 1) {
    SerialUSB.println("slide up");
  }
  else if (slide[2] == 1) {
    SerialUSB.println("slide left");
  }
  else if (slide[3] == 1) {
    SerialUSB.println("slide down");
  }
  else {
    SerialUSB.println("slide center");
  }

  spacenav.reset_spaceNav();



}