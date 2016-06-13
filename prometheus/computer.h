#ifndef Computer_h
#define Computer_h

extern"C" {
	#include "flite.h"
	cst_voice* register_cmu_us_slt(const char *voxdir);
	void unregister_cmu_us_slt(cst_voice *vox);
}
#include <alsa/asoundlib.h>

// Graphical output
#include <X11/Xlib.h>


class Computer {
	struct EState{
		int anger;
		int fear;
		int disgust;
		int happiness;
		int sadness;
		int surprise;
	};

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
	void setAlsaMasterVolume(long volume);

	cst_voice* voice;
	Display *display;
	int screen;
	Window window;
	EState emotState; 
};
#endif
