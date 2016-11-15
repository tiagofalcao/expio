#ifndef EXPIO_CYCLES_H_
#define EXPIO_CYCLES_H_

#include <stdint.h>

#if defined(__i386__)
static inline uint64_t expio_cycles() {
  unsigned long long int x;
  __asm__ volatile(".byte 0x0f, 0x31" : "=A"(x));
  return (uint64_t)x;
}
#elif defined(__x86_64__)
static inline uint64_t expio_cycles() {
  unsigned int hi, lo;
  __asm__ volatile("rdtsc" : "=a"(lo), "=d"(hi));
  return ((uint64_t)hi << 32) | lo;
}
#elif defined(__powerpc__)
static inline uint64_t expio_cycles() {
  uint64_t result = 0;
  unsigned long int upper, lower, tmp;
  __asm__ volatile("0:                  \n"
                   "\tmftbu   %0           \n"
                   "\tmftb    %1           \n"
                   "\tmftbu   %2           \n"
                   "\tcmpw    %2,%0        \n"
                   "\tbne     0b         \n"
                   : "=r"(upper), "=r"(lower), "=r"(tmp));
  result = upper;
  result = result << 32;
  result = result | lower;

  return result;
}
#else
#error "No tick counter is available!"
#endif

#endif
