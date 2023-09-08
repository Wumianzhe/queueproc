#ifndef MESSAGE_H_
#define MESSAGE_H_
#include <chrono>
#include <string>
#include <ostream>

struct message {
    int id;
    std::chrono::minutes time;
    std::string body;

    std::string toString();
    bool operator==(const message& other) const {
        return (other.id == id) && (other.time == time) && (other.body == body);
    }
};

message parse(std::string line);
#endif
