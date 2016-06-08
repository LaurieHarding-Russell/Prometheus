#ifndef computer_cpp
#define computer_cpp
#include "computer.h"

Computer::Computer() {
	flite_init();
	//voice = register_cmu_us_kal(NULL);
	voice = register_cmu_us_slt(NULL);

	// Graphics
	display = XOpenDisplay(NULL);
	if (display != NULL) {
		screen = DefaultScreen(display);
		window = XCreateSimpleWindow(display, RootWindow(display, screen), 0, 0, 720, 480, 0,
                           BlackPixel(display, screen), WhitePixel(display, screen));
		XMapWindow(display, window);
	}
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
Computer::~Computer(){
	// unregister_cmu_us_kal(voice);
	unregister_cmu_us_slt(voice);
        if (display != NULL)
                XCloseDisplay(display);

}
#endif
