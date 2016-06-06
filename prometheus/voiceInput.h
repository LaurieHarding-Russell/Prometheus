/*
VoiceInput Library
Purpose: A set of functions to help use/thread pocket sphinx.
By Laurie Harding-Russell
*/
#ifndef voiceInput_h
#define voiceInput_h

#include <sphinxbase/ad.h>
#include <sphinxbase/err.h>

#include <pocketsphinx.h>

#include <sstream>
#include <mutex>
#include <queue>
#include <thread>
#include <chrono>

#include <iostream>

namespace voiceIn {
	std::queue<std::string> input;
        bool quit = false;
        std::mutex lock;
}

bool PSRunning(){
	voiceIn::lock.lock();
	bool s = !voiceIn::quit;
	voiceIn::lock.unlock();
	return s;
}

std::string getInput(){
	voiceIn::lock.lock();
        if (voiceIn::input.empty()){
		voiceIn::lock.unlock();
                return "";
        } else {
                std::string data = voiceIn::input.front();
                voiceIn::input.pop();
		voiceIn::lock.unlock();
                return data;
        }
}

static void mainMenuInput() {
	err_set_logfile("pocketSphinx_Log.txt");
	voiceIn::quit = false;

        // Pocket sphinx setup
        ps_decoder_t *ps = NULL;
        cmd_ln_t *config = NULL;

        config = cmd_ln_init(NULL, ps_args(), TRUE,
                "-hmm", MODELDIR "/en-us/en-us",
                //"-lm", MODELDIR "/en-us/en-us.lm.bin",
                "-dict", "./dictionary/en.dict",
                "-jsgf", "./dictionary/prometheus.jsgf",
                NULL);

        ps = ps_init(config);

        ad_rec_t *ad;
        int16 adbuf[8192];
        uint8 utt_started, in_speech;
        int32 k;
        char const *hyp;
        bool failed = false;

        if ((ad = ad_open_dev(cmd_ln_str_r(config, "-adcdev"),
                                (int) cmd_ln_float32_r(config,
                                "-samprate"))) == NULL)
                failed = true;
        if (ad_start_rec(ad) < 0)
                failed = true;

        if (ps_start_utt(ps) < 0)
                failed =true;

	std::string command = "";
	voiceIn::lock.lock();
	while (!voiceIn::quit && !failed) {
		voiceIn::lock.unlock();
		k = ad_read(ad, adbuf, 8191);


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
				voiceIn::lock.lock();
				std::istringstream iss(hyp);
				while (std::getline(iss,command, ' ')) {
					voiceIn::input.push(command);
				}
				voiceIn::lock.unlock();
			}

			if (ps_start_utt(ps) < 0){

			}
			utt_started = FALSE;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(750));
		voiceIn::lock.lock();
	}
	voiceIn::lock.unlock();
	ad_close(ad);
	ps_free(ps);
	cmd_ln_free_r(config);
}

static void generalInput() {
	err_set_logfile("pocketSphinx_LogGI.txt");
	voiceIn::quit = false;

        // Pocket sphinx setup
        ps_decoder_t *ps = NULL;
        cmd_ln_t *config = NULL;

	ad_rec_t *ad;	// <- microphone recorder.


	/*
	We are using a phonetically tied language model.
	*/
	config = cmd_ln_init(NULL, ps_args(), TRUE,
		"-hmm", MODELDIR "/en-us/en-us",
		"-featparams", MODELDIR "/cmusphinx-en-us-ptm-5.2/feat.params",
		"-mdef", MODELDIR "/cmusphinx-en-us-ptm-5.2/mdef",
		"-mean", MODELDIR "/cmusphinx-en-us-ptm-5.2/means",
		"-var", MODELDIR "/cmusphinx-en-us-ptm-5.2/variances",
		"-tmat", MODELDIR "/cmusphinx-en-us-ptm-5.2/transition_matrices",
		"-sendump", MODELDIR "/cmusphinx-en-us-ptm-5.2/sendump",
		"-dict", "./dictionary/en.dict",
	     	"-compallsen", "yes",
	     	"-input_endian", "little",
		NULL);

        ps = ps_init(config);
	if (ps == NULL) {
		std::cout << "Problem?\n";
	}

	int16 adbuf[8192];
	uint8 utt_started, in_speech;
	int32 k;
	char const *hyp;
	bool failed = false;
	if ((ad = ad_open_dev(cmd_ln_str_r(config, "-adcdev"),
                                (int) cmd_ln_float32_r(config,
                                "-samprate"))) == NULL)
                failed = true;
        if (ad_start_rec(ad) < 0)
                failed = true;

        if (ps_start_utt(ps) < 0)
                failed =true;

        std::string command = "";
        voiceIn::lock.lock();
	std::cout << failed << '\n';
        while (!voiceIn::quit && !failed) {
                voiceIn::lock.unlock();
                k = ad_read(ad, adbuf, 8191);


                ps_process_raw(ps, adbuf, k, false, false);
                in_speech = ps_get_in_speech(ps);
                if (in_speech && !utt_started) {
                        utt_started = TRUE;
                }
                if (!in_speech && utt_started) {
                        // speech -> silence transition, time to start new utte$
                        ps_end_utt(ps);
			hyp = ps_get_hyp(ps, NULL );
                        if (hyp != NULL) {
                                voiceIn::lock.lock();
                                std::istringstream iss(hyp);
                                while (std::getline(iss,command, ' ')) {
                                        voiceIn::input.push(command);
                                }
                                voiceIn::lock.unlock();
                        }

                        if (ps_start_utt(ps) < 0){

                        }
                        utt_started = FALSE;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(750));
                voiceIn::lock.lock();
        }
        voiceIn::lock.unlock();
        ad_close(ad);
        ps_free(ps);
        cmd_ln_free_r(config);
}



void stopInput() {
	voiceIn::lock.lock();
	voiceIn::quit = true;
	voiceIn::lock.unlock();
}

bool waitForSpeaker(){
	int size;
	int max = 5;
	do {
		size = voiceIn::input.size();
		voiceIn::lock.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(1500));
		voiceIn::lock.lock();
		max--;
       	} while (size < voiceIn::input.size() && max != 0);
	return max == 0;
}
#endif
