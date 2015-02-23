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
unsigned int
mi_rand ()
{
  static int fd = 0;
  unsigned int r;

  if (!fd)
    {
      if (-1 == (fd = open ("/dev/urandom", O_RDONLY)))
	{
	  perror ("open");
	  fd = 0;
	  return 0;
	}
    }
  if (-1 == (read (fd, &r, sizeof (r))))
    perror ("read");
  return r;
}


//For Single Or Even toy
void
SingleOrEven_C (void *buf, size_t * len)
{
  unsigned int r;

  *len = sizeof (r);
  r = mi_rand ();
  memcpy (buf, &r, *len);

  return;
}

int
SingleOrEven_S (void *inbuf, size_t inlen, void *outbuf, size_t * outlen)
{
  unsigned int *r = inbuf;
  printf ("Number:\t%u\n", *r);

  if (*r % 2)
    {
      printf ("Single\n");
      strncpy (outbuf, "Single\n", *outlen);
    }
  else
    {
      printf ("Even\n");
      strncpy (outbuf, "Even\n", *outlen);
    }
  *outlen = strlen (outbuf);

  return HAS_RESPOND;
}


//For Leaky Coffee toy
typedef enum {AMERICANO, CAPPUCCINO, ESPRESSO, MOCHA, FLAVOUR, EMPTY} Coffee;
#define NCOFFEE 4
#define STR_AMERICANO "AMERICANO"
#define STR_CAPPUCCINO "CAPPUCCINO"
#define STR_ESPRESSO "ESPRESSO"
#define STR_MOCHA "MOCHA"
#define STR_FLAVOUR "FLAVOUR"

#define MAX_DEGREE 5
#define SUGAR "*"
#define MILK "@"

int
OrderCoffee_C (uint8_t * buf, size_t * len)
{
  Coffee choice = mi_rand() % NCOFFEE;
  char * coffeename;

  switch(choice)
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
		  break;
  }

  strcpy((char*)buf, coffeename);
  *len = strlen(coffeename) + 1; // +1 for '\0'.

  return 0;
}

int
LeakyCoffee_C (uint8_t * buf, size_t * len)
{
  return 0;
}

Coffee ReadOrder(uint8_t * buf, uint8_t len)
{
	char *order = (char*)buf;
	if(!strcmp(order, STR_AMERICANO))
		return AMERICANO;
	else if(!strcmp(order, STR_CAPPUCCINO))
		return CAPPUCCINO;
	else if(!strcmp(order, STR_ESPRESSO))
		return ESPRESSO;
	else if(!strcmp(order, STR_MOCHA))
		return MOCHA;
	else if(!strncmp(order, STR_FLAVOUR, strlen(STR_FLAVOUR)))
		return FLAVOUR;

	return EMPTY;
}

int AddSugar(uint8_t * buf)
{
	int i, degree;
	char *cup = (char*)buf;

	degree = mi_rand()%MAX_DEGREE;
	for(i = 0; i < degree; i++)
		strcat(cup, SUGAR);

	return degree;
}

int AddMilk(uint8_t * buf)
{
	int i, degree;
	char *cup = (char*)buf;

	degree = mi_rand()%MAX_DEGREE;
	for(i = 0; i < degree; i++)
		strcat(cup, MILK);

	return degree;
}
int
LeakyCoffee_S (uint8_t * inbuf, size_t inlen, uint8_t * outbuf,
		size_t * outlen)
{
	char *cup = (char*)outbuf;

	memset(outbuf, 0, *outlen);
	switch(ReadOrder(inbuf, inlen))
	{
		case AMERICANO:
			printf("%s ordered.\n", STR_AMERICANO);
			strcat(cup, STR_AMERICANO);
			break;

		case CAPPUCCINO:
			printf("%s ordered.\n", STR_CAPPUCCINO);
			strcat(cup, STR_CAPPUCCINO);
			break;

		case ESPRESSO:
			printf("%s ordered.\n", STR_ESPRESSO);
			strcat(cup, STR_ESPRESSO);
			break;

		case MOCHA:
			printf("%s ordered.\n", STR_MOCHA);
			strcat(cup, STR_MOCHA);
			break;

		case FLAVOUR:
			printf("More FLAVOUR required.\n");
			strcat(cup, STR_FLAVOUR);
			break;

		default:
			printf("Ignored bad order.\n");
			return 0;
	}

	return 1;
}
