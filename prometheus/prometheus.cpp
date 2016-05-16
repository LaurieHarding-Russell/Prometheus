#include <iostream>
#include <sstream>

#include <sphinxbase/ad.h>
#include <sphinxbase/err.h>
#include <pocketsphinx.h>

#include "tvControl.h"
#include "computer.h"
#include "internet.h"
#include <thread>
#include <chrono>
#include <mutex>
#include <queue>

static void getMicInput();
std::queue<std::string> input;

namespace m{
	bool quit = false;
	std::mutex lock;
}
int main() {
	tvControl tv;
	Computer computer;
	Internet* internet = new Internet();
	computer.say("Morning. Setting up my brain.");

	// setup pocket sphinx and listen for input
	std::thread inputThread = std::thread(&getMicInput);
	std::string command;
	m::lock.lock();
	while (!m::quit) {
		if (!input.empty()) {
			command = input.front();
			input.pop();
			if (command == "computer") {
				int num = 0;
				while (input.empty() && num <20) {
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
					num++;
				}
				if (input.empty())
					continue;

	                        command = input.front();
       		                input.pop();
				if (command == "search") {
					command = "";
					while(!input.empty()) {
		        	                command += input.front();
	        	        	        input.pop();
					}
					internet->search(command.c_str());
				} else if (command == "tv"){
					if (input.empty())
						continue;
	                	        command = input.front();
       		        	        input.pop();
				} else if (command == "say") {
					std::string say = "";
					while(!input.empty() && command != "computer") {
						say = say + input.front();
						input.pop();
					}
					computer.say(say.c_str());
				} else {
					std::cout << command + "\n";
				}
			} else {
				std::cout << command + "\n";
			}
		}

		// let other threads do work.
		m::lock.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		m::lock.lock();
	}
	inputThread.join();
	return 0;
}

/* Sound thread */
static void getMicInput() {
	err_set_logfile("pocketSphinx_Log.txt");

	// Pocket sphinx setup
        ps_decoder_t *ps = NULL;
        cmd_ln_t *config = NULL;

        config = cmd_ln_init(NULL, ps_args(), TRUE,
                "-hmm", MODELDIR "/en-us/en-us",
                "-lm", MODELDIR "/en-us/en-us.lm.bin",
                "-dict", MODELDIR "/en-us/cmudict-en-us.dict",
                NULL);

        ps = ps_init(config);

	ad_rec_t *ad;
	int16 adbuf[4096];
	uint8 utt_started, in_speech;
	int32 k;
	char const *hyp;
	bool failed;

 	if ((ad = ad_open_dev(cmd_ln_str_r(config, "-adcdev"),
            			(int) cmd_ln_float32_r(config,
       				"-samprate"))) == NULL)
 		failed = true;
	if (ad_start_rec(ad) < 0)
		failed = true;

	if (ps_start_utt(ps) < 0)
		printf("utter problems");

	std::string command = "";
	m::lock.lock();
	while (!quit) {
		m::lock.unlock();
		k = ad_read(ad, adbuf, 4096);
		ps_process_raw(ps, adbuf, k, false, false);
		in_speech = ps_get_in_speech(ps);
		if (in_speech && !utt_started) {
          		utt_started = TRUE;
       		}
        	if (!in_speech && utt_started) {
            		// speech -> silence transition, time to start new utterance 
            		ps_end_utt(ps);
            		hyp = ps_get_hyp(ps, NULL );
            		if (hyp != NULL) {
				m::lock.lock();
				std::istringstream iss(hyp);
				while (std::getline(iss,command, ' ')) {
					input.push(command);
				}
				m::lock.unlock();
       			}

       	    		if (ps_start_utt(ps) < 0){

               		}
       			utt_started = FALSE;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		m::lock.lock();
	}
	m::lock.unlock();
	std::cout << failed;
	ad_close(ad);
        ps_free(ps);
        cmd_ln_free_r(config);
}
