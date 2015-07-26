# DCP-LYD
**Code Name :: FloorFlowering**

**PLEASE SEE THE WIKI for the latest updates:** https://github.com/hevnsnt/DCP-LYD/wiki


Requirements:
* Install the Arduino SDK: http://arduino.cc/en/Main/Software
	- Edit preferences, add http://rfduino.com/package_rfduino_index.json to "Additional Board Manager URLs" and save.
	- Open the "Boards Manager" under Tools -> Board menu
	- Select the RFduino package at the end of the list, and click to install it.
	- Select the board and port
* FastLED Library: https://github.com/FastLED/FastLED/tree/FastLED3.1
* Hardware Schematics and Build Instructions are posted at: https://goo.gl/Tfyoja


Currently working code: 

	::FloorFlowering::
	* In order to change patterns:
		* Download application: RFduino Test
		* Scan for your lanyard
		* Connect (Lanyard should blink red on connect)
		* Send commands: 
			* 00 (Off)
			* 01 (rgbGlitter)
			* 02 (confetti)
			* 03 (sinelon)
			* 04 (juggle)
			* 05 (Fire)
			* 06 (bpm)
			* 07 (Cylon)
			* 08 (discostrobe)
			* 09 (rainbowWithGlitter)
			* 10 (DefconParties)
			* 11 (RRRGGGBBB) <-- In progress
			* 12 (rotatOR) <-- In progress



NEED WORK:

	* (done) Input bluetooth command (00, 01, 02, etc) and directly call the appropriate animation
	* (done) Make a function that turns off all LEDs 
	* (done) Make a function that turns ALL LEDS on a certain color
	* Make a mode that needs RGB input, light the whole strand. Get this input from color wheel


Source code is documentation for FastLED.  

	* http://fastled.io/docs/3.1/colorutils_8h_source.html
	* http://fastled.io/docs/3.1/lib8tion_8h_source.html
	* Color list: https://github.com/FastLED/FastLED/wiki/Pixel-reference#predefined-colors-list
