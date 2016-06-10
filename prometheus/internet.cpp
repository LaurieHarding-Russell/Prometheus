#include "internet.h"

Internet::Internet() {
	ERR_load_BIO_strings();
	SSL_load_error_strings();
	OpenSSL_add_all_algorithms();
	SSL_library_init();
}

Internet::~Internet() {

}

void Internet::search(std::string search) {
        BIO* bio;       // Input socket

        SSL* ssl;       // cipher
        SSL_CTX* ctx;   //cipher method
	std::string result;
	ctx = SSL_CTX_new(SSLv23_client_method());	// Setting cipher method

	if (ctx == NULL) {
		SSL_load_error_strings();
		ERR_print_errors_fp(stderr);
		//return "Failed to set cipher\n";
		results[0].desc = "Failed to set cipher";
		current = 0;
		max = 1;
	} else if(!SSL_CTX_set_default_verify_paths(ctx)) {
		//return "Failed to load SSL certs \n";
		results[0].desc = "Failed to load SSL certs";
		current = 0;
		max = 1;
	} else {
		bio = BIO_new_ssl_connect(ctx);
		BIO_get_ssl(bio, & ssl);
		SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

		// Setup connect.
		BIO_set_conn_hostname(bio,"www.duckduckgo.com:https");

		if (bio == NULL){
			results[0].desc = "Failed!?";
			current = 0;
			max = 1;
			//return "wtf Failed. \n";
		} else {
			// Verify connection + handshake
			if (BIO_do_connect(bio) <= 0) {
				SSL_load_error_strings();
				ERR_print_errors_fp(stderr);
				//return "failed to connect... \n";
                		results[0].desc = "Failed to connect...";
                		current = 0;
                		max = 1;
			} else {

				// Verify that SSL!
				//int error;
				int error = SSL_get_verify_result(ssl);
				if(error != X509_V_OK){
					SSL_load_error_strings();
					ERR_print_errors_fp(stderr);
					//std::cout << error << "\n";
					results[0].desc = "Cert failed";
					current = 0;
	                                max = 1;
				} else {
					// Send request
					char request[255];
					strcpy (request, "GET https://duckduckgo.com/html/?q=");
					strcat (request, search.c_str());
					strcat (request, "\r\n\r\n");
					//std::cout << request <<'\n';

					BIO_write(bio, request, strlen(request));

					if (error == -1) {
						SSL_load_error_strings();
						ERR_print_errors_fp(stderr);
						results[0].desc = "Failed to write";
						current = 0;
		                                max = 1;
					} else {
						// Get request!
						char buf[2048];
						do {
							error = BIO_read(bio, buf, 2047);
							if (error > 0){
								buf[error] = 0;
								result += std::string(buf);
							}
						} while (error > 0);
					}
				}

				SSL_CTX_free(ctx);
				BIO_free_all(bio);
				int pos = result.find("class=\"result results_links results_links_deep web-result \"");
				if (pos > 0) {
					result = result.substr(pos);

					// Check for meaning
					current = 0;
					max = 0;
					do {
						pos = result.find("class=\"result__snippet\"");
						if (pos > 0) {
							result = result.substr(pos);
							pos = result.find(">");
							result = result.substr(pos+1);
							pos = result.find("</a>");
							if (pos > 0 && max != 11) {
								results[max].desc = result.substr(0, pos);
								results[max].desc = cleanStringOfTags(results[max].desc);
								max++;
							}
						}
					} while (pos > 0);
					if (max = 0){
						results[0].desc = "No results";
						max = 1;
					}
					// Check for about
				} else {
					result = "Internet problems?";
				}
			}
		}
	}
}

std::string Internet::getCurrentResult() {
	return results[current].desc;
}
std::string Internet::getNextResult(){
	if (current + 1 != max) {
		current++;
	} else {
		current = 0;
	}
	return results[current].desc;
}
std::string Internet::getPreviousResult(){
	if (current - 1 != 0) {
                current++;
        } else {
                current = max - 1;
        }
        return results[current].desc;
}



std::string Internet::cleanStringOfTags(std::string text) {
	int pos1,pos2;
	do {
		pos1 = text.find("<");
		pos2 = text.find(">");
		if (pos1 > 0 && pos2 > pos1) {
			text = text.substr(0,pos1) + text.substr(pos2+1);
		}
	} while (pos1 > 0 || pos2 > pos1);
	return text;
}
