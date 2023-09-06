#ifndef MESSAGE_H
#define MESSAGE_H
#include <chrono>
#include <string>
#include <ostream>

struct message {
    int id;
    std::chrono::minutes time;
    std::string body;

    void print(std::ostream& os);
};

message parse(std::string& line);
#endif
