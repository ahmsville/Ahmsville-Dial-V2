#include <AhmsvilleDial_v2.h>
#include <FastLED.h>
#include <SPI.h>
#include "RF24.h"

#define maxconfignum 1

AhmsvilleDial ahmsville_dial; //create a new ahmsville dial instance
RF24 radio(6, 7);                          // Set up nRF24L01 radio on SPI bus plus pins 7 & 8(CE,CSN)



#define substrlen 12
#define serialdatalength 30



byte addresses[][6] = { "1Node", "2Node" };



#define DATA_PIN    3
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS   8
#define MK_LEDS   0


CRGB leds[NUM_LEDS];

#define BRIGHTNESS         96

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

uint8_t gHue = 0; // rotating "base color"


#define MOUSEMOVE '/'
#define MOUSEMOVE_UP 'U'
#define MOUSEMOVE_DOWN 'D'
#define MOUSEMOVE_LEFT 'L'
#define MOUSEMOVE_RIGHT 'R'
char KEY_SPACEBAR = 32;
char nullchar = 128;

#define ALLColors CRGB::Teal
#define NotSet CRGB::Snow
#define solid 0
#define ColorWheel 1
#define Breath 2
#define Cardio 3
#define Rainbow 4
#define SpacklingRainbow 5
#define Confetti 6
#define Jungle 7


struct {
  uint8_t brightness = BRIGHTNESS;
  uint8_t gHue = 0;
  long gHuetiming = 0;
  long timing = 0;
  uint8_t state = 0;
} Dleds[NUM_LEDS];


struct {
  int beatcount = 0;
  int breathfader = 1;
} A_variables[NUM_LEDS];


int blackpoint = 240;


// the following lines define the index of loaded config to be used in the Dial App
#define knob1CW "the app is in charge of ths000"
#define knob1CCW "the app is in charge of ths001"
#define knob2CW "the app is in charge of ths002"
#define knob2CCW "the app is in charge of ths003"
#define captouch_singletap_con1 "the app is in charge of ths004"
#define captouch_singletap_con2 "the app is in charge of ths005"
#define captouch_doubletap_con1 "the app is in charge of ths006"
#define captouch_doubletap_con2 "the app is in charge of ths007"
#define captouch_shortpress "the app is in charge of ths008"
#define captouch_longpress "the app is in charge of ths009"
#define MK1tap_con1 "the app is in charge of ths010"
#define MK1tap_con2 "the app is in charge of ths011"
#define MK1hold "the app is in charge of ths012"
#define MK2tap_con1 "the app is in charge of ths013"
#define MK2tap_con2 "the app is in charge of ths014"
#define MK2hold "the app is in charge of ths015"
#define MK3tap_con1 "the app is in charge of ths016"
#define MK3tap_con2 "the app is in charge of ths017"
#define MK3hold "the app is in charge of ths018"
#define MK4tap_con1 "the app is in charge of ths019"
#define MK4tap_con2 "the app is in charge of ths020"
#define MK4hold "the app is in charge of ths021"
#define MK5tap_con1 "the app is in charge of ths022"
#define MK5tap_con2 "the app is in charge of ths023"
#define MK5hold "the app is in charge of ths024"
#define SPnav_tiltup "the app is in charge of ths025"
#define SPnav_tiltdown "the app is in charge of ths026"
#define SPnav_tiltright "the app is in charge of ths027"
#define SPnav_tiltleft "the app is in charge of ths028"
#define SPnav_slideup "the app is in charge of ths029"
#define SPnav_slidedown "the app is in charge of ths030"
#define SPnav_slideright "the app is in charge of ths031"
#define SPnav_slideleft "the app is in charge of ths032"


#define typekeystroke 1
#define typemousectrl 2
#define typekeystring 3
#define typespacenavraw 4






struct {
  String appname;  //application or configuration name
  int ID;  //application or configuration ID
  long appcolor;  //LED color assigned to the application or configuration
  byte appanimation;
  int knob1_res;  //knob1 resolution
  bool useapp_knob1_CW;
  char knob1_CW[3];  //knob1 clockwise shortcuts keys (application specific)
  bool useapp_knob1_CCW;
  char knob1_CCW[3];  //knob1 counter-clockwise shortcuts keys (application specific)
 /* int knob2_res;  //knob2 resolution
  bool useapp_knob2_CW;
  char knob2_CW[3];  //knob2 clockwise shortcuts keys (application specific)
  bool useapp_knob2_CCW;
  char knob2_CCW[3]; //knob2 counter-clockwise shortcuts keys (application specific)*/
  bool captouch_dualtapfunc[2];
  bool useapp_captouch_singletap;
  char singletap[3];  //capacitive touch singletap shortcuts keys (application specific)
  bool useapp_captouch_singletap2;
  char singletap2[3];
  bool useapp_captouch_doubletap;
  char doubletap[3];  //capacitive touch doubletap shortcuts keys (application specific)
  bool useapp_captouch_doubletap2;
  char doubletap2[3];
  bool useapp_captouch_shortpress;
  char shortpress[3];  //capacitive touch shortpress shortcuts keys (application specific)
  bool useapp_captouch_longpress;
  char longpress[3];  //capacitive touch longpress shortcuts keys (application specific)
  /*bool MK_dualtapfunc[5];
  bool MK_tapfunc[5];  //configures each macro key tap to function as a shortcut or to print out a predefined text
  bool MK_holdfunc[5];  //configures each macro hold key to function as a shortcut or to print out a predefined text
  long MK_colors[5];  //sets the individual colors of each macro key
  byte MK_animation[5];
  bool useapp_MK1_tap;
  char MK1_tap[3];  //macro key 1 tap shortcuts keys (application specific)
  bool useapp_MK1_tap2;
  char MK1_tap2[3];
  String MK1_taptext;  //macro key 1 tap predefined text
  bool useapp_MK1_hold;
  char MK1_hold[3];  //macro key 1 hold shortcuts keys (application specific)
  String MK1_holdtext;  //macro key 1 hold predefined text
  bool useapp_MK2_tap;
  char MK2_tap[3];  //macro key 2 tap shortcuts keys (application specific)
  bool useapp_MK2_tap2;
  char MK2_tap2[3];
  String MK2_taptext;  //macro key 2 tap predefined text
  bool useapp_MK2_hold;
  char MK2_hold[3];  //macro key 2 hold shortcuts keys (application specific)
  String MK2_holdtext;  //macro key 2 hold predefined text
  bool useapp_MK3_tap;
  char MK3_tap[3];  //macro key 3 tap shortcuts keys (application specific)
  bool useapp_MK3_tap2;
  char MK3_tap2[3];
  String MK3_taptext;  //macro key 3 tap predefined text
  bool useapp_MK3_hold;
  char MK3_hold[3];  //macro key 3 hold shortcuts keys (application specific)
  String MK3_holdtext;  //macro key 3 hold predefined text
  bool useapp_MK4_tap;
  char MK4_tap[3];  //macro key 4 tap shortcuts keys (application specific)
  bool useapp_MK4_tap2;
  char MK4_tap2[3];
  String MK4_taptext;  //macro key 4 tap predefined text
  bool useapp_MK4_hold;
  char MK4_hold[3];  //macro key 4 hold shortcuts keys (application specific)
  String MK4_holdtext;  //macro key 4 hold predefined text
  bool useapp_MK5_tap;
  char MK5_tap[3];  //macro key 5 tap shortcuts keys (application specific)
  bool useapp_MK5_tap2;
  char MK5_tap2[3];
  String MK5_taptext;  //macro key 5 tap predefined text
  bool useapp_MK5_hold;
  char MK5_hold[3];  //macro key 5 hold shortcuts keys (application specific)
  String MK5_holdtext;  //macro key 5 hold predefined text
  int spacenav_func;  //integer value for setting which CAD application the dial's space navigator works with.
  bool spacenav_continuousmode[2];
  bool useapp_spacenav_tiltup;
  char spacenav_tiltup[3];  //space navigator tilt up shortcut keys (application specific)
  bool useapp_spacenav_tiltdown;
  char spacenav_tiltdown[3];  //space navigator tilt down shortcut keys (application specific)
  bool useapp_spacenav_tiltright;
  char spacenav_tiltright[3];  //space navigator tilt right shortcut keys (application specific)
  bool useapp_spacenav_tiltleft;
  char spacenav_tiltleft[3];  //space navigator tilt left shortcut keys (application specific)
  bool useapp_spacenav_slideup;
  char spacenav_slideup[3];  //space navigator slide up shortcut keys (application specific)
  bool useapp_spacenav_slidedown;
  char spacenav_slidedown[3];  //space navigator slide down shortcut keys (application specific)
  bool useapp_spacenav_slideright;
  char spacenav_slideright[3];  //space navigator slide right shortcut keys (application specific)
  bool useapp_spacenav_slideleft;
  char spacenav_slideleft[3];  //space navigator slide left shortcut keys (application specific)*/

} appconfig[maxconfignum];

