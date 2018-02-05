#ifndef EXPIO_STATS_H_
#define EXPIO_STATS_H_

#include <inttypes.h>
#include <stdbool.h>

typedef struct _expio_stats {
  uint64_t timestamp;
  const char *fqdn;
  double frequency;
  char *experiment;
  char *instance;
  char *version;
  char *target;
  char *description;

  char *file;
  uint64_t begin_time_ns;
  uint64_t end_time_ns;
  uint64_t begin_ticks;
  uint64_t end_ticks;
  uint32_t repetitions;

  void (*begin)(struct _expio_stats *);
  void (*end)(struct _expio_stats *);
  bool (*write)(struct _expio_stats *);
  void (*destructor)(struct _expio_stats **);
} expio_stats_t;

expio_stats_t *expio_stats_new(const char *experiment, const char *instance,
                               const char *version, const char *target,
                               const char *description, uint32_t repetitions);

expio_stats_t *expio_stats_tsv_new(const char *file, const char *experiment,
                                   const char *instance, const char *version,
                                   const char *target, const char *description,
                                   uint32_t repetitions);

expio_stats_t *expio_stats_sqlite3_new(const char *file, const char *experiment,
                                       const char *instance,
                                       const char *version, const char *target,
                                       const char *description,
                                       uint32_t repetitions);

#define expio_stats_begin(STATS) (*(STATS->begin))(STATS)
#define expio_stats_end(STATS) (*(STATS->end))(STATS)
#define expio_stats_write(STATS) (*(STATS->write))(STATS)
#define expio_stats_delete(STATS) (*(STATS->destructor))(&STATS)

#endif
