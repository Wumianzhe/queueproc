#include "message.h"
#include <format>

message parse(std::string& line) {
    std::chrono::time_point<std::chrono::local_time<std::chrono::minutes>> time;
    auto t = std::chrono::parse("%R",time);
    return {13,time,{"Nothing yet"}};
}

void print(std::ostream os, message m) {
    os << std::format("{} {}",m.id,m.body);
}
