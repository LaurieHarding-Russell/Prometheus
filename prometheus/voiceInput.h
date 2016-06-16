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

void clearInput(){
	voiceIn::lock.lock();
	while(!voiceIn::input.empty()) voiceIn::input.pop();
	voiceIn::lock.unlock();
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

static void grammerVInput(const char* file) {
	err_set_logfile("pocketSphinx_Log.txt");
	voiceIn::lock.lock();
	voiceIn::quit = false;
	voiceIn::lock.unlock();

        // Pocket sphinx setup
        ps_decoder_t *ps = NULL;
        cmd_ln_t *config = NULL;

        config = cmd_ln_init(NULL, ps_args(), TRUE,
                //"-hmm", MODELDIR "/en-us/en-us",
		"-hmm", MODELDIR "/cmusphinx-en-us-ptm-5.2",
                "-dict", "./dictionary/en.dict",
                "-jsgf", file,
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
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		voiceIn::lock.lock();
	}
	voiceIn::lock.unlock();
	ad_close(ad);
	ps_free(ps);
	cmd_ln_free_r(config);
}

static void generalInput() {
	err_set_logfile("pocketSphinx_LogGI.txt");
        voiceIn::lock.lock();
        voiceIn::quit = false;
        voiceIn::lock.unlock();


        // Pocket sphinx setup
        ps_decoder_t *ps = NULL;
        cmd_ln_t *config = NULL;

        config = cmd_ln_init(NULL, ps_args(), TRUE,
                //"-hmm", MODELDIR "/en-us/en-us",
                "-hmm", MODELDIR "/cmusphinx-en-us-ptm-5.2",
                "-lm", MODELDIR "/en-us/en-us.lm.bin",
                "-dict", "./dictionary/en.dict",
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

/*
Purpose: to record input from the speaker.
*/
static void saveInput() {
        err_set_logfile("pocketSphinx_LogSI.txt");
	voiceIn::lock.lock();
	voiceIn::quit = false;
	voiceIn::lock.unlock();

        // Pocket sphinx setup
        ps_decoder_t *ps = NULL;
        cmd_ln_t *config = NULL;

        config = cmd_ln_init(NULL, ps_args(), TRUE,
		"-hmm", MODELDIR "/cmusphinx-en-us-ptm-5.2",
                "-dict", "./dictionary/en.dict",
                "-jsgf", "./dictionary/saveIn.jsgf",
                NULL);

        ps = ps_init(config);

        ad_rec_t *ad;
        int16 adbuf[8192];
        uint8 utt_started, in_speech;
        int32 k;
        char const *hyp;
        bool failed = false;

	int state = 0;
	FILE * sFile = fopen("audio.txt", "wb");
	if (sFile != NULL) {
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
			adbuf[k] = '\0';
			fwrite(adbuf, 2, k, sFile);

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
						if (command == "search" && state == 0) {
							state = 1;
							voiceIn::input.push("search");
						} else if (command == "who" && state == 0) {
							state = 1;
							voiceIn::input.push("who");
						} else if (command == "please" && state == 1) {
							state = 2;
							voiceIn::quit = true;
						} else if (state == 1){
							fwrite(adbuf, 2, k, sFile);
						}
        	                        }
        	                        voiceIn::lock.unlock();
        	                }

				ps_start_utt(ps);
				utt_started = FALSE;
                	}
	                std::this_thread::sleep_for(std::chrono::milliseconds(100));
        	        voiceIn::lock.lock();
        	}

        	voiceIn::lock.unlock();
		fclose(sFile);
        	ad_close(ad);
        	ps_free(ps);
        	cmd_ln_free_r(config);
	}
}

void processVoiceData() {
	err_set_logfile("pocketSphinx_LogPF.txt");
        voiceIn::lock.lock();
        voiceIn::quit = false;
        voiceIn::lock.unlock();

        // Pocket sphinx setup
        ps_decoder_t *ps = NULL;
        cmd_ln_t *config = NULL;

	FILE * sFile = fopen("audio.txt", "rb");
	if (sFile != NULL) {
		config = cmd_ln_init(NULL, ps_args(), TRUE,
               		"-hmm", MODELDIR "/en-us/en-us",
                	"-lm", MODELDIR "/en-us/en-us.lm.bin",
                	"-dict", "./dictionary/en.dict",
                	NULL);

	        ps = ps_init(config);

	        int16 buf[8192];
	        uint8 utt_started, in_speech;
	        int32 k;
       		char const *hyp;
        	bool failed = false;

        	if (ps_start_utt(ps) < 0)
        	        failed =true;

	        while (!feof(sFile) && !failed) {
	                int size;
	 		size = fread(buf, 2, 8192, sFile);
	        	ps_process_raw(ps, buf, size, FALSE, FALSE);
       		}

		ps_end_utt(ps);
		hyp = ps_get_hyp(ps, &k);
		fclose(sFile);
		std::string command;
		if (hyp != NULL) {
	        	voiceIn::lock.lock();
       	        	std::istringstream iss(hyp);
                	while (std::getline(iss,command, ' ')) {
                		voiceIn::input.push(command);
                	}
        		voiceIn::lock.unlock();
		}
	        ps_free(ps);
        	cmd_ln_free_r(config);
	}
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
