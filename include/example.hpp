// Copyright 2021 Your Name <your_email>

#ifndef INCLUDE_EXAMPLE_HPP_
#define INCLUDE_EXAMPLE_HPP_

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <string_view>

constexpr size_t kMinLines = 10;

class Log { //singleton
 private:
  Log(): level_(false), out_(&std::cout){}

  Log( const Log&) = delete;
  Log& operator=( Log& ) = delete;

  bool level_ = false;
  mutable std::ostream* out_;
 public:
  static Log& GetInstance() {
    static Log instance;
    return instance;
  };

  void Setting(bool level) {
    level_ = level;
  };

  void Write(const std::string_view& message) const {
    *out_ << "[info] " << message << std::endl;
  };

  void WriteDebug(const std::string_view& message) const {
    if (level_) *out_ << "[debug] " << message << std::endl;
  };
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

  [[nodiscard]] size_t Used() const {return used_;};

 private:
  size_t used_ = 0;
};





class StatSender { //observer of PageContainer
 public:
  void OnLoaded(const std::vector<Item>& new_items) {
    Log::GetInstance().WriteDebug("StatSender::OnDataLoad");
    AsyncSend(new_items, "/items/loaded");
  };

  void Skip(const Item& item) {
    AsyncSend({item}, "/items/skiped");
  };

  virtual ~StatSender() = default;

 private:  //made virtual
  virtual void AsyncSend(const std::vector<Item>& items, std::string_view path);

  std::ofstream fstr{"network", std::ios::binary};
};





class Histogram{ //observer of PageContainer
 public:
  static Histogram& GetInstance() {static Histogram instance; return instance;};

  [[nodiscard]] int GetNumSkip() const {return num_skip;};

  [[nodiscard]] float GetAvg() const {return avg;};

  void SetAvg(const float& avg_) {avg = avg_;};

  void PlusNumSkip() {++num_skip;};

  void NewLap() {num_skip = 0;};
 private:
  Histogram() = default;
  Histogram( const Histogram&) = delete;
  Histogram& operator=( Histogram& ) = delete;

  int num_skip = 0;
  float avg = 0;
};





class PageContainer { //observant
 private:
  UsedMemory* memory_counter_;
  StatSender* stat_sender_;
  std::vector<Item> data_;
  std::vector<std::string> raw_data_;

 public:
  void RawLoad(std::istream& file);

  [[nodiscard]] const Item& ByIndex(const size_t& i) const {return data_[i];};

  [[nodiscard]] const Item& ById(const std::string& id) const;

  [[nodiscard]] size_t GetRawDataSize() const {return raw_data_.size();};

  [[nodiscard]] size_t GetDataSize() const {return data_.size();};

  void DataLoad(const float& threshold);

  static bool IsCorrect(std::string& line);

  void PrintTable() const;

  explicit PageContainer(UsedMemory* memory_counter = new UsedMemory(),
                         StatSender* stat_sender = new StatSender())
      : memory_counter_(memory_counter), stat_sender_(stat_sender){}

  ~PageContainer() {delete memory_counter_; delete stat_sender_;};
};

#endif // INCLUDE_EXAMPLE_HPP_
