/*
By Laurie Harding-Russell
Purpose: This class deals with controlling the tv and its periferals.
*/
#ifndef tvControl_h
#define tvControl_h
#include <string>

#include "lirc/lirc_client.h"

class tvControl {
	public:
	tvControl();
	~tvControl();
	void tvOn();
	void tvOff();

	private:
	std::string pin = "21";
	bool cec;

	// Lirc stuff
	int fd;
};

#endif
