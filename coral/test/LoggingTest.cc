/*
 * Copyright (C) 2014, Yeolar <yeolar@gmail.com>
 */

#include <gtest/gtest.h>
#include <coral/Logging.h>
#include <coral/Time.h>

using namespace coral;

TEST(Logging, LogLevel) {
  EXPECT_EQ(logging::get()->getLevel(), logging::Level::FATAL);
  LOG_DEBUG ("debug log, should not be output");
  LOG_INFO  ("info log, should not be output");
  LOG_NOTICE("notice log, should not be output");
  LOG_WARN  ("warning log, should not be output");
  LOG_ERROR ("error log, should not be output");
  LOG_CRIT  ("critical log, should not be output");
  LOG_ALERT ("alert log, should not be output");
  EXPECT_EQ(logging::get()->getLevel(), logging::Level::FATAL);

  logging::get()->setLevel(logging::Level::ALERT);
  EXPECT_EQ(logging::get()->getLevel(), logging::Level::ALERT);
  LOG_DEBUG ("debug log, should not be output");
  LOG_INFO  ("info log, should not be output");
  LOG_NOTICE("notice log, should not be output");
  LOG_WARN  ("warning log, should not be output");
  LOG_ERROR ("error log, should not be output");
  LOG_CRIT  ("critical log, should not be output");
  EXPECT_EQ(logging::get()->getLevel(), logging::Level::ALERT);

  logging::get()->setLevel(logging::Level::CRIT);
  EXPECT_EQ(logging::get()->getLevel(), logging::Level::CRIT);
  LOG_DEBUG ("debug log, should not be output");
  LOG_INFO  ("info log, should not be output");
  LOG_NOTICE("notice log, should not be output");
  LOG_WARN  ("warning log, should not be output");
  LOG_ERROR ("error log, should not be output");
  EXPECT_EQ(logging::get()->getLevel(), logging::Level::CRIT);

  logging::get()->setLevel(logging::Level::ERROR);
  EXPECT_EQ(logging::get()->getLevel(), logging::Level::ERROR);
  LOG_DEBUG ("debug log, should not be output");
  LOG_INFO  ("info log, should not be output");
  LOG_NOTICE("notice log, should not be output");
  LOG_WARN  ("warning log, should not be output");
  EXPECT_EQ(logging::get()->getLevel(), logging::Level::ERROR);

  logging::get()->setLevel(logging::Level::WARN);
  EXPECT_EQ(logging::get()->getLevel(), logging::Level::WARN);
  LOG_DEBUG ("debug log, should not be output");
  LOG_INFO  ("info log, should not be output");
  LOG_NOTICE("notice log, should not be output");
  EXPECT_EQ(logging::get()->getLevel(), logging::Level::WARN);

  logging::get()->setLevel(logging::Level::NOTICE);
  EXPECT_EQ(logging::get()->getLevel(), logging::Level::NOTICE);
  LOG_DEBUG ("debug log, should not be output");
  LOG_INFO  ("info log, should not be output");
  EXPECT_EQ(logging::get()->getLevel(), logging::Level::NOTICE);

  logging::get()->setLevel(logging::Level::INFO);
  EXPECT_EQ(logging::get()->getLevel(), logging::Level::INFO);
  LOG_DEBUG ("debug log, should not be output");
  EXPECT_EQ(logging::get()->getLevel(), logging::Level::INFO);

  logging::get()->setLevel(logging::Level::DEBUG);
  EXPECT_EQ(logging::get()->getLevel(), logging::Level::DEBUG);
}

TEST(Logging, LogFormat) {
  LOG_DEBUG ("debug log");
  LOG_INFO  ("info log");
  LOG_NOTICE("notice log");
  LOG_WARN  ("warning log");
  LOG_ERROR ("error log");
  LOG_CRIT  ("critical log");
  LOG_ALERT ("alert log");
  LOG_EMERG ("emerg log");

  LOG_INFO  ("log with 3 args: %s %d %.1f", "arg1", 2, 3.0);
  LOG_INFO  ("log check timestamp = '%s'",
             timeNowPrintf("%y%m%d %T").c_str());
  LOG_INFO  ("log check filename = " __FILE__);
  LOG_INFO  ("log check line = %d", __LINE__);
}

TEST(Logging, LogStream) {
  LOG(DEBUG)  << "debug log stream";
  LOG(INFO)   << "info log stream";
  LOG(NOTICE) << "notice log stream";
  LOG(WARN)   << "warning log stream";
  LOG(ERROR)  << "error log stream";
  LOG(CRIT)   << "critical log stream";
  LOG(ALERT)  << "alert log stream";
  LOG(EMERG)  << "emerg log stream";

  LOG(INFO)   << "log stream with 3 args: " << "arg1" << " " << 2 << " " << 3.0;
  LOG(INFO)   << "log stream check timestamp = '"
              << timeNowPrintf("%y%m%d %T") << "'";
  LOG(INFO)   << "log stream check filename = " << __FILE__;
  LOG(INFO)   << "log stream check line = " << __LINE__;
}

TEST(Logging, Check) {
  CHECK(true);
}

int main(int argc, char **argv) {
  logging::initialize(logging::Level::FATAL);
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
