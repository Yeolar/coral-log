#
# Copyright (C) 2014, Yeolar <yeolar@gmail.com>
#

set(SRC_DIR "${CORAL_SOURCE_DIR}/coral")
set(DST_DIR "${CORAL_BINARY_DIR}/coral")
message(STATUS ${CORAL_SOURCE_DIR})

set(CORAL_SRCS
    Logging.cc
    StringUtil.cc
    Time.cc
)

add_library(coral ${CORAL_SRCS})

add_executable(LoggingTest test/LoggingTest.cc)
target_link_libraries(LoggingTest gtest pthread coral)

add_test(CoralLogging LoggingTest)