int activeappid = 0, prevactiveappid = 0, count1 = 0, count2 = 0, touch = 0;


int generalKeyPressInterval = 0;
int normalKeyInterval = 150;
bool captouchdualtapmodes[2] = { 0, 0 };
bool MK1dualtapmodes = 0;
bool MK2dualtapmodes = 0;
bool MK3dualtapmodes = 0;
bool MK4dualtapmodes = 0;
bool MK5dualtapmodes = 0;
int samplekey = 0;

byte inByte;         // incoming SerialUSB byte
String inString = "";



struct PacketStruct {
  bool updateConfigID = false;
  bool spacenavRawAvailable = false;
  bool inapp = false;
  byte inappcommandID = 0;
  bool keyormouse = false; //0 = keyboard, 1 = mouse
  bool is_string = false; //0 = simple keystroke, 1 = prepare to receive long string
  char keystroke;
  char mousecontrol;
  byte mouseX;
  byte mouseY;
  bool ctrlrelease = false;
  byte dialID = 1;
} dialPacket;

struct stringPrintingPacket {
  char stringdata[substrlen];
  bool endofstring = false;
} stringtoprint;

struct spacenavRawPacket {
  float SNG_x;
  float SNG_y;
  float SNG_rad;
  float SNP_x;
  float SNP_y;
  float SNP_rad;
} spacenavRaw;



struct W_adapterPacket {
  byte appID = 0; //from PC via Serial, is also ack payload
  bool active = true; //tells dial wireless adapter is available
} WAPacket; //from wireless adapter



long configidupdate = 0;
long timedIDupdate = 0;
long timedReconnect = 0;

int sleeptime = 30000;
volatile long lastinputtime = 0;

#define connecting 0
#define wired 1
#define wireless 2

int connectState = 0;

long mousereleaseinterval = 200;
long mousecontroltime = 0;
int mousemoveMul = 1;

void setup() {
  delay(5000);
  SerialUSB.begin(115200);
  //while(!SerialUSB){ SerialUSB.begin(115200);}
  setupCommunication();
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;


  ahmsville_dial.initialize_ahmsvilleDial();
  Load_Configurations();
  updateknobresolution();
}

void loop() {

  //long fftime = millis();
  //SerialUSB.println(fftime);
  //SerialUSB.println(lastinputtime);


  sleepMode();
  getinputs(); //get and process input
  connecttoPC();
  updatedialLeds();

  mouserelease();

  // SerialUSB.println(activeappid);
  // SerialUSB.println((millis() - fftime));

}

void sleepMode() {
  if ((millis() - lastinputtime) > sleeptime || touch == 5) { //no interraction with dial for a set time
    if (connectState == wireless || touch == 5) { //wireless mode
      DeactivateDial(true);
      ahmsville_dial.setsleep(true);
      while (ahmsville_dial.readsleep()) { //turn dial to wake
        // SerialUSB.println("Sleeping"); //sleepmode
        ahmsville_dial.knob();
      }
      // DeactivateDial();
      lastinputtime = millis();
      //digitalWrite(30, HIGH); //turn off Radio and Motion tracker
      //delay(500);
      //setupCommunication();
    }
  }
}


void setmousemoveMul(float value) {
  mousemoveMul = (((100.00 / 15.00) * value) / 100.00) * 20.00; //  (((100%/maxradii)*value)/100%) * maxallowedmovemul
}

