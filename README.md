# corLog — Logging and Tracing

A comprehensive C logging library with support for multiple message types, fine-grained trace levels, and thread-safe output to files and/or stdout.

- **Version:** 0.1.0
- **Language:** C
- **License:** [Apache License 2.0](LICENSE)

The only dependency is **kbase**.

## Where it comes from

corLog is **ktrace** under the cor prefix, copied, not forked: ktrace itself is
untouched and keeps serving its own users. It was only ever called ktrace because
the name klog was taken; in the coraine stack it is the log, so it is called that.

The renames: `KT_E`/`KT_W`/`KT_T`/... → `COR_E`/`COR_W`/`COR_T`/..., `ktInit` →
`corLogInit`, `ktOut` → `corLogOut`, `ktTraceLevelSet` → `corLogTraceLevelSet` (and
the rest of `ktTraceLevel*`), the globals `ktVerbose`/`ktDebug`/... →
`corLogVerbose`/`corLogDebug`/.... A trace level is still a *trace* level — it is
not a log level, and the names say so. `KBool` became `bool`.

## Features

- **Multiple message types** - Debug, trace, verbose, info, warning, error, fatal
- **Fine-grained trace levels** - Up to 3200 individual trace levels
- **Thread-safe** - Mutex-protected output for multi-threaded applications
- **Flexible output** - Log to file, stdout, or both simultaneously
- **Timestamped** - Microsecond precision relative to initialization
- **Context-aware** - Automatically captures file, line, and function name

## API Reference

### Initialization

#### corLogInit

```c
int corLogInit(
    const char* progName,    // Program name (used for log filename)
    const char* logDir,      // Log directory (NULL for stdout only)
    bool       logToScreen, // Echo to stdout in addition to file
    const char* logLevel,    // "ERR", "WARN", "INFO", "VERBOSE", "TRACE", "DEBUG"
    const char* traceLevels, // Trace levels: "5,10,15-20,100"
    bool       verbose,     // Enable verbose output
    bool       debug,       // Enable debug output
    bool       fixme        // Enable FIXME markers
);
```

Initializes the logging system. Creates `<logDir>/<progName>.log`, backing up existing logs to `.old`.

### Trace Level Configuration

```c
void corLogTraceLevelSet(const char* levelString);  // "5,10,15-20"
void corLogTraceLevelSetOne(int level);         // Single level (0-3199)
bool corLogTraceLevelCheck(unsigned int level);    // Check if enabled
```

### Logging Macros

```c
COR_D(fmt, ...)             // Debug (temporary, to be removed)
COR_T(level, fmt, ...)      // Trace with level
COR_V(fmt, ...)             // Verbose
COR_I(fmt, ...)             // Info
COR_W(fmt, ...)             // Warning
COR_E(fmt, ...)             // Error
COR_X(exitCode, fmt, ...)   // Fatal error, then exit
COR_F(fmt, ...)             // FIXME marker

// Return variants
COR_RE(retVal, fmt, ...)    // Error, then return value
COR_RVE(fmt, ...)           // Error, then return void
```

### Output Format

```
<TYPE>: <TIMESTAMP>: <FILE>[<LINE>]: <FUNCTION>: <MESSAGE>
```

Example:
```
E: 000012.345: main.c[42]: processRequest: Connection refused
T: 000012.346: parser.c[100]: parse: entering parse loop (5)
```

## Building

```bash
make          # the library, and obj/debug/corLogTest - a smoke test, never installed
make clean    # remove build artifacts
```

## Usage Example

```c
#include "corLog/corLog.h"

// Define trace levels for your application
#define TL_PARSE    5
#define TL_NETWORK  10
#define TL_DATABASE 15

int main(int argc, char* argv[])
{
    // Initialize: log to /tmp/myapp.log and stdout, enable trace levels 5,10
    corLogInit("myapp", "/tmp", true, "TRACE", "5,10", true, false, false);

    COR_I("Application started");
    COR_V("Verbose startup info");

    COR_T(TL_PARSE, "Parsing configuration");
    COR_T(TL_NETWORK, "Connecting to server");
    COR_T(TL_DATABASE, "This won't appear - level 15 not enabled");

    if (error_condition) {
        COR_E("Something went wrong: %s", strerror(errno));
        COR_X(1, "Fatal error, exiting"); // Exits with code 1
    }

    COR_I("Application finished");
    return 0;
}
```

### Output

```
I: 000000.001: main.c[12]: main: Application started
V: 000000.001: main.c[13]: main: Verbose startup info
T: 000000.002: main.c[15]: main: Parsing configuration (5)
T: 000000.002: main.c[16]: main: Connecting to server (10)
I: 000000.050: main.c[24]: main: Application finished
```

## Trace Level Patterns

Trace levels allow fine-grained control over which messages appear:

```c
// Enable individual levels
corLogTraceLevelSet("5,10,100");

// Enable ranges
corLogTraceLevelSet("15-20");

// Combine both
corLogTraceLevelSet("5,10,15-20,100-150");
```

Use trace levels to control verbosity for different subsystems:

```c
#define TL_HTTP_REQUEST   10
#define TL_HTTP_RESPONSE  11
#define TL_DB_QUERY       20
#define TL_DB_RESULT      21
#define TL_CACHE_HIT      30
#define TL_CACHE_MISS     31
```

## Global Variables

```c
extern bool corLogInfo;      // Info messages enabled
extern bool corLogVerbose;   // Verbose messages enabled
extern bool corLogDebug;     // Debug messages enabled
extern bool corLogFixme;     // FIXME messages enabled
```

## Thread Safety

All logging is protected by a pthread mutex, ensuring log messages from different threads don't interleave.

## Dependencies

- [kbase](https://gitlab.com/kzangeli/kbase) - basic types and time functions
- pthread library

## License

[Apache 2.0](LICENSE) &copy; 2019-2026 Ken Zangelin
