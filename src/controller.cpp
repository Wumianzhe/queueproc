#include "controller.h"
#include <algorithm>
#include <stdexcept>
#include <string>

controller::controller(int tableCount, Min openTime, Min closeTime,
                       int hourRate)
    : tables(3), users(3), tCount(tableCount), openTime(openTime),
      closeTime(closeTime) {}

void controller::processQueue() {
  while (!inQueue.empty()) {
    auto m = inQueue.front();
    outQueue.push(m);
    switch (m.id) {
    case 1: {
      auto client = parseArrival(m);
      processArrival(m.time, client);
      break;
    }
    case 2: {
      auto [client, tableNum] = parseSitDown(m);
      processSitDown(m.time, client, tableNum);
      break;
    }
    case 3: {
      auto client = parseWait(m);
      processWait(m.time, client);
      break;
    }
    case 4: {
      auto client = parseLeft(m);
      processLeft(m.time,client);
      break;
    }
    default: { // shouldn't happen, but still good to have
      throw std::invalid_argument(m.str());
    }
    }
  }
}
void controller::processArrival(Min time, std::string client) {}
void controller::processSitDown(Min time, std::string client, int tableNum) {}
void controller::processWait(Min time, std::string client) {}
void controller::processLeft(Min time, std::string client) {}

std::string controller::parseArrival(message m) {
  if (!std::all_of(m.body.cbegin(), m.body.cend(), [](const unsigned char c) {
        return isalnum(c) || (c == '_');
      })) {
    throw std::invalid_argument(
        m.str()); // what is expected to be a client name, is not
  }
  return m.body;
}
std::tuple<std::string, int> controller::parseSitDown(message m) {
  auto delPos = m.body.find(' ');
  if (std::string::npos == delPos || m.body.rfind(' ') != delPos) {
    throw std::invalid_argument(m.str()); // body is not two "words"
  }
  auto client = m.body.substr(0, delPos);
  auto table = m.body.substr(0, delPos);
  int tableNum;
  try {
    tableNum = stoi(table);
  } catch (std::invalid_argument const &ex) {
    throw std::invalid_argument(m.str()); // second part is not a number
  }
  if (tableNum > tCount) {
    throw std::invalid_argument(m.str()); // table number is out of range
  }
  if (!std::all_of(m.body.cbegin(), m.body.cend(), [](const unsigned char c) {
        return isalnum(c) || (c == '_');
      })) {
    throw std::invalid_argument(
        m.str()); // what is expected to be a client name, is not
  }
  return {client, tableNum};
}
std::string controller::parseWait(message m) {
  if (!std::all_of(m.body.cbegin(), m.body.cend(), [](const unsigned char c) {
        return isalnum(c) || (c == '_');
      })) {
    throw std::invalid_argument(
        m.str()); // what is expected to be a client name, is not
  }
  return m.body;
}
std::string controller::parseLeft(message m) {
  if (!std::all_of(m.body.cbegin(), m.body.cend(), [](const unsigned char c) {
        return isalnum(c) || (c == '_');
      })) {
    throw std::invalid_argument(
        m.str()); // what is expected to be a client name, is not
  }
  return m.body;
}

void controller::queueMessage(message m) { inQueue.push(m); }
void controller::printOpen(std::ostream &os) {
  os << std::format("{:%R}\n", openTime);
}
void controller::printOut(std::ostream &os) {
  while (!outQueue.empty()) {
    os << outQueue.front().str() << "\n";
    outQueue.pop();
  }
}
void controller::printFooter(std::ostream &os) {
  os << std::format("{:%R}\n", closeTime);
  // don't know of an easy way to get pairs (index,element) in C++
  for (int i = 0; i < tCount; i++) {
    table t = tables[i];
    os << format("{} {} {:%R}\n", i + 1, t.getBillableHours() * hourRate,
                 t.getUtilization());
  }
}
