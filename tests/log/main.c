/*
 * Use the EXPIO_LOG_LEVEL environment variable to define the execution level;
 * The call to expio_log_level_global_set overwrite the environment value;
 * The macro EXPIO_LOG_LEVEL_MAXIMUM defines the higher level supported in
 * build;
 * The macro EXPIO_LOG_LEVEL_DEFAULT defines the default runtime level in build.
 */

#include "expio.h"

int main() {
  expio_log_init();

  EXPIO_LOG(20, "Verbose!!");
  EXPIO_LOG_DBG("This is only a debug message, very verbose.");
  EXPIO_LOG_INFO("This is only a informative message, verbose.");
  EXPIO_LOG_WARN("This is only a warning message, anything is not perfect.");
  EXPIO_LOG_ERR("This is a error message, ops.");
  // EXPIO_LOG_CRIT("This is a critical message, bye!");
  // EXPIO_LOG(-99, "This is a critical message, bye!");

  expio_log_level_global_set((EXPIO_Log_Level)20);
  EXPIO_LOG(20, "Verbose!!");
  EXPIO_LOG_DBG("This is only a debug message, very verbose.");
  EXPIO_LOG_INFO("This is only a informative message, verbose.");

  expio_log_shutdown();
  return 0;
}
