/*
By Laurie Harding-Russell
Purpose: To interface with some websites via the sound terminal...
Yes, the actions that can be taken will be very, very, very limited.
What have I gotten myself into?

*/

#ifndef internet_h
#define internet_h

#include <stdio.h>

// OpenSSL stuff
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

//#include <string.h>
#include <string>
#include <iostream>

class Internet {
	public:
	Internet();
	~Internet();
	std::string search(char* search);
};

#endif

