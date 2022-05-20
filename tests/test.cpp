// Copyright 2021 Your Name <your_email>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstring>
#include <example.hpp>
#include <stdexcept>
#include <thread>

using ::testing::AtLeast;

class MockLog : public Log {
 public:
  MOCK_METHOD(void, Write, (std::string_view message), (const));
  MOCK_METHOD(void, WriteDebug, (std::string_view message), (const));
  //MOCK_METHOD(Log, (size_t level), (explicit));
};

class MockUsedMemory : public UsedMemory {
 public:
  MOCK_METHOD(void, OnDataLoad,
              (const std::vector<Item>& old_items,
               const std::vector<Item>& new_items));

  MOCK_METHOD(void, OnRawDataLoad,
              (const std::vector<std::string>& old_items,
               const std::vector<std::string>& new_items));

  MOCK_METHOD(size_t, used, (), (const));
};


TEST(Example, EmptyTest) {
    EXPECT_NO_THROW(Log{1});
}

/**TEST(DISABLED_Snapshot, Speen) {
    for (;;) std::this_thread::yield();
}**/

TEST(PageContainer, Bad_file) {
  MockLog log(2);
  MockUsedMemory* memory_counter;

  PageContainer test_PageContainer(log, memory_counter);
    std::ifstream in;
    try{
      test_PageContainer.RawLoad(in);
    } catch (std::exception& e) {
      EXPECT_TRUE(std::strcmp(e.what(), "file don`t open"));
    }
}

TEST(PageContainer, modification) {
    MockLog log(2);
    MockUsedMemory* memory_counter;

    PageContainer test_PageContainer(log, memory_counter);

    std::stringstream ss;
    ss << "aboba";
    PageContainer tmp(log, memory_counter);
    EXPECT_ANY_THROW(test_PageContainer.Load(ss, 2));

    EXPECT_EQ(tmp, test_PageContainer);
}
