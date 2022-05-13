// Copyright 2021 Your Name <your_email>

#include <gtest/gtest.h>

#include <refactorer.hpp>
#include <stdexcept>
#include <thread>

TEST(Example, EmptyTest) {
    EXPECT_NO_THROW(Log{1});
}

TEST(DISABLED_Snapshot, Speen) {
  for (;;) std::this_thread::yield();
}
