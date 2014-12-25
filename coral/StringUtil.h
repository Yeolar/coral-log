/*
 * Copyright (C) 2014, Yeolar <yeolar@gmail.com>
 */

#ifndef CORAL_STRINGUTIL_H
#define CORAL_STRINGUTIL_H

namespace coral {

/**
 * A countable version of snprintf/vsnprintf.
 */
int scnprintf(char *buf, size_t size, const char *format, ...)
  __attribute__ ((format (printf, 3, 4)));

int vscnprintf(char *buf, size_t size, const char *format, va_list args);

} // namespace coral

#endif // CORAL_STRINGUTIL_H