void getinputs() {
 touch = ahmsville_dial.capTouch();  //detecting capacitive touch (returns integer 1 - 4 or 0 when no touch is detected)
   while(touch == -1){
    touch = ahmsville_dial.capTouch(); 
   }
  count1 = ahmsville_dial.knob();  //detecting knob rotations (returns a signed integer in relation to the direction of the rotation)
  // SerialUSB.println(count1);
 
 	/****************************************************************CAPTOUCH******************************************************************/
	if (touch == 1) { //cap touch is singletap
		if (appconfig[activeappid].captouch_dualtapfunc[0] == true) { //if dual shortcut is enabled for macro key1
			if (captouchdualtapmodes[0] == 0) { //use macro key1 shortcut1 / check previously used shortcut combo...
				if (appconfig[activeappid].useapp_captouch_singletap == false) { //if dial is not set to use app for this dial input - talk to PC directly via keyboard and mouse
					for (int i = 0; i < 3; i++) { //loop through shortcut keys
						if (appconfig[activeappid].singletap[i] != nullchar) {
							if (shortcutcommandType(appconfig[activeappid].singletap[i]) == true) {
								if (appconfig[activeappid].singletap[i + 1] == nullchar || i == 2) { //is the last or only keystroke use keyboard.write
									processDialInput(typekeystroke, appconfig[activeappid].singletap[i], true, "", 0, false);
								}
								else {
									processDialInput(typekeystroke, appconfig[activeappid].singletap[i], false, "", 0, false);
									if (!isSpecialKey(appconfig[activeappid].singletap[i])) { //is not Special key release immediately
										processDialInput(0, '0', false, "", 1, false);
										if (i < 2 && appconfig[activeappid].singletap[i + 1] != nullchar) { //delay between normal key combinations
											delay(normalKeyInterval);
										}
									}
									//Keyboard.press(appconfig[activeappid].singletap[i]);
									delay(generalKeyPressInterval);
								}
							}
							else {
								processDialInput(typemousectrl, appconfig[activeappid].singletap[i], false, "", 0, false);
								//mouseControl(appconfig[activeappid].singletap[i]);
							}
						}
					}
					processDialInput(0, '0', false, "", 1, false);

				}
				else { //talk to app instead
					processDialInput(0, '0', false, captouch_singletap_con1, 0, true);
					// SerialUSB.print(captouch_singletap_con1);
				}
				captouchdualtapmodes[0] = !captouchdualtapmodes[0];  //invert for the next tap
			}
			else if (captouchdualtapmodes[0] == 1) {  //use macro key1 shortcut2
				if (appconfig[activeappid].useapp_captouch_singletap2 == false) { //if dial is not set to use app for this dial input - talk to PC directly via keyboard and mouse
					for (int i = 0; i < 3; i++) { //loop through shortcut keys
						if (appconfig[activeappid].singletap2[i] != nullchar) {
							if (shortcutcommandType(appconfig[activeappid].singletap2[i]) == true) {
								if (appconfig[activeappid].singletap2[i + 1] == nullchar || i == 2) { //is the last or only keystroke use keyboard.write
									processDialInput(typekeystroke, appconfig[activeappid].singletap2[i], true, "", 0, false);
								}
								else {
									processDialInput(typekeystroke, appconfig[activeappid].singletap2[i], false, "", 0, false);
									if (!isSpecialKey(appconfig[activeappid].singletap2[i])) { //is not Special key release immediately
										processDialInput(0, '0', false, "", 1, false);
										if (i < 2 && appconfig[activeappid].singletap2[i + 1] != nullchar) { //delay between normal key combinations
											delay(normalKeyInterval);
										}
									}
									//Keyboard.press(appconfig[activeappid].singletap[i]);
									delay(generalKeyPressInterval);
								}
							}
							else {
								processDialInput(typemousectrl, appconfig[activeappid].singletap2[i], false, "", 0, false);
								// mouseControl(appconfig[activeappid].singletap2[i]);
							}
						}
					}
					processDialInput(0, '0', false, "", 1, false);

				}
				else { //talk to app instead
					processDialInput(0, '0', false, captouch_singletap_con2, 0, true);
					//SerialUSB.print(captouch_singletap_con2);
				}
				captouchdualtapmodes[0] = !captouchdualtapmodes[0];  //invert for the next tap
			}
		}
		else {
			if (appconfig[activeappid].useapp_captouch_singletap == false) { //if dial is not set to use app for this dial input - talk to PC directly via keyboard and mouse
				for (int i = 0; i < 3; i++) { //loop through shortcut keys
					if (appconfig[activeappid].singletap[i] != nullchar) {
						if (shortcutcommandType(appconfig[activeappid].singletap[i]) == true) {
							if (appconfig[activeappid].singletap[i + 1] == nullchar || i == 2) { //is the last or only keystroke use keyboard.write
								processDialInput(typekeystroke, appconfig[activeappid].singletap[i], true, "", 0, false);
							}
							else {
								processDialInput(typekeystroke, appconfig[activeappid].singletap[i], false, "", 0, false);
								if (!isSpecialKey(appconfig[activeappid].singletap[i])) { //is not Special key release immediately
									processDialInput(0, '0', false, "", 1, false);
									if (i < 2 && appconfig[activeappid].singletap[i + 1] != nullchar) { //delay between normal key combinations
										delay(normalKeyInterval);
									}
								}
								//Keyboard.press(appconfig[activeappid].singletap[i]);
								delay(generalKeyPressInterval);
							}
						}
						else {
							processDialInput(typemousectrl, appconfig[activeappid].singletap[i], false, "", 0, false);
							//  mouseControl(appconfig[activeappid].singletap[i]);
						}
					}
				}
				processDialInput(0, '0', false, "", 1, false);

			}
			else { //talk to app instead
				processDialInput(0, '0', false, captouch_singletap_con1, 0, true);
				// SerialUSB.print(captouch_singletap_con1);
			}
		}
	}
	/*-----------------------------------------------------------------------------------*/
	else if (touch == 2) { //cap touch is doubletap
		if (appconfig[activeappid].captouch_dualtapfunc[1] == true) { //if dual shortcut is enabled for macro key1
			if (captouchdualtapmodes[1] == 0) { //use macro key1 shortcut1 / check previously used shortcut combo...
				if (appconfig[activeappid].useapp_captouch_doubletap == false) { //if dial is not set to use app for this dial input - talk to PC directly via keyboard and mouse
					for (int i = 0; i < 3; i++) { //loop through shortcut keys
						if (appconfig[activeappid].doubletap[i] != nullchar) {
							if (shortcutcommandType(appconfig[activeappid].doubletap[i]) == true) {
								if (appconfig[activeappid].doubletap[i + 1] == nullchar || i == 2) { //is the last or only keystroke use keyboard.write
									processDialInput(typekeystroke, appconfig[activeappid].doubletap[i], true, "", 0, false);
								}
								else {
									processDialInput(typekeystroke, appconfig[activeappid].doubletap[i], false, "", 0, false);
									if (!isSpecialKey(appconfig[activeappid].doubletap[i])) { //is not Special key release immediately
										processDialInput(0, '0', false, "", 1, false);
										if (i < 2 && appconfig[activeappid].doubletap[i + 1] != nullchar) { //delay between normal key combinations
											delay(normalKeyInterval);
										}
									}
									//Keyboard.press(appconfig[activeappid].doubletap[i]);
									delay(generalKeyPressInterval);
								}
							}
							else {
								processDialInput(typemousectrl, appconfig[activeappid].doubletap[i], false, "", 0, false);
								//mouseControl(appconfig[activeappid].doubletap[i]);
							}
						}
					}
					processDialInput(0, '0', false, "", 1, false);

				}
				else { //talk to app instead
					processDialInput(0, '0', false, captouch_doubletap_con1, 0, true);
					// SerialUSB.print(captouch_doubletap_con1);
				}
				captouchdualtapmodes[1] = !captouchdualtapmodes[1];  //invert for the next tap
			}
			else if (captouchdualtapmodes[1] == 1) {  //use macro key1 shortcut2
				if (appconfig[activeappid].useapp_captouch_doubletap2 == false) { //if dial is not set to use app for this dial input - talk to PC directly via keyboard and mouse
					for (int i = 0; i < 3; i++) { //loop through shortcut keys
						if (appconfig[activeappid].doubletap2[i] != nullchar) {
							if (shortcutcommandType(appconfig[activeappid].doubletap2[i]) == true) {
								if (appconfig[activeappid].doubletap2[i + 1] == nullchar || i == 2) { //is the last or only keystroke use keyboard.write
									processDialInput(typekeystroke, appconfig[activeappid].doubletap2[i], true, "", 0, false);
								}
								else {
									processDialInput(typekeystroke, appconfig[activeappid].doubletap2[i], false, "", 0, false);
									if (!isSpecialKey(appconfig[activeappid].doubletap2[i])) { //is not Special key release immediately
										processDialInput(0, '0', false, "", 1, false);
										if (i < 2 && appconfig[activeappid].doubletap2[i + 1] != nullchar) { //delay between normal key combinations
											delay(normalKeyInterval);
										}
									}
									//Keyboard.press(appconfig[activeappid].doubletap[i]);
									delay(generalKeyPressInterval);
								}
							}
							else {
								processDialInput(typemousectrl, appconfig[activeappid].doubletap2[i], false, "", 0, false);
								// mouseControl(appconfig[activeappid].doubletap2[i]);
							}
						}
					}
					processDialInput(0, '0', false, "", 1, false);

				}
				else { //talk to app instead
					processDialInput(0, '0', false, captouch_doubletap_con2, 0, true);
					//SerialUSB.print(captouch_doubletap_con2);
				}
				captouchdualtapmodes[1] = !captouchdualtapmodes[1];  //invert for the next tap
			}
		}
		else {
			if (appconfig[activeappid].useapp_captouch_doubletap == false) { //if dial is not set to use app for this dial input - talk to PC directly via keyboard and mouse
				for (int i = 0; i < 3; i++) { //loop through shortcut keys
					if (appconfig[activeappid].doubletap[i] != nullchar) {
						if (shortcutcommandType(appconfig[activeappid].doubletap[i]) == true) {
							if (appconfig[activeappid].doubletap[i + 1] == nullchar || i == 2) { //is the last or only keystroke use keyboard.write
								processDialInput(typekeystroke, appconfig[activeappid].doubletap[i], true, "", 0, false);
							}
							else {
								processDialInput(typekeystroke, appconfig[activeappid].doubletap[i], false, "", 0, false);
								if (!isSpecialKey(appconfig[activeappid].doubletap[i])) { //is not Special key release immediately
									processDialInput(0, '0', false, "", 1, false);
									if (i < 2 && appconfig[activeappid].doubletap[i + 1] != nullchar) { //delay between normal key combinations
										delay(normalKeyInterval);
									}
								}
								//Keyboard.press(appconfig[activeappid].doubletap[i]);
								delay(generalKeyPressInterval);
							}
						}
						else {
							processDialInput(typemousectrl, appconfig[activeappid].doubletap[i], false, "", 0, false);
							//  mouseControl(appconfig[activeappid].doubletap[i]);
						}
					}
				}
				processDialInput(0, '0', false, "", 1, false);

			}
			else { //talk to app instead
				processDialInput(0, '0', false, captouch_doubletap_con1, 0, true);
				// SerialUSB.print(captouch_doubletap_con1);
			}
		}
	}


	/*-----------------------------------------------------------------------------------*/
	else if (touch == 3) { //cap touch is shortpress
		if (appconfig[activeappid].useapp_captouch_shortpress == false) { //if dial is not set to use app for this dial input - talk to PC directly via keyboard and mouse
			for (int i = 0; i < 3; i++) { //loop through shortcut keys
				if (appconfig[activeappid].shortpress[i] != nullchar) {
					if (shortcutcommandType(appconfig[activeappid].shortpress[i]) == true) {
						if (appconfig[activeappid].shortpress[i + 1] == nullchar || i == 2) { //is the last or only keystroke use keyboard.write
							processDialInput(typekeystroke, appconfig[activeappid].shortpress[i], true, "", 0, false);
						}
						else {
							processDialInput(typekeystroke, appconfig[activeappid].shortpress[i], false, "", 0, false);
							if (!isSpecialKey(appconfig[activeappid].shortpress[i])) { //is not Special key release immediately
								processDialInput(0, '0', false, "", 1, false);
								if (i < 2 && appconfig[activeappid].shortpress[i + 1] != nullchar) { //delay between normal key combinations
									delay(normalKeyInterval);
								}
							}
							//Keyboard.press(appconfig[activeappid].shortpress[i]);
							delay(generalKeyPressInterval);
						}
					}
					else {
						processDialInput(typemousectrl, appconfig[activeappid].shortpress[i], false, "", 0, false);
						//mouseControl(appconfig[activeappid].shortpress[i]);
					}
				}
			}
			processDialInput(0, '0', false, "", 1, false);

		}
		else { //talk to app instead
			processDialInput(0, '0', false, captouch_shortpress, 0, true);
			//SerialUSB.print(captouch_shortpress);
		}
	}

	/*-----------------------------------------------------------------------------------*/
	else if (touch == 4) { //cap touch is longpress
		if (appconfig[activeappid].useapp_captouch_longpress == false) { //if dial is not set to use app for this dial input - talk to PC directly via keyboard and mouse
			for (int i = 0; i < 3; i++) { //loop through shortcut keys
				if (appconfig[activeappid].longpress[i] != nullchar) {
					if (shortcutcommandType(appconfig[activeappid].longpress[i]) == true) {
						if (appconfig[activeappid].longpress[i + 1] == nullchar || i == 2) { //is the last or only keystroke use keyboard.write
							processDialInput(typekeystroke, appconfig[activeappid].longpress[i], true, "", 0, false);
						}
						else {
							processDialInput(typekeystroke, appconfig[activeappid].longpress[i], false, "", 0, false);
							if (!isSpecialKey(appconfig[activeappid].longpress[i])) { //is not Special key release immediately
								processDialInput(0, '0', false, "", 1, false);
								if (i < 2 && appconfig[activeappid].longpress[i + 1] != nullchar) { //delay between normal key combinations
									delay(normalKeyInterval);
								}
							}
							//Keyboard.press(appconfig[activeappid].longpress[i]);
							delay(generalKeyPressInterval);
						}
					}
					else {
						processDialInput(typemousectrl, appconfig[activeappid].longpress[i], false, "", 0, false);
						//mouseControl(appconfig[activeappid].longpress[i]);
					}
				}
			}
			processDialInput(0, '0', false, "", 1, false);

		}
		else { //talk to app instead
			processDialInput(0, '0', false, captouch_longpress, 0, true);
			//SerialUSB.print(captouch_longpress);
		}
	}
  /**************************************************************ROTARY ENCODERS*********************************************************/
  if (count1 > 0) {  //knob1 clockwise
    if (appconfig[activeappid].useapp_knob1_CW == false) { //if dial is not set to use app for this dial input - talk to PC directly via keyboard and mouse
      for (int i = 0; i < count1; i++) {
        for (int i = 0; i < 3; i++) { //loop through shortcut keys
          if (appconfig[activeappid].knob1_CW[i] != nullchar) {
            if (shortcutcommandType(appconfig[activeappid].knob1_CW[i]) == true) {
              processDialInput(typekeystroke, appconfig[activeappid].knob1_CW[i], false, "", 0, false);
              delay(generalKeyPressInterval);
            }
            else {
              processDialInput(typemousectrl, appconfig[activeappid].knob1_CW[i], false, "", 0, false);
            }
          }
        }
        if (appconfig[activeappid].knob1_res != 0) { //granular res is set, dont release immediately
          mousecontroltime = millis();
        }
        else {
          processDialInput(0, '0', false, "", 1, false);
        }

      }
    }
    else { //talk to app instead
      for (int i = 0; i < count1; i++) {
        processDialInput(0, '0', false, knob1CW, 0, true);
      }
    }
  }
  else if (count1 < 0) {  //knob1 counter-clockwise
    if (appconfig[activeappid].useapp_knob1_CCW == false) { //if dial is not set to use app for this dial input - talk to PC directly via keyboard and mouse
      for (int i = 0; i > count1; i--) {
        for (int i = 0; i < 3; i++) { //loop through shortcut keys
          if (appconfig[activeappid].knob1_CCW[i] != nullchar) {
            if (shortcutcommandType(appconfig[activeappid].knob1_CCW[i]) == true) {
              processDialInput(typekeystroke, appconfig[activeappid].knob1_CCW[i], false, "", 0, false);
              delay(generalKeyPressInterval);
            }
            else {
              processDialInput(typemousectrl, appconfig[activeappid].knob1_CCW[i], false, "", 0, false);
            }
          }
        }
        if (appconfig[activeappid].knob1_res != 0) { //granular res is set, dont release immediately
          mousecontroltime = millis();
        }
        else {
          processDialInput(0, '0', false, "", 1, false);
        }
      }
    }
    else { //talk to app instead
      for (int i = 0; i > count1; i--) {
        processDialInput(0, '0', false, knob1CCW, 0, true);
      }
    }
  }
  


  ahmsville_dial.normalize();

}

