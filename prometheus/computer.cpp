#ifndef computer_cpp
#define computer_cpp
#include "computer.h"

Computer::Computer() {
	//setAlsaMasterVolume(85);
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
/*
void Computer::setAlsaMasterVolume(long volume) {
    long min, max;
    snd_mixer_t *handle;
    snd_mixer_selem_id_t *sid;
    const char *card = "default";
    const char *selem_name = "Master";

    snd_mixer_open(&handle, 0);
    snd_mixer_attach(handle, card);
    snd_mixer_selem_register(handle, NULL, NULL);
    snd_mixer_load(handle);

    snd_mixer_selem_id_alloca(&sid);
    snd_mixer_selem_id_set_index(sid, 0);
    snd_mixer_selem_id_set_name(sid, selem_name);
    snd_mixer_elem_t* elem = snd_mixer_find_selem(handle, sid);

    snd_mixer_selem_get_playback_volume_range(elem, &min, &max);
    snd_mixer_selem_set_playback_volume_all(elem, volume * max / 100);

    snd_mixer_close(handle);
}*/

Computer::~Computer() {
	unregister_cmu_us_slt(voice);
        if (display != NULL)
                XCloseDisplay(display);

}
#endif
