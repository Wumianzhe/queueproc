#include "message.h"
#include <chrono>
#include <format>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <regex> // it's in std and described format is simple
#include <string>

message parse(std::string line) {
    // requiring 2 digits means leading zeroes are necessary, incoming messages have id of 1-4
    // body is anything non-empty for now and will be looked at during processing as contents depends on id
    const std::regex format_re("^([0-9]{2}:[0-9]{2})\\ ([1-4])\\ (.+)$");
    std::smatch submatches;
    if (!std::regex_match(line,submatches,format_re)) {
        throw std::invalid_argument{line.c_str()};
    }
    std::ssub_match sub_time = submatches[1];
    std::ssub_match sub_id = submatches[2];
    std::ssub_match sub_body = submatches[3];

    std::istringstream tss(sub_time);
    std::tm tm_time;
    tss >> std::get_time(&tm_time, "%H:%M");
    if (tss.fail()) {
        throw std::invalid_argument{line.c_str()};
    }
    std::chrono::minutes time = std::chrono::hours(tm_time.tm_hour) + std::chrono::minutes(tm_time.tm_min);

    int id = std::stoi(sub_id);
    return message{id,time,sub_body};
}

void message::print(std::ostream& os) {
    os << std::format("{:%R} {} {}\n",time,id,body);
}
