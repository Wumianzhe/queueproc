#ifndef MESSAGE_H_
#define MESSAGE_H_
#include <chrono>
#include <ostream>
#include <string>

struct Message {
  int id;
  std::chrono::minutes time;
  std::string body;

  std::string toString();
  bool operator==(const Message &other) const {
    return (other.id == id) && (other.time == time) && (other.body == body);
  }
};

Message parse(const std::string &line);
#endif