void restartDial() {
  mouserelease();
  cleardialpacket();
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(200);
  }
  digitalWrite(30, HIGH);
  delay(500);
  digitalWrite(30, LOW);
  delay(2000);

  setupCommunication();
  ahmsville_dial.initialize_ahmsvilleDial();
  Load_Configurations();
  updateknobresolution();
  resetledVariables();
  activeappid = 0;
}


void connecttoPC() {
  if (connectState == wired) { //already connected via usb
    // reply only when you receive data:
    if (SerialUSB.available() > 0) {
      inByte = SerialUSB.read();
      inString += (char)inByte;

      if (inString != "") {
        if (inString != "a") { //change app byte
          if (inString == "r") {
            //reset dial
            restartDial();
          } else {
            if (inString.toInt() < maxconfignum) {
              activeappid = inString.toInt();
              prevactiveappid = activeappid;
              updateknobresolution();
              resetledVariables();
            }
          }
        }
        else if (inString == "a") {  //connection byte
          SerialUSB.print("******************Base Variant"); //reply with your dial type
        }

      }
    }
  }
  else if (connectState == wireless) { //already connected via wireless adapter
    if ((millis() - timedIDupdate) > 1000) { //update ID from wireless adapter every few secs
      sendDialPacket();
    }
  }


  if ((millis() - timedReconnect) > 5000) { //reconnect every few secs
    //reset connection variable
    // activeappid = 0;
    connectState = connecting;
    WAPacket.active = false;
    SerialUSB.print("******************Base Variant"); //check if pc is still connected
    if (!sendDialPacket()) {
      connectState = wired;
      timedReconnect = millis();
    }
    //sendDialPacket(); //check if wireless adapter is still connected
    if (SerialUSB.available() > 0) {  //pc replied
      /******************************************/
      inByte = SerialUSB.read();
      inString += (char)inByte;

      if (inString != "") {
        if (inString != "a") { //change app byte
          if (inString == "r") {
            //reset dial
            restartDial();
          } else {
            if (inString.toInt() < maxconfignum) {
              activeappid = inString.toInt();
              prevactiveappid = activeappid;
              updateknobresolution();
              resetledVariables();
            }
          }
        }
        else if (inString == "a") {  //connection byte
          SerialUSB.print("******************Base Variant"); //reply with your dial type
        }

      }
      /******************************************/

      connectState = wired;
      timedReconnect = millis();
    }
    else if (WAPacket.active == true) { //wireless adapter replied
      connectState = wireless;
      timedReconnect = millis();
    }
    else {


    }

  }
  inString = "";
}

