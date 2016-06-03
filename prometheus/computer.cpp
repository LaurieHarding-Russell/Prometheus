#ifndef computer_cpp
#define computer_cpp
#include "computer.h"

Computer::Computer(){
	flite_init();
	voice = register_cmu_us_kal(NULL);

	// Graphics
	display = XOpenDisplay(NULL);
	if (display != NULL) {
		screen = DefaultScreen(display);
		window = XCreateSimpleWindow(display, RootWindow(display, screen), 0, 0, 720, 480, 0,
                           BlackPixel(display, screen), WhitePixel(display, screen));
		XMapWindow(display, window);
	}
}

Computer::~Computer() {
	if (display != NULL)
		XCloseDisplay(display);
}

void Computer::say(const char* text) {
	flite_text_to_speech(text, voice, "play");
}

// graphics
void Computer::displayText(const char* text){
	if (display != NULL){
		XDrawString(display, window, DefaultGC(display, screen), 0, 360, text, strlen(text));
	}
}
#endif
