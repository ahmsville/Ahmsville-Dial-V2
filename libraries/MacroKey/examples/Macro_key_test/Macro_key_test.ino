#include <MacroKey.h>

MacroKey MK;

int *mkeystate;
void setup() {
   SerialUSB.begin(115200);
  MK.initialize_macroKey();
  
}

void loop() {
mkeystate = MK.get_keyPress();
if(mkeystate[0] != 0 || mkeystate[1] != 0 || mkeystate[2] != 0 || mkeystate[3] != 0 || mkeystate[4] != 0){
 SerialUSB.print(mkeystate[0]);
 SerialUSB.print("\t");
 SerialUSB.print(mkeystate[1]);
 SerialUSB.print("\t");
 SerialUSB.print(mkeystate[2]);
 SerialUSB.print("\t");
 SerialUSB.print(mkeystate[3]);
 SerialUSB.print("\t");
 SerialUSB.println(mkeystate[4]);
 MK.reset_macroKey();
}
}
