
#include "MacroKey.h"

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



int Mkeypins[Mkeycount] = {0,1,5,10,12};

volatile long Mkey_risetime[Mkeycount];
volatile long Mkey_falltime[Mkeycount];
volatile long autokeyrelease[Mkeycount] = { 0,0,0,0,0 };





void Mkey1_stateChange() {
	if (digitalRead(Mkeypins[0]) == 1 && Mkey_risetime[0] == 0) { //set time of rise once
		Mkey_risetime[0] = millis();
		autokeyrelease[0] = Mkey_risetime[0];
	}
	else if (digitalRead(Mkeypins[0]) == 0 && Mkey_risetime[0] != 0) {  //set time of fall if time of rise as already been set
		Mkey_falltime[0] = millis();
	}
}

void Mkey2_stateChange() {
	if (digitalRead(Mkeypins[1]) == 1 && Mkey_risetime[1] == 0) { //set time of rise once
		Mkey_risetime[1] = millis();
		autokeyrelease[1] = Mkey_risetime[1];
	}
	else if (digitalRead(Mkeypins[1]) == 0 && Mkey_risetime[1] != 0) {  //set time of fall if time of rise as already been set
		Mkey_falltime[1] = millis();
	}
}

void Mkey3_stateChange() {
	if (digitalRead(Mkeypins[2]) == 1 && Mkey_risetime[2] == 0) { //set time of rise once
		Mkey_risetime[2] = millis();
		autokeyrelease[2] = Mkey_risetime[2];
	}
	else if (digitalRead(Mkeypins[2]) == 0 && Mkey_risetime[2] != 0) {  //set time of fall if time of rise as already been set
		Mkey_falltime[2] = millis();
	}
}

void Mkey4_stateChange() {
	if (digitalRead(Mkeypins[3]) == 1 && Mkey_risetime[3] == 0) { //set time of rise once
		Mkey_risetime[3] = millis();
		autokeyrelease[3] = Mkey_risetime[3];
	}
	else if (digitalRead(Mkeypins[3]) == 0 && Mkey_risetime[3] != 0) {  //set time of fall if time of rise as already been set
		Mkey_falltime[3] = millis();
	}
}

void Mkey5_stateChange() {
	if (digitalRead(Mkeypins[4]) == 1 && Mkey_risetime[4] == 0) { //set time of rise once
		Mkey_risetime[4] = millis();
		autokeyrelease[4] = Mkey_risetime[4];
	}
	else if (digitalRead(Mkeypins[4]) == 0 && Mkey_risetime[4] != 0) {  //set time of fall if time of rise as already been set
		Mkey_falltime[4] = millis();
	}
}

void (*MkeyISRfunctions[])() = { Mkey1_stateChange, Mkey2_stateChange ,Mkey3_stateChange ,Mkey4_stateChange ,Mkey5_stateChange };


void setinterrupt() {
	for (int i = 0; i < Mkeycount; i++) {
		pinMode(Mkeypins[i], INPUT);
		if (i != 4) { //set int for mk 1-4
			attachInterrupt(digitalPinToInterrupt(Mkeypins[i]), (*MkeyISRfunctions[i]), CHANGE);
		}
		
	}
	
}


MacroKey::MacroKey() {

}


void MacroKey::initialize_macroKey() {
	setinterrupt();
}



int *MacroKey::get_keyPress() {
	Key5detection();
	for (int i = 0; i < Mkeycount; i++) {
		if (Mkey_risetime[i] != 0 && Mkey_falltime[i] != 0) {
			Mkey_timediff[i] = Mkey_falltime[i] - Mkey_risetime[i];
			if (Mkey_timediff[i] > keydebounce) {
				if (Mkey_timediff[i] < keytapthresh) {
					keyState[i] = tap;
					reset_macroKeytime(i);
				}
				else {
					keyState[i] = hold;
					reset_macroKeytime(i);
				}
			}
		}
		if ((millis() - autokeyrelease[i]) >= autoreleasethresh) {
			Mkey_falltime[i] = millis();
		}
	}
	for (int i = 0; i < Mkeycount; i++) {
		tempkeystates[i] = keyState[i];
		keyState[i] = nullIn;
	}
	
	return tempkeystates;;
}

void MacroKey::reset_macroKeytime(int mkey) {
	Mkey_risetime[mkey] = 0;
	Mkey_falltime[mkey] = 0;
}

void MacroKey::reset_macroKey() {
	for (int i = 0; i < Mkeycount; i++) {
		keyState[i] = nullIn;
	}
	
}


void MacroKey::Key5detection() {
	if (digitalRead(12) == 1) {  //read first high
		if (startedkey5detection == 0) {
			Mkey_risetime[4] = millis(); //set risetime and continue
			autokeyrelease[4] = Mkey_risetime[4];
			startedkey5detection = 1;
			//SerialUSB.println(startedkey5detection);
		}
		else if (startedkey5detection == 1) {
			if ((millis() - Mkey_risetime[4]) > keydebounce) { //debounce
				startedkey5detection = 2;
			//	SerialUSB.println(startedkey5detection);
			}
		}
		else if (startedkey5detection == 2) { //held too long
			if ((millis() - Mkey_risetime[4]) > autoreleasethresh) {
				Mkey_falltime[4] = millis();
				startedkey5detection = 3;
				//SerialUSB.println(startedkey5detection);
			}
		}

	}
	else { //wait for release
		if (startedkey5detection == 2) {
			Mkey_falltime[4] = millis();
			startedkey5detection = 0;
		//	SerialUSB.println(startedkey5detection);
		}
		else {
			startedkey5detection = 0;
		}
	}
	
}

