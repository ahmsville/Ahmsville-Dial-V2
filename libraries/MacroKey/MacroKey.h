

#ifndef MacroKey_h
#define MacroKey_h

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


#define Mkeycount 5 // number of macrokeys

#define tap 1
#define hold 2
#define nullIn 0


class MacroKey {
private:

	long Mkey_timediff[Mkeycount];
	long autoreleasethresh = 1000; //autorelease threshold in millis
	long keydebounce = 10; //key debounce range in millis
	long keytapthresh = 500; //tap threshold in millis
	int keyState[Mkeycount] = { 0,0,0,0,0 };
	int tempkeystates[Mkeycount] = {0,0,0,0,0};
	int startedkey5detection = 0;
	
public:

	MacroKey();
	void initialize_macroKey();
	
	int *get_keyPress();
	void reset_macroKeytime(int mkey);
	
	void reset_macroKey();
	
	void Key5detection();

};



#endif
