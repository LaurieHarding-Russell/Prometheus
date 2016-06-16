/*
By Laurie Harding-Russell
Purpose: This class deals with controlling the tv and its periferals.
*/
#ifndef tvControl_h
#define tvControl_h
#include <string>

#include "lirc/lirc_client.h"
// CEC
#include <iostream> // CEC libraries use std::cout without initializing it... wtf.
#include <libcec/cec.h>
//#include <libcec/cecloader.h>
//#include "bcm_host.h"

class tvControl {
	public:
	tvControl();
	~tvControl();
	void tvOn();
	void hdmiUp();
	void hdmiDown();
	void tvOff();
	int onKeypress(void* not_used, const CEC::cec_keypress msg);

	private:
	std::string pin = "21";

	// Lirc stuff
	int fd;

	// CEC
	bool cec;
	//CEC::ICECCallbacks        cec_callbacks;
	CEC::libcec_configuration cecConfig;
	CEC::ICECAdapter* cecAdapter;

	// Command
	CEC::cec_logical_address device;

	char* port;
};

#endif
