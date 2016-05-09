#ifndef computer_cpp
#define computer_cpp
#include "computer.h"

Computer::Computer(){
	flite_init();
	voice = register_cmu_us_kal(NULL);
}

Computer::~Computer() {

}

void Computer::say(const char* text) {
	flite_text_to_speech(text, voice, "play");
}
#endif
