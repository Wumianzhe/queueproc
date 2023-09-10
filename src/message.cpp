#include "message.h"
#include <chrono>
#include <format>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <regex> // it's in std and described format is simple
#include <string>

MessageAny::MessageAny(int id, std::chrono::minutes time, std::string body):time{time},id{id},body{body} {
}
MessageAny::MessageAny(const std::string& line) {
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

std::string MessageAny::toString() {
    return std::format("{:%R} {} {}\n",time,id,body);
}

std::string MessageArrived::toString() {
    return std::format("{:%R} 1 {}\n",time,client);
}

std::string MessageSatDown::toString() {
    return std::format("{:%R} 2 {} {}\n",time,client,tableNum);
}

std::string MessageWaiting::toString() {
    return std::format("{:%R} 3 {}\n",time,client);
}

std::string MessageLeft::toString() {
    return std::format("{:%R} 4 {}\n",time,client);
}

MessageArrived::MessageArrived(MessageAny m) {
  if (!std::all_of(m.body.cbegin(), m.body.cend(), [](const unsigned char c) {
        return isalnum(c) || (c == '_');
      })) {
    throw std::invalid_argument(
        m.toString()); // what is expected to be a client name, is not
  }
  time = m.time;
  client = m.body;
}
std::tuple<std::string, int> Controller::parseSitDown(Message m) {
  auto delPos = m.body.find(' ');
  if (std::string::npos == delPos || m.body.rfind(' ') != delPos) {
    throw std::invalid_argument(m.toString()); // body is not two "words"
  }
  auto client = m.body.substr(0, delPos);
  auto table = m.body.substr(0, delPos);
  int tableNum;
  try {
    tableNum = stoi(table);
  } catch (std::invalid_argument const &ex) {
    throw std::invalid_argument(m.toString()); // second part is not a number
  }
  if (tableNum > tCount) {
    throw std::invalid_argument(m.toString()); // table number is out of range
  }
  if (!std::all_of(m.body.cbegin(), m.body.cend(), [](const unsigned char c) {
        return isalnum(c) || (c == '_');
      })) {
    throw std::invalid_argument(
        m.toString()); // what is expected to be a client name, is not
  }
  return {client, tableNum};
}
std::string Controller::parseWait(Message m) {
  if (!std::all_of(m.body.cbegin(), m.body.cend(), [](const unsigned char c) {
        return isalnum(c) || (c == '_');
      })) {
    throw std::invalid_argument(
        m.toString()); // what is expected to be a client name, is not
  }
  return m.body;
}
std::string Controller::parseLeft(Message m) {
  if (!std::all_of(m.body.cbegin(), m.body.cend(), [](const unsigned char c) {
        return isalnum(c) || (c == '_');
      })) {
    throw std::invalid_argument(
        m.toString()); // what is expected to be a client name, is not
  }
  return m.body;
}
