/*
 * Copyright (C) 2014, Yeolar <yeolar@gmail.com>
 */

/**
 * Fixed size string buffers.
 * A stringstream, istringstream, ostringstream backed by a fixed size
 * string buffer.
 */

#ifndef CORAL_FIXEDSTREAM_H
#define CORAL_FIXEDSTREAM_H

#include <istream>
#include <ostream>
#include <streambuf>
#include <string>

namespace coral {

/**
 * A simple streambuf with fixed size buffer.
 * Convenient for limitting size of output; faster than ostringstream
 * and friends, which require heap allocations
 */
class FixedStreamBuf : public std::streambuf {
public:
  FixedStreamBuf(char *buf, size_t len) {
    setp(buf, buf + len);
  }

  FixedStreamBuf(const char *buf, const char *next, const char *end) {
    setg((char *)buf, (char *)next, (char *)end);
  }

  std::string str() { return std::string(pbase(), pptr()); }
};

/**
 * Output stream using a fixed buffer
 */
class FixedOstream : private virtual FixedStreamBuf, public std::ostream {
public:
  typedef FixedStreamBuf StreamBuf;

  FixedOstream(char *buf, size_t len)
    : FixedStreamBuf(buf, len), std::ostream(static_cast<StreamBuf *>(this)) {
  }

  char *output() { return StreamBuf::pbase(); }

  char *output_ptr() { return StreamBuf::pptr(); }

  char *output_end() { return StreamBuf::epptr(); }

  std::string str() { return StreamBuf::str(); }
};

/**
 * Input stream using a fixed buffer
 */
class FixedIstream : private virtual FixedStreamBuf, public std::istream {
public:
  typedef FixedStreamBuf StreamBuf;

  FixedIstream(const char *buf, const char *end)
    : FixedStreamBuf(buf, buf, end)
    , std::istream(static_cast<StreamBuf *>(this)) {
  }

  char *input() { return StreamBuf::eback(); }

  char *input_ptr() { return StreamBuf::gptr(); }

  char *input_end() { return StreamBuf::egptr(); }
};

} // namespace coral

#endif // CORAL_FIXEDSTREAM_H
