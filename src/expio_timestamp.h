#ifndef EXPIO_TIMESTAMP_H_
#define EXPIO_TIMESTAMP_H_

#include <stdint.h>
#include <time.h>

#define EXPIO_NANO_SECONDS_IN_SEC 1000000000LL;

inline static uint64_t expio_timestamp() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  uint64_t ret = ts.tv_sec;
  ret *= EXPIO_NANO_SECONDS_IN_SEC;
  ret += ts.tv_nsec;
  return ret;
}

#endif
