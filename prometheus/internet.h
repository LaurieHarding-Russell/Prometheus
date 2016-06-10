/*
By Laurie Harding-Russell
Purpose: To interface with some websites. 

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

#define MAXRESULTS 10
struct Result {
	std::string desc;
	std::string website;
};

class Internet {
	public:
	Internet();
	~Internet();
	void search(std::string search);
	std::string getCurrentResult();
	std::string getNextResult();
	std::string getPreviousResult();

	private:
	std::string cleanStringOfTags(std::string text);
	int current;
	int max;
	Result results[MAXRESULTS];

};

#endif

