// Copyright 2021 Your Name <your_email>

#ifndef INCLUDE_EXAMPLE_HPP_
#define INCLUDE_EXAMPLE_HPP_

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <string_view>

class Log { //singleton
 public:
  static Log& GetInstance();

  void Setting(bool level);

  void Write(const std::string_view& message) const;

  void WriteDebug(const std::string_view& message) const;

 private:
  Log(): level_(false), out_(&std::cout){};

  Log( const Log&) = delete;
  Log& operator=( Log& ) = delete;

  bool level_ = false;
  mutable std::ostream* out_;
};

struct Item {
  std::string id;
  std::string name;
  float score = 0;
};





class UsedMemory { //observer of PageContainer
 public:

  void OnDataLoad(const std::vector<Item>& old_items,
                  const std::vector<Item>& new_items);

  void OnRawDataLoad(const std::vector<std::string>& old_items,
                     const std::vector<std::string>& new_items);

  [[nodiscard]] size_t Used() const;

 private:
  size_t used_ = 0;
};





class StatSender { //observer of PageContainer
 public:

  void OnLoaded(const std::vector<Item>& new_items);

  void Skip(const Item& item);

  virtual ~StatSender() = default;

 private:  //made virtual
  virtual void AsyncSend(const std::vector<Item>& items, std::string_view path);

  std::ofstream fstr{"network", std::ios::binary};
};





class Histogram{ //observer of PageContainer
 public:
  static Histogram& GetInstance();

  [[nodiscard]] int GetNumSkip() const;

  [[nodiscard]] float GetAvg() const;

  void SetAvg(const float& avg_);

  void PlusNumSkip();

  void NewLap();
 private:
  Histogram() = default;
  Histogram( const Histogram&) = delete;
  Histogram& operator=( Histogram& ) = delete;

  int num_skip = 0;
  float avg = 0;
};




constexpr size_t kMinLines = 10;

class PageContainer { //observant
 public:
  void RawLoad(std::istream& file);

  [[nodiscard]] const Item& ByIndex(const size_t& i) const;

  [[nodiscard]] const Item& ById(const std::string& id) const;

  [[nodiscard]] size_t GetRawDataSize() const;

  [[nodiscard]] size_t GetDataSize() const;

  void DataLoad(const float& threshold);

  static bool IsCorrect(std::string& line);

  void PrintTable() const;

  explicit PageContainer(UsedMemory* memory_counter = new UsedMemory(),
                         StatSender* stat_sender = new StatSender())
      : memory_counter_(memory_counter), stat_sender_(stat_sender){}

  ~PageContainer();
 private:
  UsedMemory* memory_counter_;
  StatSender* stat_sender_;
  std::vector<Item> data_;
  std::vector<std::string> raw_data_;
};

#endif // INCLUDE_EXAMPLE_HPP_
