#include "timing.h"

static clock_time_t last_period;

void save_last_period()
{
	last_start = timing_start;
	last_end = timing_end;
	last_period = last_end - last_start;
	return;
}

clock_time_t get_last_period()
{
	return last_period;
}
