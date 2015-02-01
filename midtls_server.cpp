#include <stdlib.h>
#include <iostream>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <error.h>

#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/rand.h>


using namespace std;

void start_server (int port, const char *local_address);


int
main ()
{
  start_server (217, "localhost");
  return 0;
}

void start_server(int port, const char *local_address)
{
	int socketfd;
	SSL_CTX *ctx;
	SSL *ssl;
	BIO *bio;

	struct sockaddr_in server = {0}, client ={0};
	
	server.sin_family = AF_INET;
	server.port = htons(port);

	OpenSSL_add_all_algorithms();
	SSL_load_error_strings();
	ctx = SSL_CTX_new(DTLSv1_server_method());

	SSL_CTX_set_cipher_list(ctx, "ALL:NULL:eNULL:aNULL");
	SSL_CTX_set_session_cache_mode(ctx, SSL_SESS_CACHE_OFF);

	if(-1 == (fd = socket(AF_INET, SOCK_DGRAM, 0)))
	{
		perror(NULL);
		return;
	}

	return;
}
