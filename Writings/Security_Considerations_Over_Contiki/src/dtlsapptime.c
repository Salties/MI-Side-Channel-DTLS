static int ApplicationCode(/*...DTLS arguments...*/) 
{
  clock_tick_t resp;
  //Time the execution of random_rand().
  start = RTIMER_NOW();
  for(i=0; i < RequestData; i++)
	  random_rand();
  end = RTIMER_NOW();  
  //Reply with the execution time.
  resp = end - start;
  dtls_write(ctx, session, &resp, resplen);
  return 0;
}
