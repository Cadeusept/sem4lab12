// Copyright 2021 Your Name <your_email>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstring>
#include <example.hpp>
#include <stdexcept>
#include <thread>

using ::testing::AtLeast;

class MockStatSender : public StatSender {
 public:
  MOCK_METHOD2(AsyncSend, void(const std::vector<Item>&, std::string_view));
};

TEST(PageContainer, InvalidFile_1){
  PageContainer page{};
  std::ifstream in;
  try{
    page.RawLoad(in);
  } catch (std::exception& e) {
    EXPECT_TRUE(std::strcmp(e.what(), "file doesn`t open"));
  }
}

TEST(PageContainer, InvalidFile_2){
  PageContainer page{};
  std::ifstream in("");
  try{
    page.RawLoad(in);
  } catch (std::exception& e) {
    EXPECT_TRUE(std::strcmp(e.what(), "file is empty"));
  }
}

TEST(PageContainer, InvalidFile_3){
  PageContainer page{};
  std::ifstream in("ABOBA");
  EXPECT_THROW(page.RawLoad(in),std::runtime_error);
}

TEST(PageContainer, BadFileFormat){
  PageContainer page{};
  std::stringstream file;
  file << "0 name 10\n1 name 3\n2 name 4\n3 name 3\n4 name 10\n5 name 7\n"
          "6 name 4\n7 name 4\n8 name 5\n9 name 18\n10 name  12\n";
  page.RawLoad(file);
  EXPECT_EQ(10, page.GetRawDataSize());
}

TEST(PageContainer, RightFileFormat){
  PageContainer page{};
  std::stringstream file;
  file << "0 name 10\n1 name 3\n2 name 4\n3 name 3\n4 name 10\n5 name 7\n"
          "6 name 4\n7 name 4\n8 name 5\n9 name 18\n10 name 12\n";
  page.RawLoad(file);
  EXPECT_EQ(11, page.GetRawDataSize());
}