bool shortcutcommandType(char command) {
  if (command == MOUSE_LEFT || command == MOUSE_RIGHT || command == MOUSE_MIDDLE || command == MOUSEMOVE_UP || command == MOUSEMOVE_DOWN || command == MOUSEMOVE_LEFT || command == MOUSEMOVE_RIGHT) {
    return false;
  }
  else {
    return true;
  }
}

bool isSpecialKey(char command) {  //vheck if key is a special key -- employ diff control release
  if (command == MOUSE_LEFT || command == MOUSE_RIGHT || command == MOUSE_MIDDLE || command == MOUSEMOVE_UP || command == MOUSEMOVE_DOWN || command == MOUSEMOVE_LEFT || command == MOUSEMOVE_RIGHT || command == KEY_SPACEBAR) {
    return true;
  }
  if (command <= 251 && command >= 128) {
    return true;
  }
  else {
    return false;
  }
}

void mouseControl(char command, int mouse_x, int mouse_y) {
  if (command == MOUSE_LEFT || command == MOUSE_RIGHT || command == MOUSE_MIDDLE) {
    Mouse.press(command);
  }
  else {
    Mouse.move(mouse_x * mousemoveMul, mouse_y * mousemoveMul, 0);
  }
}

void resetledVariables() {
  for (int i = 0; i < NUM_LEDS; i++) {
    Dleds[i].brightness = BRIGHTNESS;
    Dleds[i].gHue = 0;
    Dleds[i].gHuetiming = 0;
    Dleds[i].timing = 0;
    Dleds[i].state = 0;
    A_variables[i].beatcount = 0;
    A_variables[i].breathfader = 1;
  }
}

void colorwheel(int ledindex) {
  if ((millis() - Dleds[ledindex].gHuetiming) > 10) { // check timing
    Dleds[ledindex].gHue++;  // slowly cycle the "base color" through the rainbow
    Dleds[ledindex].gHuetiming = millis();
    //leds[ledindex] = ColorFromPalette( currentPalette, Dleds[ledindex].gHue, Dleds[ledindex].brightness, currentBlending);
    leds[ledindex] = CHSV(Dleds[ledindex].gHue, 255, Dleds[ledindex].brightness + 40);
  }
}

