/*
 * Copyright (C) 2014, Yeolar <yeolar@gmail.com>
 */

/**
 * Logging routines and macros.
 */

#ifndef CORAL_LOGGING_H
#define CORAL_LOGGING_H

#include <csignal>
#include <cstdlib>
#include <cstring>

#include <coral/FixedStream.h>
#include <coral/Time.h>

namespace coral {

const char *baseName(const char *name) {
  const char *pos = strrchr(name, '/');
  return pos ? pos + 1 : name;
}

namespace logging {

// The LOG_ABORT macro terminates the application and generates a core dump
#ifdef LOG_USE_ABORT
# define LOG_ABORT abort()
#else
# define LOG_ABORT raise(SIGABRT)
#endif

/**
 * Output levels modelled after syslog
 */
enum class Level {
  EMERG  = 0,
  FATAL  = 0,
  ALERT  = 1,
  CRIT   = 2,
  ERROR  = 3,
  WARN   = 4,
  NOTICE = 5,
  INFO   = 6,
  DEBUG  = 7,
  NOTSET = 8
};

const size_t BUF_SIZE = 1024;

inline const char *getLevelName(Level level) {
  static const char *levelName[] = {
    "FATAL",
    "ALERT",
    "CRIT",
    "ERROR",
    "WARN",
    "NOTICE",
    "INFO",
    "DEBUG",
    "NOTSET"
  };
  return levelName[static_cast<int>(level)];
}

class LogWriter {
public:
  LogWriter(Level level, const char *name = nullptr);
  ~LogWriter();

  void setLevel(Level level) {
    level_ = level;
  }

  Level getLevel() const {
    return level_;
  }

  bool isEnabled(Level level) const {
    return level <= level_;
  }

  void log(Level level, const char *file, int line, const char *format, ...);

  // Hexadecimal dump in the canonical hex + ascii display
  // See -C option in man hexdump
  void logHexdump(const char *file, int line, char *data, int datalen,
                   const char *format, ...);

  void logString(const char *message, size_t len) {
    fwrite(message, sizeof(char), len, file_);
  }

private:
  Level level_;         // The current level (everything above is filtered)
  FILE *file_;          // The output file descriptor
};

/**
 * Public initialization function - creates a singleton instance of LogWriter
 */
extern void initialize(Level level, const char *name = nullptr);

/**
 * Accessor for the LogWriter singleton instance
 */
extern LogWriter *get();

class LogMessage {
public:
  LogMessage(Level level, const char *file, int line, bool abort)
    : out_(buf_, BUF_SIZE), abort_(abort) {
    out_ << "[" << getLevelName(level)[0] << timeNowPrintf(" %y%m%d %T ")
      << file << ":" << line << "] ";
  }

  ~LogMessage() {
    out_ << std::endl;
    std::string message = out_.str();
    get()->logString(message.c_str(), message.size());
    if (abort_)
      LOG_ABORT;
  }

  FixedOstream& stream() { return out_; }

private:
  char buf_[BUF_SIZE];
  FixedOstream out_;
  bool abort_;
};

} // namespace logging
} // namespace coral

// printf interface macro helper; do not use directly

#define LOG_FORMAT(_level, ...) do { \
  if (::coral::logging::get()->isEnabled(_level)) { \
    ::coral::logging::get()->log(_level, baseName(__FILE__), __LINE__, __VA_ARGS__); \
  } \
} while (0)

#define LOG_STREAM(_level, _abort) \
  (!::coral::logging::get()->isEnabled(_level)) ? \
    (void*) 0 : \
    ::coral::logging::LogMessage(_level, baseName(__FILE__), __LINE__, _abort).stream()

// Logging macros interface starts here

#define LOG_HEXDUMP(_data, _datalen, ...) do { \
  ::coral::logging::get()->logHexdump(baseName(__FILE__), __LINE__, \
    (char *)_data, (int)_datalen, __VA_ARGS__); \
} while (0)

#ifndef NDEBUG
#define LOG_DEBUG(...)  LOG_FORMAT(::coral::logging::Level::DEBUG, __VA_ARGS__)
#else
#define LOG_DEBUG(...)
#endif

#define LOG_INFO(...)   LOG_FORMAT(::coral::logging::Level::INFO, __VA_ARGS__)
#define LOG_NOTICE(...) LOG_FORMAT(::coral::logging::Level::NOTICE, __VA_ARGS__)
#define LOG_WARN(...)   LOG_FORMAT(::coral::logging::Level::WARN, __VA_ARGS__)
#define LOG_ERROR(...)  LOG_FORMAT(::coral::logging::Level::ERROR, __VA_ARGS__)
#define LOG_CRIT(...)   LOG_FORMAT(::coral::logging::Level::CRIT, __VA_ARGS__)
#define LOG_ALERT(...)  LOG_FORMAT(::coral::logging::Level::ALERT, __VA_ARGS__)
#define LOG_EMERG(...)  LOG_FORMAT(::coral::logging::Level::EMERG, __VA_ARGS__)

#define LOG_FATAL(...) do { \
  LOG_FORMAT(::coral::logging::Level::FATAL, __VA_ARGS__); \
  LOG_ABORT; \
} while (0)

#ifndef NDEBUG
#define LOG_STREAM_DEBUG  LOG_STREAM(::coral::logging::Level::DEBUG,  false)
#else
#define LOG_STREAM_DEBUG  LOG_STREAM(::coral::logging::Level::NOTSET, false)
#endif

#define LOG_STREAM_INFO   LOG_STREAM(::coral::logging::Level::INFO,   false)
#define LOG_STREAM_NOTICE LOG_STREAM(::coral::logging::Level::NOTICE, false)
#define LOG_STREAM_WARN   LOG_STREAM(::coral::logging::Level::WARN,   false)
#define LOG_STREAM_ERROR  LOG_STREAM(::coral::logging::Level::ERROR,  false)
#define LOG_STREAM_CRIT   LOG_STREAM(::coral::logging::Level::CRIT,   false)
#define LOG_STREAM_ALERT  LOG_STREAM(::coral::logging::Level::ALERT,  false)
#define LOG_STREAM_EMERG  LOG_STREAM(::coral::logging::Level::EMERG,  false)
#define LOG_STREAM_FATAL  LOG_STREAM(::coral::logging::Level::FATAL,  true)

#define LOG(_level) LOG_STREAM_ ## _level
#define PLOG(_level) LOG_STREAM_ ## _level << strerror(errno) << " "

#define LOG_FIRST_N(_level, n) \
  static int LOG_OCCURRENCES = 0; \
  if (LOG_OCCURRENCES <= n) ++LOG_OCCURRENCES; \
  if (LOG_OCCURRENCES <= n) LOG(_level)

#define CHECK(_condition) \
  (_condition) ? (void*) 0 : LOG(FATAL) << "Check `" # _condition "' failed. "

#define PCHECK(_condition) \
  (_condition) ? (void*) 0 : PLOG(FATAL) << "Check `" # _condition "' failed. "

#define CHECK_ERR(_invocation) \
  ((_invocation) != -1) ? (void*) 0 : LOG(FATAL) << # _invocation

#endif // CORAL_LOGGING_H
