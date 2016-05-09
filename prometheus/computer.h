#ifndef Computer_h
#define Computer_h

extern"C" {
	#include "flite.h"
	cst_voice* register_cmu_us_kal(const char *voxdir);
}

class Computer {
        public:
        Computer();
        ~Computer();
        void say(const char* text);

        private:
	cst_voice* voice;

};
#endif
