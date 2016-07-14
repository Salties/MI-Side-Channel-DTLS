static unsigned long seed = 1;

int
rand (void)
{
  return do_rand (&rand);
}