void breath(int ledindex, long color) {     //function to fade from color1 to color2.

  if (color != ALLColors) { // solid color is selected
    if (Dleds[ledindex].state == 0) {
      if (A_variables[ledindex].breathfader < blackpoint) {
        leds[ledindex] = color;
        leds[ledindex].fadeToBlackBy(A_variables[ledindex].breathfader);
        // delay(2);
        A_variables[ledindex].breathfader += 2;
      }
      else if (A_variables[ledindex].breathfader >= blackpoint) {
        Dleds[ledindex].state = 1;
        A_variables[ledindex].breathfader = blackpoint - 1;
      }
    }
    else if (Dleds[ledindex].state == 1) {    //fadeup color 2
      if (A_variables[ledindex].breathfader > 0) {
        leds[ledindex] = color;
        leds[ledindex].fadeToBlackBy(A_variables[ledindex].breathfader);
        //delay(2);
        A_variables[ledindex].breathfader -= 2;
      }
      else if (A_variables[ledindex].breathfader <= 0) {
        Dleds[ledindex].state = 0;
        A_variables[ledindex].breathfader = 1;
      }
    }
  }
  else { //all colors
    if (Dleds[ledindex].state == 0) {
      if (A_variables[ledindex].breathfader < blackpoint) {
        leds[ledindex] = CHSV(Dleds[ledindex].gHue, 255, A_variables[ledindex].breathfader);
        // leds[ledindex] = ColorFromPalette( currentPalette, Dleds[ledindex].gHue, A_variables[ledindex].breathfader, currentBlending);
        // delay(2);
        A_variables[ledindex].breathfader += 2;
      }
      else if (A_variables[ledindex].breathfader >= blackpoint) {
        Dleds[ledindex].state = 1;
        A_variables[ledindex].breathfader = blackpoint - 1;
      }
    }
    else if (Dleds[ledindex].state == 1) {    //fadeup color 2
      if (A_variables[ledindex].breathfader > 0) {
        leds[ledindex] = CHSV(Dleds[ledindex].gHue, 255, A_variables[ledindex].breathfader);
        // leds[ledindex] = ColorFromPalette( currentPalette, Dleds[ledindex].gHue, A_variables[ledindex].breathfader, currentBlending);
        // delay(2);
        A_variables[ledindex].breathfader -= 2;
      }
      else if (A_variables[ledindex].breathfader <= 0) {
        Dleds[ledindex].state = 0;
        A_variables[ledindex].breathfader = 1;
      }
    }
  }
  if ((millis() - Dleds[ledindex].gHuetiming) > 10) { // check timing
    Dleds[ledindex].gHue++;  // slowly cycle the "base color" through the rainbow
    Dleds[ledindex].gHuetiming = millis();
  }
}

void cardio(int ledindex, long color) {
  if (color != ALLColors) { // solid color is selected
    if (Dleds[ledindex].state == 0) {
      nblend(leds[ledindex], CRGB::Black, 10);
      if ((millis() - Dleds[ledindex].timing) > 200) { // check timing
        Dleds[ledindex].state = 1;
        Dleds[ledindex].timing = millis();
      }

    }
    if (Dleds[ledindex].state == 1) {
      nblend(leds[ledindex], color, 10);
      if ((millis() - Dleds[ledindex].timing) > 200) { // check timing
        Dleds[ledindex].state = 2;
        Dleds[ledindex].timing = millis();
        A_variables[ledindex].beatcount += 1;
      }

    }
    if (Dleds[ledindex].state == 2) {

      if ((millis() - Dleds[ledindex].timing) > 150) { // check timing
        if (A_variables[ledindex].beatcount <= 1) {
          Dleds[ledindex].state = 0;
          Dleds[ledindex].timing = millis();
        }
        else {
          Dleds[ledindex].state = 3;
          Dleds[ledindex].timing = millis();
        }

      }
    }
    if (Dleds[ledindex].state == 3) {
      nblend(leds[ledindex], color, 5);
      if ((millis() - Dleds[ledindex].timing) > 600) { // check timing
        Dleds[ledindex].state = 0;
        Dleds[ledindex].timing = millis();
        A_variables[ledindex].beatcount = 0;
      }
    }
  }
  else { //all colors
    if (Dleds[ledindex].state == 0) {
      if ((millis() - Dleds[ledindex].gHuetiming) > 10) {
        Dleds[ledindex].gHue++;  // slowly cycle the "base color" through the rainbow
        Dleds[ledindex].gHuetiming = millis();
      }
      //leds[ledindex] =  CHSV( Dleds[ledindex].gHue, 255, Dleds[ledindex].brightness);
      leds[ledindex] = ColorFromPalette(currentPalette, Dleds[ledindex].gHue, Dleds[ledindex].brightness, currentBlending);
      if (Dleds[ledindex].brightness > 0) {
        Dleds[ledindex].brightness -= 3;
      }
      if ((millis() - Dleds[ledindex].timing) > 200 && Dleds[ledindex].brightness <= 30) { // check timing
        Dleds[ledindex].state = 1;
        Dleds[ledindex].timing = millis();
      }

    }
    if (Dleds[ledindex].state == 1) {
      if ((millis() - Dleds[ledindex].gHuetiming) > 10) {
        Dleds[ledindex].gHue++;  // slowly cycle the "base color" through the rainbow
        Dleds[ledindex].gHuetiming = millis();
      }
      //leds[ledindex] =  CHSV( Dleds[ledindex].gHue, 255, Dleds[ledindex].brightness);
      leds[ledindex] = ColorFromPalette(currentPalette, Dleds[ledindex].gHue, Dleds[ledindex].brightness, currentBlending);
      if (Dleds[ledindex].brightness < BRIGHTNESS) {
        Dleds[ledindex].brightness += 3;
      }
      if ((millis() - Dleds[ledindex].timing) > 200 && Dleds[ledindex].brightness >= BRIGHTNESS) { // check timing
        Dleds[ledindex].state = 2;
        Dleds[ledindex].timing = millis();
        A_variables[ledindex].beatcount += 1;
      }

    }
    if (Dleds[ledindex].state == 2) {

      if ((millis() - Dleds[ledindex].timing) > 150) { // check timing
        if (A_variables[ledindex].beatcount <= 1) {
          Dleds[ledindex].state = 0;
          Dleds[ledindex].timing = millis();
        }
        else {
          Dleds[ledindex].state = 3;
          Dleds[ledindex].timing = millis();
        }

      }
    }
    if (Dleds[ledindex].state == 3) {
      Dleds[ledindex].brightness += 1;
      leds[ledindex] = ColorFromPalette(currentPalette, Dleds[ledindex].gHue, Dleds[ledindex].brightness, currentBlending);
      if ((millis() - Dleds[ledindex].timing) > 600) { // check timing
        Dleds[ledindex].state = 0;
        Dleds[ledindex].timing = millis();
        A_variables[ledindex].beatcount = 0;
        Dleds[ledindex].brightness = BRIGHTNESS;
      }
    }
  }

}

