#include "expio.h"
#include <unistd.h>

int main() {
#ifdef HAVE_SQLITE3
  static const char *file = "/tmp/expio_stats_unittest.sqlite3";
  expio_stats_t *stats = expio_stats_sqlite3_new(file, "unittest", "write",
                                             "VERSION", "sqlite3", "", 3);

  expio_stats_begin(stats);
  sleep(1);
  sleep(1);
  sleep(1);
  expio_stats_end(stats);

  if (!expio_stats_write(stats)) { return 1; }

  expio_stats_delete(stats);
#endif

  return 0;
}
