#include <stdlib.h>
#include <iostream>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <error.h>
#include <poll.h>

#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/rand.h>

const int on = 1;


using namespace std;

void start_server (int port, const char *local_address);
void server_echo(struct sockaddr_in * client, SSL* ssl);

int
main ()
{
  start_server (23232, "127.0.0.1");
  return 0;
}

void
start_server (int port, const char *local_address)
{
  int sockfd;
  SSL_CTX *ctx;
  SSL *ssl;
  BIO *bio;
  struct pollfd pfd={0};

  struct sockaddr_in server = { 0 }, client =
  {
  0};

  server.sin_family = AF_INET;
  server.sin_port = htons (port);
  server.sin_addr.s_addr = INADDR_ANY;

  OpenSSL_add_ssl_algorithms ();
  SSL_load_error_strings ();
  ctx = SSL_CTX_new (DTLSv1_server_method ());

  SSL_CTX_set_cipher_list (ctx, "ALL:NULL:eNULL:aNULL");
  SSL_CTX_set_session_cache_mode (ctx, SSL_SESS_CACHE_OFF);

  if (!SSL_CTX_use_certificate_file
      (ctx, "certs/cacert.pem", SSL_FILETYPE_PEM))
    printf ("ERROR: no certificate found!\n");

  if (!SSL_CTX_use_PrivateKey_file
      (ctx, "certs/privkey.pem", SSL_FILETYPE_PEM))
    printf ("\nERROR: no private key found!");

  if (!SSL_CTX_check_private_key (ctx))
    printf ("\nERROR: invalid private key!\n");

  SSL_CTX_set_verify (ctx, SSL_VERIFY_NONE, NULL);

  if (-1 == (sockfd = socket (AF_INET, SOCK_DGRAM, 0)))
    {
      perror (NULL);
      return;
    }

  if (-1 == setsockopt (sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof (on)))
    {
      perror (NULL);
      return;
    }

  if (-1 == bind (sockfd, (struct sockaddr *) &server, sizeof (server)))
    {
      perror (NULL);
      return;
    }

  pfd.fd = sockfd;

  bio = BIO_new_dgram (sockfd, BIO_NOCLOSE);
  ssl = SSL_new (ctx);
  SSL_set_bio (ssl, bio, bio);

  while (1)
    {
      memset (&client, 0, sizeof (client));
      pfd.events = 0 & POLLIN;
      pfd.revents = 0;
      if(0 <= poll(&pfd, 1, -1))
      {
	      perror(NULL);
      }

      if(pfd.revents && POLLIN)
      {
	      while(DTLSv1_listen(ssl, &client) <= 0);
	      server_echo(&client, ssl);
      }
    }



  return;
}

void server_echo(struct sockaddr_in *client, SSL* ssl)
{
	return;
}
