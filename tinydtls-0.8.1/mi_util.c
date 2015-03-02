#include "mi_util.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <error.h>

#define HAS_RESPOND 1
#define NO_RESPOND 0
#define MAX_PACKET 1400

//For general use.
int mi_sessiondone = 0;

unsigned int mi_rand()
{
    static int fd = 0;
    unsigned int r;

    if (!fd)
      {
	  if (-1 == (fd = open("/dev/urandom", O_RDONLY)))
	    {
		perror("open");
		fd = 0;
		return 0;
	    }
      }
    if (-1 == (read(fd, &r, sizeof(r))))
	perror("read");
    return r;
}


//For Single Or Even toy
void SingleOrEven_C(void *buf, size_t * len)
{
    unsigned int r;

    *len = sizeof(r);
    r = mi_rand();
    memcpy(buf, &r, *len);

    return;
}

int SingleOrEven_S(void *inbuf, size_t inlen, void *outbuf, size_t * outlen)
{
    unsigned int *r = inbuf;
    printf("Number:\t%u\n", *r);

    if (*r % 2)
      {
	  printf("Single\n");
	  strncpy(outbuf, "Single\n", *outlen);
      }
    else
      {
	  printf("Even\n");
	  strncpy(outbuf, "Even\n", *outlen);
      }
    *outlen = strlen(outbuf);

    return HAS_RESPOND;
}


//For Leaky Coffee toy
typedef enum { AMERICANO, CAPPUCCINO, ESPRESSO, MOCHA, FLAVOUR, EMPTY } Coffee;
#define NCOFFEE 4
#define STR_AMERICANO "AMERICANO"
#define STR_CAPPUCCINO "CAPPUCCINO"
#define STR_ESPRESSO "ESPRESSO"
#define STR_MOCHA "MOCHA"
#define STR_FLAVOUR "FLAVOUR"

#define MAX_DEGREE 4
#define MAX_DRINK_TIME 10
#define COFFEE_COOLDOWN_TIME 5
#define SUGAR '*'
#define MILK '*'
#define STR_SUGAR "*"
#define STR_MILK "@"
#define MOOD 0

Coffee TasteCoffee(uint8_t * buf, size_t len)
{
    char *order = (char *) buf;
    if (!strncmp(order, STR_AMERICANO, sizeof(STR_AMERICANO) - 1))
	return AMERICANO;
    else if (!strncmp(order, STR_CAPPUCCINO, sizeof(STR_CAPPUCCINO) - 1))
	return CAPPUCCINO;
    else if (!strncmp(order, STR_ESPRESSO, sizeof(STR_ESPRESSO) - 1))
	return ESPRESSO;
    else if (!strncmp(order, STR_MOCHA, sizeof(STR_MOCHA) - 1))
	return MOCHA;
    else if (!strncmp(order, STR_FLAVOUR, sizeof(STR_FLAVOUR) - 1))
	return FLAVOUR;

    return EMPTY;
}

int TasteSugar(char *cup, size_t len)
{
    int degree, i;

    for (i = 0, degree = 0; i < len; i++)
	if (cup[i] == SUGAR)
	    degree++;

    return degree;
}

int TasteMilk(char *cup, size_t len)
{
    int degree, i;

    for (i = 0, degree = 0; i < len; i++)
	if (cup[i] == MILK)
	    degree++;

    return degree;
}

int AddSugar(char *buf, int degree)
{
    int i;
    char *cup = (char *) buf;

    if (degree == MOOD)
	degree = mi_rand() % MAX_DEGREE;
    for (i = 0; i < degree; i++)
	strcat(cup, STR_SUGAR);

    return degree;
}

int AddMilk(char *buf, int degree)
{
    int i;
    char *cup = (char *) buf;

    if (degree == MOOD)
	degree = mi_rand() % MAX_DEGREE;
    for (i = 0; i < degree; i++)
	strcat(cup, STR_MILK);

    return degree;
}

void DrinkCoffee(int seconds)
{
    int i;
    printf("Drinking");
    fflush(stdout);
    for (i = 0; i < seconds; i++)
      {
	  sleep(1);
	  printf(".");
	  fflush(stdout);
      }
    printf("Done.\n");
    return;
}

