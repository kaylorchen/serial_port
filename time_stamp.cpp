//
// Created by kaylor on 2022/4/6.
//

#include "time_stamp.h"
#include <sys/time.h>

timestamp_t TimeStamp::now()
{
  struct timeval tv;
  gettimeofday(&tv,0);
  timestamp_t time=(timestamp_t)tv.tv_sec*1000+tv.tv_usec/1000;
  return time;
}