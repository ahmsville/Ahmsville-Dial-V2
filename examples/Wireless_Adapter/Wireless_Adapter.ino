 #include <Keyboard.h>

#include <Mouse.h>

#include <SPI.h>
#include "RF24.h"

#define substrlen 12

#define MOUSEMOVE_UP 'U'
#define MOUSEMOVE_DOWN 'D'
#define MOUSEMOVE_LEFT 'L'
#define MOUSEMOVE_RIGHT 'R'
char KEY_SPACEBAR = 32;
char nullchar = 128;

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
  byte dialID = 0;
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
} WAPacket;






String receivedString = "";

/*********************************RX******************/


RF24 radio(0, 1);                          // Set up nRF24L01 radio on SPI bus plus(CE,CSN)


byte addresses[][6] = {"1Node", "2Node"};



int inByte;         // incoming Serial byte
String inString = "";
int activeappid;

long mousereleaseinterval = 200;
long mousecontroltime = 0;

long ledofftime = 0;
long stillavailable = 0;

void setup() {
  pinMode(9, OUTPUT);

  Serial.begin(115200);
  delay(500);
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.enableAckPayload();
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);

  radio.startListening();
  radio.writeAckPayload(1, &WAPacket, sizeof(WAPacket));
  radio.setChannel(63);
}

void loop() {


  receiveDataFromDial();
  //Serial.println(dialPacket.spacenavRawAvailable);
  /*

  */
  connecttoPC ();
  mouserelease();
  if((millis() - ledofftime) > 2){
     digitalWrite(9, LOW);
  }
 
}

void connecttoPC () {
  // reply only when you receive data:
  if (Serial.available() > 0) {
    inByte = Serial.read();
    inString += (char)inByte;

  }
  if (inString != "") {

    if (inString != "a") { //change app byte

      WAPacket.appID = inString.toInt();

      inString = "";

    }
    else if (inString == "a") {  //connection byte
      inString = "";
     digitalWrite(9, HIGH);
ledofftime = millis();
      Serial.print("***Wireless Adapter"); //reply with your dial type

    }
  }

  if ((millis() - stillavailable) > 5000) { //update wirelessly connected dial to pc every few secs
    if (dialPacket.dialID == 1) {
      Serial.print("********|||1|0|0|0|");
    }
    else if (dialPacket.dialID == 2) {
      Serial.print("********|||0|1|0|0|");
    }
    else if (dialPacket.dialID == 3) {
      Serial.print("********|||0|0|1|0|");
    }
    else if (dialPacket.dialID == 4) {
      Serial.print("********|||0|0|0|1|");
    }
    stillavailable = millis();

  }
}



void receiveDataFromDial() {
  //long receivetimeout = micros();
  if (dialPacket.dialID != 0) {
    if ( radio.available()) {
      digitalWrite(9, HIGH);
ledofftime = millis();
      if (dialPacket.is_string) { //recieve long string
        if (receiveLongString()) {
          receivedString.replace("  ", "");
          processWirelessDialInput();
          Serial.println(receivedString);
          receivedString = "";
          stringtoprint.endofstring = false;
          dialPacket.is_string = false;
        }
      } else if (dialPacket.spacenavRawAvailable) {
        while (radio.available()) {   //receive raw spacenav data
          digitalWrite(9, HIGH);
ledofftime = millis();
          radio.read(  &spacenavRaw, sizeof(spacenavRaw));
          radio.writeAckPayload(1, &WAPacket, sizeof(WAPacket) );
        }

        processWirelessDialInput();
        dialPacket.spacenavRawAvailable = false;
        spacenavRaw.SNG_x = 0;
        spacenavRaw.SNG_y = 0;
        spacenavRaw.SNG_rad = 0;
        spacenavRaw.SNP_x = 0;
        spacenavRaw.SNP_y = 0;
        spacenavRaw.SNP_rad = 0;
      } else {
        radio.read( &dialPacket, sizeof(dialPacket) );   //receive normal dial packet
        radio.writeAckPayload(1, &WAPacket, sizeof(WAPacket) );

        processWirelessDialInput();
        cleardialpacket();

      }
    }
  } else {

    if (radio.available()) {
      digitalWrite(9, HIGH);
ledofftime = millis();
      radio.read( &dialPacket, sizeof(dialPacket) );   //receive normal dial packet
      radio.writeAckPayload(1, &WAPacket, sizeof(WAPacket) );
    }
  }
}