void rainbow(int ledindex) {
  // FastLED's built-in rainbow generator
  if ((millis() - Dleds[ledindex].gHuetiming) > 10) {

    fill_rainbow(leds, NUM_LEDS, Dleds[ledindex].gHue++, 7);
    if ((millis() - Dleds[ledindex].gHuetiming) > 20) { // check timing
      Dleds[ledindex].gHue++;  // slowly cycle the "base color" through the rainbow
      Dleds[ledindex].gHuetiming = millis();
      leds[ledindex] = ColorFromPalette(currentPalette, Dleds[ledindex].gHue, Dleds[ledindex].brightness, currentBlending);
    }
    Dleds[ledindex].gHuetiming = millis();
  }
}
void sparklingrainbow(int ledindex) {
  if ((millis() - Dleds[ledindex].gHuetiming) > 10) {
    rainbow(ledindex);
    if (random8() < 80) {
      leds[random16(NUM_LEDS)] += CRGB::White;
    }
    Dleds[ledindex].gHuetiming = millis();
  }
}

void confetti(int ledindex)
{
  // random colored speckles that blink in and fade smoothly
  if ((millis() - Dleds[ledindex].gHuetiming) > 10) {
    fadeToBlackBy(leds, NUM_LEDS, 10);
    int pos = random16(NUM_LEDS);
    leds[pos] += CHSV(Dleds[ledindex].gHue++ + random8(64), 200, 255);
    Dleds[ledindex].gHuetiming = millis();
  }
}

void jungle(int ledindex)
{
  // eight colored dots, weaving in and out of sync with each other
  if ((millis() - Dleds[ledindex].gHuetiming) > 10) {
    fadeToBlackBy(leds, NUM_LEDS, 20);
    byte dothue = 0;
    for (int i = 0; i < 8; i++) {
      leds[beatsin16(i + 7, 0, NUM_LEDS - 1)] |= CHSV(dothue, 200, 255);
      dothue += 32;
    }
    Dleds[ledindex].gHuetiming = millis();
  }
}

void updatedialLeds() {

  // blanket type animation is set to appcolor
  if (appconfig[activeappid].appanimation == Rainbow) {
    rainbow(0);
  }
  else if (appconfig[activeappid].appanimation == SpacklingRainbow) {
    sparklingrainbow(0);
  }
  else if (appconfig[activeappid].appanimation == Confetti) {
    confetti(0);
  }
  else if (appconfig[activeappid].appanimation == Jungle) {
    jungle(0);
  }
  else {
    /*****************************appled***************************/
    if (appconfig[activeappid].appanimation == solid) { //no animation is set
      if (appconfig[activeappid].appcolor == ALLColors || appconfig[activeappid].appcolor == NotSet) { //set to all colors or not set at all
        for (int i = 0; i < (NUM_LEDS - MK_LEDS); i++) {
          colorwheel(i);
        }
      }
      else {
        for (int i = 0; i < (NUM_LEDS - MK_LEDS); i++) {
          leds[i] = appconfig[activeappid].appcolor;
        }
      }
    }
    else if (appconfig[activeappid].appanimation == ColorWheel) { //Rainbow animation is set
      for (int i = 0; i < (NUM_LEDS - MK_LEDS); i++) {
        colorwheel(i);
      }
    }
    else if (appconfig[activeappid].appanimation == Breath) { //breath animation is set
      if (appconfig[activeappid].appcolor == ALLColors || appconfig[activeappid].appcolor == NotSet) { //set to all colors or not set at all
        for (int i = 0; i < (NUM_LEDS - MK_LEDS); i++) {
          breath(i, ALLColors);
        }
      }
      else { //set to a specific color
        for (int i = 0; i < (NUM_LEDS - MK_LEDS); i++) {
          breath(i, appconfig[activeappid].appcolor);
        }
      }
    }
    else if (appconfig[activeappid].appanimation == Cardio) { // cardio animation is set
      if (appconfig[activeappid].appcolor == ALLColors || appconfig[activeappid].appcolor == NotSet) { //set to all colors or not set at all
        for (int i = 0; i < (NUM_LEDS - MK_LEDS); i++) {
          cardio(i, ALLColors);
        }
      }
      else { //set to a specific color
        for (int i = 0; i < (NUM_LEDS - MK_LEDS); i++) {
          cardio(i, appconfig[activeappid].appcolor);
        }
      }
    }

    /**************************************************************************************************/

  
  }
  FastLED.show();

}


/******************************************************************************************************************************************************************************************************************
*******************************************************************************************************************************************************************************************************************
*******************************************************************************************************************************************************************************************************************/

void DeactivateDial(bool act) {
  if (act) {
    for (int i = 0; i < NUM_LEDS; i++) { // update macro key leds
      leds[i + 1] = CRGB::Black;
      processDialInput(0, '0', false, "", 1, false);
      delay(150);
      FastLED.show();
    }
  }


}






void setupCommunication() {

  radio.begin();
  radio.setPALevel(RF24_PA_HIGH);
  radio.enableAckPayload();

  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1, addresses[1]);

  radio.stopListening();
  radio.setChannel(63);
}

void testtransfer() {
  dialPacket.is_string = true;
  sendDialPacket();
  //breakandsendstring(originalstring);
}

void breakandsendstring(String longstring) {
  int stringlen = longstring.length();  //get string lenght
  int transmitdelay = 5;
  bool nofailure = true;
  while (stringlen > (substrlen - 1) && nofailure == true) { //sring lenght is still greater than 25
    String stringportion = longstring.substring(stringlen - (substrlen - 1)); //get the last 25 char
    longstring.replace(stringportion, "");  //replace the last 25 char
    nofailure = sendstring(stringportion);  //send the aquired 25 char
    delay(transmitdelay); //transmit delay
    stringlen = longstring.length();
  }
  stringlen = longstring.length();
  if (stringlen <= substrlen) {
    stringtoprint.endofstring = true;
    memset(stringtoprint.stringdata, 0, substrlen);
    nofailure = sendstring(longstring); //send remaining string
    delay(transmitdelay); //transmit delay
    stringtoprint.endofstring = false;
  }


}
bool sendstring(String strdata) {
  //SerialUSB.print(strdata);
  //SerialUSB.print("\t");
  strdata.toCharArray(stringtoprint.stringdata, substrlen);
  for (int i = 0; i < substrlen; i++) {
    // SerialUSB.print(stringtoprint.stringdata[i]);
  }
  //SerialUSB.print("\t");
  //SerialUSB.println("");
  radio.stopListening();                                    // First, stop listening so we can talk.
  long stringsendtimeout = micros();
  bool stringportionsent = false;
  while (micros() - (stringsendtimeout) < 3000 && stringportionsent == false) {
    if (!radio.write(&stringtoprint, sizeof(stringtoprint))) {
      SerialUSB.println("failed");
    }
    else {
      while (radio.available()) {
        radio.read(&WAPacket, sizeof(WAPacket));
        if (WAPacket.appID < maxconfignum) {
          if (WAPacket.appID != prevactiveappid) { //new app id
            activeappid = WAPacket.appID;
            prevactiveappid = activeappid;
            updateknobresolution();
            resetledVariables();
          }
        }
        stringportionsent = true;
      }
    }
  }
  return stringportionsent;

}

