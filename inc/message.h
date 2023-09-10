#ifndef MESSAGE_H_
#define MESSAGE_H_
#include <chrono>
#include <string>
#include <ostream>

struct Message {
    Message(){}

    virtual std::string toString() = 0;
};

struct MessageAny : public Message {
    int id;
    std::chrono::minutes time;
    std::string body;

    MessageAny(int id, std::chrono::minutes time, std::string body);
    MessageAny(const std::string& line);
    bool operator==(const MessageAny &other) const {
      return (other.id == id) && (other.time == time) && (other.body == body);
    }
    virtual std::string toString() override;
};

struct MessageArrived : public Message {
    MessageArrived(MessageAny m);
    std::chrono::minutes time;
    std::string client;
    bool operator==(const MessageArrived &other) const {
      return (other.time == time) && (other.client == client);
    }
    virtual std::string toString() override;
};
struct MessageSatDown : public Message {
    MessageSatDown(MessageAny m);
    std::chrono::minutes time;
    std::string client;
    int tableNum;
    bool operator==(const MessageSatDown &other) const {
      return (other.time == time) && (other.client == client) && (other.tableNum == tableNum);
    }
    virtual std::string toString() override;
};
struct MessageWaiting : public Message {
    MessageWaiting(MessageAny m);
    std::chrono::minutes time;
    std::string client;
    bool operator==(const MessageWaiting &other) const {
      return (other.time == time) && (other.client == client);
    }
    virtual std::string toString() override;
};
struct MessageLeft : public Message {
    MessageLeft(MessageAny m);
    std::chrono::minutes time;
    std::string client;
    bool operator==(const MessageLeft &other) const {
      return (other.time == time) && (other.client == client);
    }
    virtual std::string toString() override;
};
#endif
