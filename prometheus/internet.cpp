#include "internet.h"

Internet::Internet() {
	ERR_load_BIO_strings();
	SSL_load_error_strings();
	OpenSSL_add_all_algorithms();
	SSL_library_init();
}

Internet::~Internet() {

}

std::string Internet::search(std::string search) {
	BIO* bio;
	std::string result;

	SSL* ssl;
	SSL_CTX* ctx;
	ctx = SSL_CTX_new(SSLv23_client_method());	// Setting cipher method

	if (ctx == NULL) {
		SSL_load_error_strings();
		ERR_print_errors_fp(stderr);
		return "Failed to set cipher\n";
	} else if(!SSL_CTX_set_default_verify_paths(ctx)) {
		return "Failed to load SSL certs \n";
	}
	bio = BIO_new_ssl_connect(ctx);
	BIO_get_ssl(bio, & ssl);
	SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

	// Setup connect.
	BIO_set_conn_hostname(bio,"www.duckduckgo.com:https");

	if (bio == NULL)
		return "wtf Failed. \n";
	// Verify connection + handshake
	if (BIO_do_connect(bio) <= 0) {
		SSL_load_error_strings();
		ERR_print_errors_fp(stderr);
		return "failed to connect... \n";
	}

	// Verify that SSL!
	//int error;
	int error = SSL_get_verify_result(ssl);
	if(error != X509_V_OK){
		SSL_load_error_strings();
		ERR_print_errors_fp(stderr);
		std::cout << error << "\n";
		result = "Cert failed \n";
	} else {
		// Send request
		char request[255];
		strcpy (request, "GET https://duckduckgo.com/?q=");
		strcat (request, search.c_str());
		strcat (request, "\r\n\r\n");
		std::cout << request <<'\n';

		BIO_write(bio, request, strlen(request));

		if (error == -1) {
			SSL_load_error_strings();
			ERR_print_errors_fp(stderr);
			result = "Failed to write\n";
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
	if (result.length()>25) {
		int pos = result.find("<div id=\"zero_click_wrapper\"");
		if (pos > 0){
			result = result.substr(pos);
			// Check for meaning
			int meaning = result.find("\"zci-meanings\"");
			int about = result.find("\"zci-about\"");
			if(meaning> 0) {
				// Has meaning ia
				result = result.substr(meaning);
				
			} else if ( meaning > 0) {
				// Has about ia
				result = result.substr(about);
			} else {
				result = "unsupported instant answer.";
			}
			// Check for about
		} else {
			result = "Sorry, Duck Duck go failed to provide an instant answer. Maybe try rephrasing the query?";
		}
	}
	return result;
}
