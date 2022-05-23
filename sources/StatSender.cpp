//
// Created by cadeusept on 23.05.22.
//

void StatSender::AsyncSend(const std::vector<Item>& items,
                           std::string_view path) {
  Log::GetInstance().Write(path);
  Log::GetInstance().Write("send stat " + std::to_string(items.size()));

  for (const auto& item : items) {
    Log::GetInstance().WriteDebug("send: " + item.id);
    // ... some code
    fstr << item.id << item.name << item.score;
    fstr.flush();
  }
}

