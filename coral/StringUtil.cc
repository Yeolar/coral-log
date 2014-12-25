/*
 * Copyright (C) 2014, Yeolar <yeolar@gmail.com>
 */

#include <cstdarg>
#include <cstdio>

#include <coral/StringUtil.h>

namespace coral {

int scnprintf(char *buf, size_t size, const char *format, ...) {
  va_list ap;
  va_start(ap, format);
  int n = vscnprintf(buf, size, format, ap);
  va_end(ap);
  return n;
}

int vscnprintf(char *buf, size_t size, const char *format, va_list args) {
  int n = vsnprintf(buf, size, format, args);

  //
  // The return value is the number of characters which would be written
  // into buf not including the trailing '\0'. If size is == 0 the
  // function returns 0.
  //
  // On error, the function also returns 0. This is to allow idiom such
  // as len += _vscnprintf(...)
  //
  // See: http://lwn.net/Articles/69419/
  //
  if (n <= 0)
    return 0;
  if (n < (int) size)
    return n;
  return size - 1;
}

} // namespace coral
