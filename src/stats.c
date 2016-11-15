#include "expio.h"

#include <string.h>

double getfrequency() {
  uint64_t begints, begints2, endts;
  uint64_t begin = 0, end = 0;
  begints = expio_timestamp();
  begints2 = expio_timestamp();

  begin = expio_cycles();
  for (uint64_t i = 0; i < 100000; i++)
    __asm__ __volatile__("");
  end = expio_cycles();

  endts = expio_timestamp();

  end -= begin;

  uint64_t nsecElapsed = endts - begints2 - begints2 + begints;

  return (((double)end) / nsecElapsed);
}

void expio_stats_default_begin(expio_stats_t *stats) {
  stats->begin_time_ns = expio_timestamp();
  stats->begin_ticks = expio_cycles();
}

void expio_stats_default_end(expio_stats_t *stats) {
  stats->end_ticks = expio_cycles();
  stats->end_time_ns = expio_timestamp();
  stats->frequency = getfrequency();
}

bool expio_stats_default_write(expio_stats_t *stats) {
  printf("STATS\t%" PRIu64 "\t%s\t%s\t%s\t%s\t%s\t%s\t%" PRIu32 "\t%" PRIu64
         "\t%" PRIu64 "\t%f\n",
         stats->timestamp, stats->fqdn, stats->experiment, stats->instance,
         stats->version, stats->target, stats->description, stats->repetitions,
         stats->end_time_ns - stats->begin_time_ns,
         stats->end_ticks - stats->begin_ticks, stats->frequency);
  return true;
}

#define FREE_STR(STR)                                                          \
  if ((*stats)->STR)                                                           \
    free((*stats)->STR);
void expio_stats_default_destructor(expio_stats_t **stats) {
  FREE_STR(experiment);
  FREE_STR(instance);
  FREE_STR(version);
  FREE_STR(target);
  FREE_STR(description);
  FREE_STR(file);

  free(*stats);
  *stats = NULL;
}

expio_stats_t *expio_stats_new(const char *experiment, const char *instance,
                           const char *version, const char *target,
                           const char *description, uint32_t repetitions) {
  expio_stats_t *stats;

  stats = (expio_stats_t *)malloc(sizeof(expio_stats_t));
  stats->timestamp = expio_timestamp();
  stats->fqdn = expio_fqdn_get();
  stats->experiment = strdup(experiment);
  stats->instance = strdup(instance);
  stats->version = strdup(version);
  stats->target = strdup(target);
  stats->description = strdup(description);
  stats->file = NULL;
  stats->repetitions = repetitions;

  stats->begin = &expio_stats_default_begin;
  stats->end = &expio_stats_default_end;
  stats->write = &expio_stats_default_write;
  stats->destructor = &expio_stats_default_destructor;

  return stats;
}