bool receiveLongString() {
  receivedString = "";
  long receivetimeout = millis();
  while ((millis() - receivetimeout) < 500 && stringtoprint.endofstring == false) {
    if ( radio.available()) {
      while (radio.available()) {                                   // While there is data ready
        radio.read( &stringtoprint, sizeof(stringtoprint) );             // Get the payload
        radio.writeAckPayload(1, &WAPacket, 1 );
      }
      for (int i = 0; i < substrlen; i++) {
        receivedString = stringtoprint.stringdata[(substrlen - 1) - i] + receivedString; //append string
        // Serial.println(receivedString);
      }
      for (int i = 0; i < substrlen; i++) {
        //Serial.print(stringtoprint.stringdata[i]);
      }
      //Serial.println("");
    }
  }
  return stringtoprint.endofstring;
}


void processWirelessDialInput() {
  if (dialPacket.keystroke != '0' || dialPacket.mousecontrol != '0') { //is mouse or key input
    if (dialPacket.keyormouse) { //is mouse control
      mouseControl(dialPacket.mousecontrol, dialPacket.mouseX, dialPacket.mouseY);
    } else { //is keystroke
      Keyboard.press(dialPacket.keystroke);
    }
  } else {
    if (dialPacket.inapp) { //is inapp command
      String sss = "";
      if (dialPacket.inappcommandID < 10) {
        Serial.print(sss + "app is in charge00" + dialPacket.inappcommandID); //send inappcommandID to pc
      } else if (dialPacket.inappcommandID >= 10) {
        Serial.print(sss + "app is in charge0" + dialPacket.inappcommandID); //send inappcommandID to pc
      }

    }
    else if (dialPacket.is_string) { //is string
      //print string
      int strbuflen = receivedString.length();
      char strbuf[strbuflen];
      receivedString.toCharArray(strbuf, strbuflen + 1);
      for (int i = 0; i < strbuflen; i++) {
        if (strbuf[i] == '#') {
          Keyboard.write('\n');
        } else {
          Keyboard.write(strbuf[i]);
        }
      }
    }
    else if (dialPacket.spacenavRawAvailable) {
      //send spacenav data to app
      String gyro = String(spacenavRaw.SNG_x, 2) + "|" + String(spacenavRaw.SNG_y, 2) + "|" + String(spacenavRaw.SNG_rad, 2) + "|" ;
      int padding = 19 - gyro.length();
      for (int i = 0; i < padding; i++) {
        gyro = "<" + gyro;
      }
      String planar = String(spacenavRaw.SNP_x, 2) + "|" + String(spacenavRaw.SNP_y, 2) + "|" + String(spacenavRaw.SNP_rad, 2) + "|" ;
      padding = 19 - planar.length();
      for (int i = 0; i < padding; i++) {
        planar = ">" + planar;
      }
      Serial.print(gyro + planar);
      //Serial.print("\t");
      //delay(10);
      // Serial.print(planar);
    }
  }
  if (dialPacket.ctrlrelease) {
    Keyboard.releaseAll();
    Mouse.release(MOUSE_LEFT);
    Mouse.release(MOUSE_RIGHT);
    Mouse.release(MOUSE_MIDDLE);
  }
}

void cleardialpacket() {


  dialPacket.inapp = false;
  dialPacket.inappcommandID = 0;
  dialPacket.keyormouse = false; //0 = keyboard, 1 = mouse
  dialPacket.keystroke = '0';
  dialPacket.mousecontrol = '0';
  dialPacket.ctrlrelease = false;
}

void mouseControl(char command, int mouse_x, int mouse_y) {
  if (command == MOUSE_LEFT || command == MOUSE_RIGHT || command == MOUSE_MIDDLE) {
    Mouse.press(command);
  }
  else {
    Mouse.move(mouse_x, mouse_y, 0);
  }
}

void mouserelease() {
  if ((millis() - mousecontroltime) > mousereleaseinterval) {
    Keyboard.releaseAll();
    Mouse.release(MOUSE_LEFT);
    Mouse.release(MOUSE_RIGHT);
    Mouse.release(MOUSE_MIDDLE);
    mousecontroltime = millis();
  }
}