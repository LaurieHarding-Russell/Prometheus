/*
By Laurie Harding-Russell
Purpose: to use the pocket sphinx API to interpret speach and output it to a que
*/

#ifndef voiceInput_h
#define voiceInput_h

extern"C" {
	#include <sphinxbase/ad.h>
	#include <sphinxbase/err.h>
	#include <pocketsphinx.h>
}

#include <mutex>
#include <queue>

class VoiceInput {
	public:
	VoiceInput();
	static void getMicInput();

	bool quit;
	std::mutex lock;
	std::queue<std::string> input;

	private:
	ps_decoder_t *ps;
       cmd_ln_t *config;
	ad_rec_t *ad;
	bool failed;
};

#endif