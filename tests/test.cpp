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

TEST(PageContainer, DataLoad){
  PageContainer page{};
  std::stringstream file;
  file << "0 name 10\n1 name 3\n2 name 4\n3 name 3\n4 name 10\n5 name 7\n"
          "6 name 4\n7 name 4\n8 name 5\n9 name 18\n10 name 12\n";
  page.RawLoad(file);
  page.DataLoad(0);
  EXPECT_EQ(11, page.GetDataSize());
}

TEST(PageContainer, FileError_Duplicate){
  PageContainer page{};
  std::stringstream file;
  file << "0 name 10\n1 name 3\n2 name 4\n3 name 3\n4 name 10\n5 name 7\n"
          "6 name 4\n1 name 4\n8 name 5\n9 name 18\n10 name 12\n";
  page.RawLoad(file);
  EXPECT_THROW(page.DataLoad(0), std::runtime_error);
}

TEST(PageContainer, FileError_SmallAmountOfData){
  PageContainer page{};
  std::stringstream file;
  file << "0 name 10\n1 name 3\n2 name 4\n3 name 3\n4 name 10\n5 name 7\n"
          "6 name 4\n7 name 4\n8 name 5\n9 name 18\n10 name 12\n";
  page.RawLoad(file);
  EXPECT_THROW(page.DataLoad(4), std::runtime_error);
}

TEST(StatSender, MultipleTest){
  using ::testing::_;
  using ::testing::AtLeast;
  UsedMemory* used = new UsedMemory();
  MockStatSender* sender = new MockStatSender();
  PageContainer page(used, sender);
  std::stringstream ss;
  ss << "0 name 10\n1 name 3\n2 name 4\n3 name 3\n4 name 10\n5 name 7\n"
        "6 name 4\n7 name 4\n8 name 5\n9 name 18\n10 name 12\n11 name 10\n"
        "12 name 17\n13 name 10\n14 name 11\n15 name 9\n";
  page.RawLoad(ss);
  EXPECT_CALL(*sender,
              AsyncSend(_, std::string_view{"/items/loaded"}))
      .Times(2);
  EXPECT_CALL(*sender,
              AsyncSend(_, std::string_view{"/items/skiped"}))
      .Times(5);
  page.DataLoad(1);
  page.DataLoad(4);
}

TEST(UsedMemory, IsCorrectCounter){
  std::vector<std::string> old_raw_data{ "line1" };
  std::vector<std::string> new_raw_data {"line1", "line2", "line3", "line4"};
  UsedMemory usedMemory1;
  usedMemory1.OnRawDataLoad(old_raw_data, new_raw_data);
  EXPECT_EQ(45,usedMemory1.Used());
  std::vector<Item> old_data{ {"0", "name", 0} };
  std::vector<Item> new_data {{"0", "name", 0},
                             {"1", "name", 0},
                             {"2", "name", 0}};
  UsedMemory usedMemory2;
  usedMemory2.OnDataLoad(old_data, new_data);
  EXPECT_EQ(68,usedMemory2.Used());
}
