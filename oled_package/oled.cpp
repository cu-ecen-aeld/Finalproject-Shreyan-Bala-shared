#include "ssd1306.h"



int main( int argc, char * argv [] ) {

	SSD1306 myDisplay;
	myDisplay.initDisplay();

	//myDisplay.setAltI2C();
	myDisplay.initDisplay();
	myDisplay.clearDisplay();
	myDisplay.setWordWrap(TRUE);
	myDisplay.setDisplayMode(SSD1306::Mode::SCROLL);
	myDisplay.textDisplay("THIS IS THE FIRST LINE.");
	myDisplay.textDisplay("LONG WRAP THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG LONG WRAP THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG LONG WRAP THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG");
	myDisplay.textDisplay("PUNCTUATION !\"#$%&'()*+,-./:;<=>?@[\\]^_`");
	myDisplay.textDisplay("NUMBERS 0123456789");
	myDisplay.setWordWrap(FALSE);
	myDisplay.textDisplay("LONG NO WRAP THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG");
	myDisplay.textDisplay("THIS IS THE LAST LINE.");
	myDisplay.textDisplay("DISPLAY WRAP.");

/*
	SSD1306 myDisplay;
	myDisplay.setDisplayMode(SSD1306::Mode::SCROLL);
	myDisplay.setWordWrap(TRUE);
	myDisplay.textDisplay("THIS IS THE FIRST LINE.");
	myDisplay.textDisplay("LONG WRAP THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG LONG WRAP THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG LONG WRAP THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG");
	myDisplay.textDisplay("PUNCTUATION !\"#$%&'()*+,-./:;<=>?@[\\]^_`");
	myDisplay.textDisplay("NUMBERS 0123456789");
	myDisplay.setWordWrap(FALSE);
	myDisplay.textDisplay("LONG NO WRAP THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG");
	myDisplay.textDisplay("THIS IS THE LAST LINE.");
	myDisplay.textDisplay("DISPLAY WRAP.");

	//myDisplay.setDisplayMode(SSD1306::Mode::SCROLL);
	for(int i=0; i<20; i++)
	{
		char buffer [10];
		sprintf (buffer, "LINE %c", i + 64);
		myDisplay.textDisplay(buffer);
	}
	*/
}
