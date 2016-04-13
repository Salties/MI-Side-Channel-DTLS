#include "sys/rtimer.h"

clock_time_t timing_start;
clock_time_t timing_end;
clock_time_t last_start;
clock_time_t last_end;

#define TIMING_START() ((timing_start = RTIMER_NOW()))
#define TIMING_END() ((timing_end = RTIMER_NOW()))

void save_last_period();
clock_time_t get_last_period();
