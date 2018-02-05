#include "expio.h"

#ifdef HAVE_SQLITE3
#include <sqlite3.h>
#include <string.h>

bool expio_stats_sqlite3_write(expio_stats_t *stats) {
  sqlite3 *db;

  {
    int rc = sqlite3_open(stats->file, &db);
    if (rc != SQLITE_OK) {
      EXPIO_LOG_ERR("Can't open database %s: %s\n", stats->file,
                    sqlite3_errmsg(db));
      sqlite3_close(db);
      return false;
    }
  }

  sqlite3_busy_timeout(db, 2000);

  {
    static const char *sql =
        "CREATE TABLE IF NOT EXISTS stats (id INTEGER PRIMARY KEY"
        ", timestamp INTEGER"
        ", fqdn TEXT"
        ", experiment TEXT"
        ", instance TEXT"
        ", version TEXT"
        ", target TEXT"
        ", description TEXT"
        ", repetitions INTEGER"
        ", duration INTEGER"
        ", ticks INTEGER"
        ", frequency REAL"
        ");";
    sqlite3_stmt *res;

    if (sqlite3_prepare_v2(db, sql, -1, &res, 0) != SQLITE_OK) {
      EXPIO_LOG_ERR("Failed to execute statement: %s\n", sqlite3_errmsg(db));
      return false;
    }

    if (sqlite3_step(res) != SQLITE_DONE) {
      EXPIO_LOG_ERR("Failed to execute statement: %s\n", sqlite3_errmsg(db));
      return false;
    }

    sqlite3_finalize(res);
  }

  {
    static const char *sql =
        "INSERT INTO stats (timestamp, fqdn, experiment, instance, version, "
        "target, description, repetitions, duration, ticks, frequency) "
        "VALUES(@timestamp, @fqdn, @experiment, @instance, @version, @target, "
        "@description, @repetitions, @duration, @ticks, @frequency);";

    sqlite3_stmt *res;

    if (sqlite3_prepare_v2(db, sql, -1, &res, 0) != SQLITE_OK) {
      EXPIO_LOG_ERR("Failed to execute statement: %s\n", sqlite3_errmsg(db));
      return false;
    }

    int idx;
    idx = sqlite3_bind_parameter_index(res, "@timestamp");
    sqlite3_bind_int64(res, idx, stats->timestamp);
    idx = sqlite3_bind_parameter_index(res, "@fqdn");
    sqlite3_bind_text(res, idx, stats->fqdn, -1, SQLITE_STATIC);
    idx = sqlite3_bind_parameter_index(res, "@experiment");
    sqlite3_bind_text(res, idx, stats->experiment, -1, SQLITE_STATIC);
    idx = sqlite3_bind_parameter_index(res, "@instance");
    sqlite3_bind_text(res, idx, stats->instance, -1, SQLITE_STATIC);
    idx = sqlite3_bind_parameter_index(res, "@version");
    sqlite3_bind_text(res, idx, stats->version, -1, SQLITE_STATIC);
    idx = sqlite3_bind_parameter_index(res, "@target");
    sqlite3_bind_text(res, idx, stats->target, -1, SQLITE_STATIC);
    idx = sqlite3_bind_parameter_index(res, "@description");
    sqlite3_bind_text(res, idx, stats->description, -1, SQLITE_STATIC);
    idx = sqlite3_bind_parameter_index(res, "@repetitions");
    sqlite3_bind_int64(res, idx, stats->repetitions);
    idx = sqlite3_bind_parameter_index(res, "@duration");
    sqlite3_bind_int64(res, idx, stats->end_time_ns - stats->begin_time_ns);
    idx = sqlite3_bind_parameter_index(res, "@ticks");
    sqlite3_bind_int64(res, idx, stats->end_ticks - stats->begin_ticks);
    idx = sqlite3_bind_parameter_index(res, "@frequency");
    sqlite3_bind_double(res, idx, stats->frequency);

    if (sqlite3_step(res) != SQLITE_DONE) {
      EXPIO_LOG_ERR("Failed to execute statement: %s\n", sqlite3_errmsg(db));
      return false;
    }

    sqlite3_finalize(res);
  }

  sqlite3_close(db);
  return true;
}

expio_stats_t *expio_stats_sqlite3_new(const char *file, const char *experiment,
                                       const char *instance,
                                       const char *version, const char *target,
                                       const char *description,
                                       uint32_t repetitions) {
  expio_stats_t *stats = expio_stats_new(experiment, instance, version, target,
                                         description, repetitions);
  stats->write = &expio_stats_sqlite3_write;
  stats->file = strdup(file);
  return stats;
}

#else

expio_stats_t *expio_stats_sqlite3_new(const char *file, const char *experiment,
                                       const char *instance,
                                       const char *version, const char *target,
                                       const char *description,
                                       uint32_t repetitions) {
  (void)file;
  (void)experiment;
  (void)instance;
  (void)version;
  (void)target;
  (void)description;
  (void)repetitions;
  EXPIO_LOG_ERR("SQLite3 is not supported in this build\n");
  return nullptr;
}

#endif
