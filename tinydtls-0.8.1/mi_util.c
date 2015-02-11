#include "mi_util.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <error.h>

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
