#include "expio.h"
#include <unistd.h>

int main() {
  static const char *file = "/tmp/expio_stats_unittest.tsv";
  expio_stats_t *stats =
      expio_stats_tsv_new(file, "unittest", "write", "VERSION", "tsv", "", 3);

  expio_stats_begin(stats);
  sleep(1);
  sleep(1);
  sleep(1);
  expio_stats_end(stats);

  if (!expio_stats_write(stats)) {
    return 1;
  }

  expio_stats_delete(stats);

  return 0;
}
