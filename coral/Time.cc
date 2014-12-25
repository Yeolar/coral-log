/*
 * Copyright (C) 2014, Yeolar <yeolar@gmail.com>
 */

#include <cstring>
#include <stdexcept>

#include <coral/Time.h>

namespace coral {

namespace {

inline void timePrintfImpl(std::string& output,
                           const char *format,
                           const struct tm *tm) {
  const auto writePoint = output.size();
  size_t formatLen = strlen(format);
  size_t remaining = std::max(std::max(32UL, output.capacity() - writePoint),
                              formatLen * 2);
  output.resize(writePoint + remaining);
  int bytesUsed = 0;

  do {
    bytesUsed = strftime(&output[writePoint], remaining, format, tm);
    if (bytesUsed > 0) {
      break;  // There was enough room, break
    }
    else if (bytesUsed == 0) {
      remaining *= 2;
      if (remaining > formatLen * 16) {
        throw std::invalid_argument(
            std::string("Maybe a non-output format given: ") + format);
      }
      output.resize(writePoint + remaining);
    }
    else {
      throw std::runtime_error(
          "strftime should never return a negative value.");
    }
  } while (bytesUsed == 0);

  output.resize(writePoint + bytesUsed);
}

} // namespace

std::string timePrintf(const char *format, const struct tm *tm) {
  std::string ret;
  timePrintfImpl(ret, format, tm);
  return ret;
}

void timeAppendf(std::string *out, const char *format, const struct tm *tm) {
  timePrintfImpl(*out, format, tm);
}

} // namespace coral

