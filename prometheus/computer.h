#ifndef Computer_h
#define Computer_h

extern"C" {
	#include "flite.h"
	cst_voice* register_cmu_us_kal(const char *voxdir);
}

// Graphical output
#include <X11/Xlib.h>

class Computer {
        public:
        Computer();
        ~Computer();

	/*
	Purpose: to output the given text as speech
	*/
        void say(const char* text);
	/*
	Purpose: to display text
	*/
	void displayText(const char* text);

        private:
	cst_voice* voice;
	Display *display;
	int screen;
	Window window;
};
#endif
