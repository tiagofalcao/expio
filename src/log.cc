#include "expio.h"

#include <csignal>
#include <cstring>
#include <pthread.h>
#include <unistd.h>

#ifdef EXPIO_LOG_LEVEL_DEFAULT
static EXPIO_Log_Level _expio_log_level =
    (EXPIO_Log_Level)EXPIO_LOG_LEVEL_DEFAULT;
#else
static EXPIO_Log_Level _expio_log_level = EXPIO_LOG_LEVEL_ERR;
#endif

static bool _expio_log_color_enable = true;
pthread_t main_thread;
uint64_t rtdsc_init = 0;

static inline bool _expio_log_term_color_supported(const char *term) {
  const char *tail;
  size_t len;

  if (term == nullptr) { return false; }

  len = strlen(term);
  tail = term + 1;
  switch (term[0]) {
  /* list of known to support color terminals,
   * take from gentoo's portage.
   */

  case 'x': /* xterm and xterm-(256)color */
    return ((strncmp(tail, "term", sizeof("term") - 1) == 0) &&
            ((tail[sizeof("term") - 1] == '\0') ||
             (strcmp(term + len - sizeof("color") + 1, "color") == 0)));

  case 'E': /* Eterm */
  case 'a': /* aterm */
  case 'k': /* kterm */
    return (strcmp(tail, "term") == 0);

  case 'r': /* xrvt or rxvt-unicode */
    return ((strncmp(tail, "xvt", sizeof("xvt") - 1) == 0) &&
            ((tail[sizeof("xvt") - 1] == '\0') ||
             (strcmp(tail + sizeof("xvt") - 1, "-unicode") == 0)));

  case 's': /* screen */
    return ((strncmp(tail, "creen", sizeof("creen") - 1) == 0) &&
            ((tail[sizeof("creen") - 1] == '\0') ||
             (strcmp(term + len - sizeof("color") + 1, "color") == 0)));

  case 'g': /* gnome */
    return (strcmp(tail, "nome") == 0);

  case 'i': /* interix */
    return (strcmp(tail, "nterix") == 0);

  default:
    return false;
  }
}

bool expio_log_init(void) {
  const char *level, *tmp;

  rtdsc_init = expio_cycles();

  if ((tmp = getenv("EXPIO_LOG_COLOR")) != nullptr) {
    _expio_log_color_enable = atoi(tmp) > 0;
  }

#ifndef _WIN32
  /* color was not explicitly disabled or enabled, guess it */
  else if (_expio_log_color_enable) {
    if (_expio_log_term_color_supported(getenv("TERM"))) {
      _expio_log_color_enable = false;
    } else {
      /* if not a terminal, but redirected to a file, disable color */
      if (isatty(STDERR_FILENO) == 0) {
        _expio_log_color_enable = false;
      }
    }
  }
#endif

  // Global log level
  if ((level = getenv("EXPIO_LOG_LEVEL")) != nullptr) {
    _expio_log_level = static_cast<EXPIO_Log_Level>(atoi(level));
  }

  main_thread = pthread_self();

  return true;
}

void expio_log_level_global_set(EXPIO_Log_Level level) {
  _expio_log_level = level;
}

bool expio_log_shutdown(void) { return true; }

static const char *_names[] = {
    "CRI", "ERR", "WRN", "INF", "DBG",
};

static const char *_colors[] = {
    EXPIO_LOG_COLOR_RED,   EXPIO_LOG_COLOR_LIGHTRED,  EXPIO_LOG_COLOR_YELLOW,
    EXPIO_LOG_COLOR_GREEN, EXPIO_LOG_COLOR_LIGHTBLUE,
};

#define EXPIO_LOG_UNLIKELY(exp) __builtin_expect((exp), 0)
#define EXPIO_LOG_LIKELY(exp) __builtin_expect((exp), 1)

static inline void _expio_log_print_prefix(FILE *fp, EXPIO_Log_Level level,
                                           const char *file, const char *fnc,
                                           int line) {
  uint64_t timestamp = expio_cycles();
  pthread_t thread = pthread_self();

  if (EXPIO_LOG_UNLIKELY(level < 0)) {
    level = EXPIO_LOG_LEVEL_CRITICAL;
  } else if (EXPIO_LOG_UNLIKELY(level >= EXPIO_LOG_LEVELS)) {
    level = EXPIO_LOG_LEVEL_DBG;
  }

  if (_expio_log_color_enable) {
    if (EXPIO_LOG_LIKELY(pthread_equal(thread, main_thread))) {
      fprintf(fp,
              "%010" PRIX64 "  %s%s" EXPIO_LOG_COLOR_RESET
              ": %s:%d " EXPIO_LOG_COLOR_HIGH "%s()" EXPIO_LOG_COLOR_RESET " ",
              timestamp - rtdsc_init, _colors[level], _names[level], file, line,
              fnc);
    } else {
      fprintf(fp,
              "%010" PRIX64 " %10lu\t%s%s" EXPIO_LOG_COLOR_RESET
              ": %s:%d " EXPIO_LOG_COLOR_HIGH "%s()" EXPIO_LOG_COLOR_RESET " ",
              timestamp - rtdsc_init, thread, _colors[level],
              _names[level], file, line, fnc);
    }
  } else {
    if (EXPIO_LOG_LIKELY(pthread_equal(thread, main_thread))) {
      fprintf(fp, "%010" PRIX64 "  %s: %s:%d %s() ", timestamp - rtdsc_init,
              _names[level], file, line, fnc);
    } else {
      fprintf(fp, "%010" PRIX64 " %10lu\t%s: %s:%d %s() ",
              timestamp - rtdsc_init, thread, _names[level], file,
              line, fnc);
    }
  }
}

static inline void
_expio_log_print_cb_stderr(EXPIO_Log_Level level, const char *file,
                           const char *fnc, int line, const char *fmt,
                           __attribute__((unused)) void *data, va_list args) {
  flockfile(stderr);
  _expio_log_print_prefix(stderr, level, file, fnc, line);
  vfprintf(stderr, fmt, args);
  putc('\n', stderr);
  funlockfile(stderr);
}

static inline void _expio_log_print_unlocked(EXPIO_Log_Level level,
                                             const char *file, const char *fnc,
                                             int line, const char *fmt,
                                             va_list args) {
  if (level > _expio_log_level) { return; }

  _expio_log_print_cb_stderr(level, file, fnc, line, fmt, nullptr, args);
}

void expio_log_print(EXPIO_Log_Level level, const char *file, const char *fnc,
                     int line, const char *fmt, ...) {
  va_list args;

  va_start(args, fmt);
  _expio_log_print_unlocked(level, file, fnc, line, fmt, args);
  va_end(args);

  fflush(stdout); // Optional
  if (level <= 0) {
    raise(SIGTSTP);
  }
}