bool sendDialPacket() {
  radio.stopListening();                                    // First, stop listening so we can talk.
  int stringsendtimeout = micros();
  if (!radio.write(&dialPacket, sizeof(dialPacket))) {
    // SerialUSB.println("failed");
    return false;
  }
  else {
    while (radio.available()) {
      radio.read(&WAPacket, sizeof(WAPacket));
      if (WAPacket.appID < maxconfignum) {
        if (WAPacket.appID != prevactiveappid) { //new app id
          activeappid = WAPacket.appID;
          prevactiveappid = activeappid;
          updateknobresolution();
          resetledVariables();
        }
      }else{
        if(WAPacket.appID ==  114){ // is char r for reset
          restartDial();
          activeappid = 0;
          WAPacket.appID = 0;
        }
      }
    }
    return true;
  }
}

void sendSpaceNavRaw() {
  //delay(1); //delay for receiver to prepare to receive data
  long datasendtimeout = micros();
  bool spacenavdatasent = false;
  if (!radio.write(&spacenavRaw, sizeof(spacenavRaw))) {
    SerialUSB.println("failed");
  }
  else {
    while (radio.available()) {
      radio.read(&WAPacket, sizeof(WAPacket));
      if (WAPacket.appID < maxconfignum) {
        if (WAPacket.appID != prevactiveappid) { //new app id
          activeappid = WAPacket.appID;
          prevactiveappid = activeappid;
          updateknobresolution();
          resetledVariables();
        }
      }
    }
  }
}





void processDialInput(int commandtype, char commandchar, bool commandmethod, String commandstring, int commandrelease, bool is_inappcommand) {
  if (commandrelease != 1) { //input is an actual command not release type
    lastinputtime = millis();
  }

  if (connectState == wireless) { //if wireless mode is set
    cleardialpacket(); //reset dial packet
    if (commandtype == 1) { //is keystroke
      dialPacket.keystroke = commandchar;
    }
    else if (commandtype == 2) { //is mouse control
      dialPacket.keyormouse = true;
      dialPacket.mousecontrol = commandchar;
      if (commandchar == MOUSEMOVE_UP) {
        dialPacket.mouseY = -1 * mousemoveMul;
      }
      else if (commandchar == MOUSEMOVE_DOWN) {
        dialPacket.mouseY = 1 * mousemoveMul;
      }
      else if (commandchar == MOUSEMOVE_LEFT) {
        dialPacket.mouseX = -1 * mousemoveMul;
      }
      else if (commandchar == MOUSEMOVE_RIGHT) {
        dialPacket.mouseX = 1 * mousemoveMul;
      }
    }
    else if (commandtype == 3 || commandtype == 4) { //is raw data from dial
      if (commandtype == 3) { //is string
        dialPacket.is_string = true;
        if (sendDialPacket()) { //if prep was sent to adapter successfully
          breakandsendstring(commandstring);
          dialPacket.is_string = false;
        }
      }
      else {  //is spacenav data
        dialPacket.spacenavRawAvailable = true;
        if (sendDialPacket()) { //if prep was sent to adapter successfully
          sendSpaceNavRaw();
          dialPacket.spacenavRawAvailable = false;
        }

      }
    }
    if (is_inappcommand) { //is in app command
      dialPacket.inapp = true;
      commandstring.replace("app is in charge", "");
      dialPacket.inappcommandID = commandstring.toInt();
    }
    if (commandrelease != 0) { //should release after command
      dialPacket.ctrlrelease = true;
    }
    if (commandtype != 3 || commandtype != 4) { //if data has not already being sent as string or spacenav data
      //SerialUSB.println(dialPacket.keystroke);
      sendDialPacket(); //send dial packet
      cleardialpacket(); //reset dial packet
    }

  }
  else { //is in wiredmode
    if (commandtype == 1) { //is keystroke
      if (!commandmethod) { //use keyboard.press
        Keyboard.press(commandchar);
      }
      else {
        Keyboard.write(commandchar);
      }
    }
    else if (commandtype == 2) { //is mouse control
      if (commandchar == MOUSEMOVE_UP) {
        mouseControl(commandchar, 0, -1);
      }
      else if (commandchar == MOUSEMOVE_DOWN) {
        mouseControl(commandchar, 0, 1);
      }
      else if (commandchar == MOUSEMOVE_LEFT) {
        mouseControl(commandchar, -1, 0);
      }
      else if (commandchar == MOUSEMOVE_RIGHT) {
        mouseControl(commandchar, 1, 0);
      }
      else { //is mouse click
        mouseControl(commandchar, 0, 0);
      }
    }
    else if (commandtype == 3 || commandtype == 4) { //is raw data from dial
      if (commandtype == 3) { //is string
        int strbuflen = commandstring.length();
        char strbuf[strbuflen];
        commandstring.toCharArray(strbuf, strbuflen + 1);
        for (int i = 0; i < strbuflen; i++) {
          if (strbuf[i] == '#') {
            Keyboard.write('\n');
          }
          else {
            Keyboard.write(strbuf[i]);
          }
        }
        //Keyboard.print(commandtype);
      }
      else {  //is spacenav data
       //collect and send spacenav data via serial
        String gyro = String(spacenavRaw.SNG_x, 2) + "|" + String(spacenavRaw.SNG_y, 2) + "|" + String(spacenavRaw.SNG_rad, 2) + "|";
        int padding = 19 - gyro.length();
        for (int i = 0; i < padding; i++) {
          gyro = "<" + gyro;
        }
        String planar = String(spacenavRaw.SNP_x, 2) + "|" + String(spacenavRaw.SNP_y, 2) + "|" + String(spacenavRaw.SNP_rad, 2) + "|";
        padding = 19 - planar.length();
        for (int i = 0; i < padding; i++) {
          planar = ">" + planar;
        }
        SerialUSB.print(gyro + planar);
      }
    }
    if (is_inappcommand) { //is in app command
      SerialUSB.print(commandstring);
    }
    if (commandrelease == 1) {
      Keyboard.releaseAll();
      Mouse.release(MOUSE_LEFT);
      Mouse.release(MOUSE_RIGHT);
      Mouse.release(MOUSE_MIDDLE);
    }
  }

}

void cleardialpacket() {


  dialPacket.inapp = false;
  dialPacket.inappcommandID = 0;
  dialPacket.keyormouse = false; //0 = keyboard, 1 = mouse
  dialPacket.keystroke = '0';
  dialPacket.mousecontrol = '0';
  dialPacket.mouseX = 0;
  dialPacket.mouseY = 0;
  dialPacket.ctrlrelease = false;
dialPacket.spacenavRawAvailable = false;
  dialPacket.is_string = false;


}

void updateknobresolution() {
  ahmsville_dial.setknobresolution(appconfig[activeappid].knob1_res, 1);

}

void mouserelease() {
  if ((millis() - mousecontroltime) > mousereleaseinterval) {
    processDialInput(0, '0', false, "", 1, false); //release mouse
    mousecontroltime = millis();
  }
}
