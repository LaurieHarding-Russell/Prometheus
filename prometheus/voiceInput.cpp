#include "voiceInput.h"

VoiceInput::VoiceInput() {
	err_set_logfile("pocketSphinx_Log.txt");
	
	ps_decoder_t *ps = NULL;
       cmd_ln_t *config = NULL;
	// Pocket sphinx setup

	config = cmd_ln_init(NULL, ps_args(), TRUE,
                "-hmm", MODELDIR "/en-us/en-us",
                "-lm", MODELDIR "/en-us/en-us.lm.bin",
                "-dict", MODELDIR "/en-us/cmudict-en-us.dict",
                NULL);

        ps = ps_init(config);

	if ((ad = ad_open_dev(cmd_ln_str_r(config, "-adcdev"),
            			(int) cmd_ln_float32_r(config,
       				"-samprate"))) == NULL)
 		failed = true;
	if (ad_start_rec(ad) < 0)
		failed = true;

	if (ps_start_utt(ps) < 0)
		failed =true;

}

void VoiceInput::getMicInput() {
	quit = false;
	int16 adbuf[4096];
	uint8 utt_started, in_speech;
	int32 k;
	char const *hyp;
	bool failed = false;

	std::string command = "";
	lock.lock();
	while (!quit && !failed) {
		lock.unlock();
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
				lock.lock();
				std::istringstream iss(hyp);
				while (std::getline(iss,command, ' ')) {
					input.push(command);
				}
				lock.unlock();
       			}

       	    		if (ps_start_utt(ps) < 0){

               		}
       			utt_started = FALSE;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		lock.lock();
	}
	lock.unlock();
	ad_close(ad);
	ps_free(ps);
        cmd_ln_free_r(config);

}