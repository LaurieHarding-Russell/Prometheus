#include "internet.h"

Internet::Internet() {

}

Internet::~Internet() {

}

std::string Internet::search(std::string search) {
	int sock;
	search = "https://duckduckgo.com//?q=" + search;
	struct hostent *hp =gethostbyname(search.c_str());
	if (!hp){
		return "FAILED";
	}
	if (sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)!= -1) {
		memset(&server, 0, sizeof(server));
		server.sin_family = AF_INET;
		server.sin_port = htons(PORT);
		memcpy((void *)&server.sin_addr, hp->h_addr_list[0], hp->h_length);

		if (connect(sock,(struct sockaddr *) &server,sizeof(server)) != -1) {	// Try to connect
			char buffer[2048];
			int size;
			if(size=recv(sock, buffer, 2047,0)>=1) {
				std::cout <<buffer << std::endl;
			}
		}
	}
}
