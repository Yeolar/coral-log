/*
 * Copyright (C) 2014, Yeolar <yeolar@gmail.com>
 */

#include <cerrno>
#include <system_error>

#include <coral/Logging.h>
#include <coral/StringUtil.h>

namespace coral {
namespace logging {

static LogWriter *logger = nullptr;

LogWriter::LogWriter(Level level, const char *name)
  : level_(level) {
  if (name && strlen(name) > 0) {
    file_ = fopen(name, "a+");
    if (!file_) {
      throw std::system_error(
          errno, std::system_category(), "open log file failed");
    }
  }
  else {
    file_ = stderr;
  }
}

LogWriter::~LogWriter() {
  if (file_ != NULL && file_ != stderr)
    fclose(file_);
}

void LogWriter::log(Level level,
                    const char *file,
                    int line,
                    const char *format, ...) {
  char buf[BUF_SIZE];
  size_t len = 0;
  size_t size = BUF_SIZE;
  time_t t = time(nullptr);

  len += scnprintf(buf + len, size - len, "[%c ", getLevelName(level)[0]);
  len += strftime (buf + len, size - len, "%y%m%d %T", localtime(&t));
  len += scnprintf(buf + len, size - len, " %s:%d] ", file, line);

  va_list args;
  va_start(args, format);
  len += vscnprintf(buf + len, size - len, format, args);
  va_end(args);
  buf[len++] = '\n';
  logString(buf, len);
}

// Hexadecimal dump in the canonical hex + ascii display
// See -C option in man hexdump
void LogWriter::logHexdump(const char *file,
                           int line,
                           char *data,
                           int datalen,
                           const char *format, ...) {
  char buf[8 * BUF_SIZE];
  int off = 0;    // data offset
  size_t len = 0;
  size_t size = 8 * BUF_SIZE;
  time_t t = time(nullptr);
  int i;

  len += strftime (buf + len, size - len, "[- %y%m%d %T", localtime(&t));
  len += scnprintf(buf + len, size - len, " %s:%d] ", file, line);

  va_list args;
  va_start(args, format);
  len += vscnprintf(buf + len, size - len, format, args);
  va_end(args);
  buf[len++] = '\n';

  while (datalen != 0 && (len < size - 1)) {
    char *save;
    int savelen;
    const char *str;
    unsigned char c;

    len += scnprintf(buf + len, size - len, "%08x  ", off);
    save = data;
    savelen = datalen;
    for (i = 0; datalen != 0 && i < 16; data++, datalen--, i++) {
      c = *data;
      str = (i == 7) ? "  " : " ";
      len += scnprintf(buf + len, size - len, "%02x%s", c, str);
    }
    for ( ; i < 16; i++) {
      str = (i == 7) ? "  " : " ";
      len += scnprintf(buf + len, size - len, "  %s", str);
    }
    data = save;
    datalen = savelen;
    len += scnprintf(buf + len, size - len, "  |");
    for (i = 0; datalen != 0 && i < 16; data++, datalen--, i++) {
      c = isprint(*data) ? *data : '.';
      len += scnprintf(buf + len, size - len, "%c", c);
    }
    len += scnprintf(buf + len, size - len, "|\n");
    off += 16;
  }
  logString(buf, len);
}

void initialize(Level level, const char *name) {
  if (!logger)
    logger = new LogWriter(level, name);
}

LogWriter *get() {
  if (!logger) {
    logger = new LogWriter(Level::INFO);
    LOG(WARN) << "Logger uninitialized, "
                 "use INFO level and output to stderr by default.";
  }
  return logger;
}

} // namespace logging
} // namespace coral
