#ifndef EXPIO_LOG_H_
#define EXPIO_LOG_H_

#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#define EXPIO_LOG_COLOR_LIGHTRED "\033[31;1m"
#define EXPIO_LOG_COLOR_RED "\033[31m"
#define EXPIO_LOG_COLOR_LIGHTBLUE "\033[34;1m"
#define EXPIO_LOG_COLOR_BLUE "\033[34m"
#define EXPIO_LOG_COLOR_GREEN "\033[32;1m"
#define EXPIO_LOG_COLOR_YELLOW "\033[33;1m"
#define EXPIO_LOG_COLOR_ORANGE "\033[0;33m"
#define EXPIO_LOG_COLOR_WHITE "\033[37;1m"
#define EXPIO_LOG_COLOR_LIGHTCYAN "\033[36;1m"
#define EXPIO_LOG_COLOR_CYAN "\033[36m"
#define EXPIO_LOG_COLOR_RESET "\033[0m"
#define EXPIO_LOG_COLOR_HIGH "\033[1m"

typedef enum _EXPIO_Log_Level {
  EXPIO_LOG_LEVEL_CRITICAL,    /**< Critical log level */
  EXPIO_LOG_LEVEL_ERR,         /**< Error log level */
  EXPIO_LOG_LEVEL_WARN,        /**< Warning log level */
  EXPIO_LOG_LEVEL_INFO,        /**< Information log level */
  EXPIO_LOG_LEVEL_DBG,         /**< Debug log level */
  EXPIO_LOG_LEVELS,            /**< Count of default log levels */
  EXPIO_LOG_LEVEL_UNKNOWN = -1 /**< Unknown level */
} EXPIO_Log_Level;

void expio_log_print(EXPIO_Log_Level level, const char *file, const char *fnc,
                     int line, const char *fmt, ...);

bool expio_log_init(void);

void expio_log_level_global_set(EXPIO_Log_Level level);

bool expio_log_shutdown(void);

#ifdef EXPIO_LOG_LEVEL_MAXIMUM
#define EXPIO_LOG(LEVEL, fmt, ...)                                             \
  do {                                                                         \
    if (((EXPIO_Log_Level)LEVEL) <= EXPIO_LOG_LEVEL_MAXIMUM) {                 \
      expio_log_print((EXPIO_Log_Level)LEVEL, __FILE__, __FUNCTION__,          \
                      __LINE__, fmt, ##__VA_ARGS__);                           \
    }                                                                          \
  } while (0)
#else
#define EXPIO_LOG(LEVEL, fmt, ...)                                             \
  expio_log_print((EXPIO_Log_Level)LEVEL, __FILE__, __FUNCTION__, __LINE__,    \
                  fmt, ##__VA_ARGS__)
#endif

/**
 * @def EXPIO_LOG_CRIT(fmt, ...)
 * Logs a message with level CRITICAL on the default domain with the specified
 * format.
 */
#define EXPIO_LOG_CRIT(fmt, ...)                                               \
  EXPIO_LOG(EXPIO_LOG_LEVEL_CRITICAL, fmt, ##__VA_ARGS__)

/**
 * @def EXPIO_LOG_ERR(fmt, ...)
 * Logs a message with level ERROR on the default domain with the specified
 * format.
 */
#define EXPIO_LOG_ERR(fmt, ...)                                                \
  EXPIO_LOG(EXPIO_LOG_LEVEL_ERR, fmt, ##__VA_ARGS__)

/**
 * @def EXPIO_LOG_WARN(fmt, ...)
 * Logs a message with level WARN on the default domain with the specified
 * format.
 */
#define EXPIO_LOG_WARN(fmt, ...)                                               \
  EXPIO_LOG(EXPIO_LOG_LEVEL_WARN, fmt, ##__VA_ARGS__)

/**
 * @def EXPIO_LOG_DBG(fmt, ...)
 * Logs a message with level DEBUG on the default domain with the specified
 * format.
 */
#define EXPIO_LOG_DBG(fmt, ...)                                                \
  EXPIO_LOG(EXPIO_LOG_LEVEL_DBG, fmt, ##__VA_ARGS__)

/**
 * @def EXPIO_LOG_INFO(fmt, ...)
 * Logs a message with level INFO on the default domain with the specified
 * format.
 */
#define EXPIO_LOG_INFO(fmt, ...)                                               \
  EXPIO_LOG(EXPIO_LOG_LEVEL_INFO, fmt, ##__VA_ARGS__)

#endif
