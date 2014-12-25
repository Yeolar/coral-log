/*
 * Copyright (C) 2014, Yeolar <yeolar@gmail.com>
 */

#ifndef CORAL_TIME_H
#define CORAL_TIME_H

#include <ctime>
#include <string>

namespace coral {

/**
 * time_printf is much like strftime but deposits its result into a
 * string. Two signatures are supported: the first simply returns the
 * resulting string, and the second appends the produced characters to
 * the specified string and returns a reference to it.
 */
std::string timePrintf(const char *format, const struct tm *tm);

void timeAppendf(std::string *out, const char *format, const struct tm *tm);

/**
 * time_now_printf is a specialized time_printf for print current time.
 */
inline std::string timeNowPrintf(const char *format) {
  time_t t = time(nullptr);
  return timePrintf(format, localtime(&t));
}

inline void timeNowAppendf(std::string *out, const char *format) {
  time_t t = time(nullptr);
  timeAppendf(out, format, localtime(&t));
}

// Some common time format.
const char *const EMAIL_TIME_FORMAT = "%a, %d %b %Y %T GMT";

} // namespace coral

#endif // CORAL_TIME_H
