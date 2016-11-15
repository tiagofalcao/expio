EXPio
=====

**A Library for experiments.**


License
-------
 - This module is distributed under the GNU Affero General Public License v3.0.
   See the [License](LICENSE.TXT) file for details.

Build
------------
Optional requirement: sqlite3-dev.

1. Build
```bash
make
```

2. Build and Test
```bash
make test
```

Installation
------------

```bash
make install PREFIX=/usr/local/
```

Use
---

### Log

```C
expio_log_init();
EXPIO_LOG(20, "Verbose!!");
EXPIO_LOG_DBG("This is only a debug message, very verbose.");
EXPIO_LOG_INFO("This is only a informative message, verbose.");
EXPIO_LOG_WARN("This is only a warning message, anything is not perfect.");
EXPIO_LOG_ERR("This is a error message, ops.");
EXPIO_LOG_CRIT("This is a critical message, bye!");
EXPIO_LOG(-99, "This is a critical message, bye!");
expio_log_shutdown();
```

### Timing instrumentation

```C
expio_stats_t *stats = new_expio_stats("expA", "throughput", "1.0", "serverA", "desc", 100);

expio_stats_begin(stats);
for(int count=100; count>0; count--){
  task();
}
expio_stats_end(stats);

if (!expio_stats_write(stats))
  return ERROR;

expio_stats_delete(stats);
```
See the [tests](./tests) for more examples.

More
----

Thanks for the interest. We hope you enjoy using this library!
