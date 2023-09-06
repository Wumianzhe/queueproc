#ifndef MESSAGE_H
#define MESSAGE_H
#include <string>
#include <vector>
#include <ostream>
#include <chrono>

struct message {
    int id;
    std::chrono::time_point<std::chrono::local_time<std::chrono::minutes>> time;
    std::string body;
};

message parse(std::string& line);
void print(std::ostream os, message m);
#endif
