#include "expio.h"

#include <string.h>
#include <sys/file.h>
#include <unistd.h>

bool expio_stats_tsv_write(expio_stats_t *stats) {
  char line[256];
  snprintf(line, 256, "%" PRIu64 "\t%s\t%s\t%s\t%s\t%s\t%s\t%" PRIu32
                      "\t%" PRIu64 "\t%" PRIu64 "\t%f\n",
           stats->timestamp, stats->fqdn, stats->experiment, stats->instance,
           stats->version, stats->target, stats->description,
           stats->repetitions, stats->end_time_ns - stats->begin_time_ns,
           stats->end_ticks - stats->begin_ticks, stats->frequency);

  int len = strlen(line);

  int fd = open(stats->file, O_APPEND | O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
  flock(fd, LOCK_EX);
  off_t seek = lseek(fd, 0, SEEK_END);
  if (seek == 0) {
    static const char *header = "Timestamp\tFQDN\tExperiment\tInstance\tVersion"
                                "\tTarget\tDescription\tRepetitions\tDuration\t"
                                "Ticks\tFrequency\n";
    write(fd, header, strlen(header));
  }
  write(fd, line, len);
  flock(fd, LOCK_UN);
  close(fd);

  return true;
}

expio_stats_t *expio_stats_tsv_new(const char *file, const char *experiment,
                               const char *instance, const char *version,
                               const char *target, const char *description,
                               uint32_t repetitions) {
  expio_stats_t *stats = expio_stats_new(experiment, instance, version, target,
                                     description, repetitions);
  stats->write = &expio_stats_tsv_write;
  stats->file = strdup(file);
  return stats;
}
