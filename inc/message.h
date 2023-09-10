#ifndef MESSAGE_H_
#define MESSAGE_H_
#include <chrono>
#include <string>
#include <ostream>

struct Message {
    int id;
    std::chrono::minutes time;
    std::string body;

    Message(const int id,const std::chrono::minutes& time,const std::string& body);
    Message(const std::string& line);
    Message(const Message&) = default;
    bool operator==(const Message &other) const {
      return (other.id == id) && (other.time == time) && (other.body == body);
    }
    virtual std::string toString();
};

struct MessageArrived : public Message {
    MessageArrived(const Message& m);
};
struct MessageSatDown : public Message {
    MessageSatDown(const Message& m);
};
struct MessageWaiting : public Message {
    MessageWaiting(const Message& m);
};
struct MessageLeft : public Message {
    MessageLeft(const Message& m);
};
#endif
