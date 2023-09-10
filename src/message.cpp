#include "message.h"
#include <chrono>
#include <format>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <regex> // it's in std and described format is simple
#include <string>

Message::Message(const int id, const std::chrono::minutes& time,const std::string& body):time{time},id{id},body{body} {
}
Message::Message(const std::string& line) {
    // requiring 2 digits means leading zeroes are necessary, incoming messages have id of 1-4
    // body is anything non-empty for now and will be looked at later as contents depends on id
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

    time = std::chrono::hours(tm_time.tm_hour) + std::chrono::minutes(tm_time.tm_min);
    id = std::stoi(sub_id);
    body=sub_body;
}

std::string Message::toString() {
    return std::format("{:%R} {} {}\n",time,id,body);
}

MessageArrived::MessageArrived(const Message& m):Message(m) {
  if (!std::all_of(m.body.cbegin(), m.body.cend(), [](const unsigned char c) {
        return isalnum(c) || (c == '_');
      })) {
    throw std::invalid_argument(
        m.toString()); // what is expected to be a client name, is not
  }
  time = m.time;
  client = m.body;
}
MessageSatDown::MessageSatDown(const Message& m) {
  auto delPos = m.body.find(' ');
  if (std::string::npos == delPos || m.body.rfind(' ') != delPos) {
    throw std::invalid_argument(m.toString()); // body is not two "words"
  }
  client = m.body.substr(0, delPos);
  auto table = m.body.substr(0, delPos);
  try {
    tableNum = stoi(table);
  } catch (std::invalid_argument const &ex) {
    throw std::invalid_argument(m.toString()); // second part is not a number
  }
  if (!std::all_of(m.body.cbegin(), m.body.cend(), [](const unsigned char c) {
        return isalnum(c) || (c == '_');
      })) {
    throw std::invalid_argument(
        m.toString()); // what is expected to be a client name, is not
  }
  time = m.time;
}
MessageWaiting::MessageWaiting(const Message& m) {
  if (!std::all_of(m.body.cbegin(), m.body.cend(), [](const unsigned char c) {
        return isalnum(c) || (c == '_');
      })) {
    throw std::invalid_argument(
        m.toString()); // what is expected to be a client name, is not
  }
  time = m.time;
  client = m.body;
}
MessageLeft::MessageLeft(const Message& m) {
  if (!std::all_of(m.body.cbegin(), m.body.cend(), [](const unsigned char c) {
        return isalnum(c) || (c == '_');
      })) {
    throw std::invalid_argument(
        m.toString()); // what is expected to be a client name, is not
  }
  time = m.time;
  client = m.body;
}
