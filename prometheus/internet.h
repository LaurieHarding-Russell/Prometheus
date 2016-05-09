/*
By Laurie Harding-Russell
Purpose: To interface with some websites via the sound terminal...
Yes, the actions that can be taken will be very, very, very limited.
What have I gotten myself into?

*/

#ifndef internet_h
#define internet_h

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>

#include <string.h>
#include <iostream>

#define PORT 80

class Internet {
	public:
	Internet();
	~Internet();
	std::string search(std::string search);

	private:
	struct sockaddr_in server;
};

#endif