int OrderCoffee_C(uint8_t * buf, size_t * len)
{
    Coffee choice = mi_rand() % NCOFFEE;
    char *coffeename;

    mi_sessiondone = 0;

    switch (choice)
      {
      case AMERICANO:
	  coffeename = STR_AMERICANO;
	  break;

      case CAPPUCCINO:
	  coffeename = STR_CAPPUCCINO;
	  break;

      case ESPRESSO:
	  coffeename = STR_ESPRESSO;
	  break;

      case MOCHA:
	  coffeename = STR_MOCHA;
	  break;

      default:
	  coffeename = "OUT_OF_ORDER";
	  return 0;
	  break;
      }

    strcpy((char *) buf, coffeename);
    *len = strlen(coffeename) + 1;	// +1 for '\0'.

    return 1;
}

int LeakyCoffee_C(struct dtls_context_t *ctx, session_t * dst, uint8_t * buf, size_t * len)
{
    static int sugarpref = 0, milkpref = 0;
    char *cup = (char *) buf;
    int sugardiff, milkdiff;
    Coffee servedcoffee;

    switch (servedcoffee = TasteCoffee(buf, *len))
      {
      case AMERICANO:
      case CAPPUCCINO:
      case MOCHA:
	  sugarpref = mi_rand() % MAX_DEGREE;
	  milkpref = mi_rand() % MAX_DEGREE;
	  sugardiff = TasteSugar(cup, *len) - sugarpref;
	  milkdiff = TasteMilk(cup, *len) - milkpref;
	  if (sugardiff >= 0 && milkdiff >= 0)
	    {
		DrinkCoffee(COFFEE_COOLDOWN_TIME + mi_rand() % MAX_DRINK_TIME);
		mi_sessiondone = 1;
	    }
	  else
	    {
		char * flavourbuf = malloc(20);
		printf("Need more flavour...\t");
		fflush(stdout);
		memset(buf, 0, *len);
		strcpy(flavourbuf, STR_FLAVOUR);
		AddSugar(flavourbuf, 0 - sugardiff);
		AddMilk(flavourbuf, 0 - sugardiff);
		dtls_write(ctx, dst, (uint8_t *) flavourbuf, strlen(flavourbuf)+1);
		free(flavourbuf);
	    }
	  break;
      case ESPRESSO:
	  sugarpref = 0;
	  milkpref = 0;
	  DrinkCoffee(COFFEE_COOLDOWN_TIME + mi_rand() % MAX_DRINK_TIME);
	  mi_sessiondone = 1;
	  break;
      case FLAVOUR:
	  printf("got flavour.\n");
	  DrinkCoffee(COFFEE_COOLDOWN_TIME + mi_rand() % MAX_DRINK_TIME);
	  mi_sessiondone = 1;
	  break;
      default:
	  break;
      }

    return 0;
}

int LeakyCoffee_S(uint8_t * inbuf, size_t inlen, uint8_t * outbuf, size_t * outlen)
{
    char *order = (char*)inbuf;
    char *cup = (char*) outbuf;
    int sugardegree, milkdegree;

    memset(outbuf, 0, *outlen);
    switch (TasteCoffee(inbuf, inlen))
      {
      case AMERICANO:
	  printf("%s ordered.\n", STR_AMERICANO);
	  strcat(cup, STR_AMERICANO);
	  AddMilk(cup, MOOD);
	  AddSugar(cup, MOOD);
	  break;

      case CAPPUCCINO:
	  printf("%s ordered.\n", STR_CAPPUCCINO);
	  strcat(cup, STR_CAPPUCCINO);
	  AddMilk(cup, MOOD);
	  AddSugar(cup, MOOD);
	  break;

      case ESPRESSO:
	  printf("%s ordered.\n", STR_ESPRESSO);
	  strcat(cup, STR_ESPRESSO);
	  break;

      case MOCHA:
	  printf("%s ordered.\n", STR_MOCHA);
	  strcat(cup, STR_MOCHA);
	  AddMilk(cup, MOOD);
	  AddSugar(cup, MOOD);
	  break;

      case FLAVOUR:
	  printf("More FLAVOUR ordered.\n");
	  strcat(cup, STR_FLAVOUR);
	  sugardegree = TasteSugar(order, inlen);
	  milkdegree = TasteMilk(order, inlen);
	  AddSugar(cup, sugardegree);
	  AddMilk(cup, milkdegree);
	  break;

      default:
	  printf("Ignored bad order.\n");
	  return 0;
      }
    *outlen = strlen((char *) outbuf);

    return 1;
}
