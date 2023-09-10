#include "controller.h"
#include <algorithm>
#include <format>
#include <iomanip>
#include <iterator>
#include <stdexcept>
#include <string>

Controller::Controller(int tableCount, Min openTime, Min closeTime,
                       int hourRate)
    : tables(tableCount), tableUsers(tableCount, ""), tCount(tableCount),
      openTime(openTime), closeTime(closeTime), hourRate(hourRate) {}

void Controller::processQueue() {
  while (!inQueue.empty()) {
    auto m = inQueue.front();
    inQueue.pop();
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
      processLeft(m.time, client);
      break;
    }
    default: { // shouldn't happen, but still good to have
      throw std::invalid_argument(m.toString());
    }
    }
  }
}

void Controller::processArrival(Min time, std::string client) {
  if (std::find(clientsInside.cbegin(), clientsInside.cend(), client) !=
      clientsInside.cend()) {
    outQueue.push({13, time, "YouShallNotPass"});
    return;
  }
  if (time < openTime) {
    outQueue.push({13, time, "NotOpenYet"});
    return;
  }
  clientsInside.emplace_back(client);
}
void Controller::processSitDown(Min time, std::string client, int tableNum) {
  if (std::find(clientsInside.cbegin(), clientsInside.cend(), client) ==
      clientsInside.cend()) {
    outQueue.push({13, time, "ClientUnknown"});
    return;
  }
  if (tables[tableNum - 1].isFull()) {
    outQueue.push({13, time, "PlaceIsBusy"});
    return;
  }
  auto pos = std::find(tableUsers.cbegin(), tableUsers.cend(), client);
  if (pos != tableUsers.cend()) {
    auto i = std::distance(tableUsers.cbegin(), pos);
    tables[i].endJob(time);
    tableUsers[i] = "";
  }
  tables[tableNum - 1].startJob(time);
  tableUsers[tableNum - 1] = client;
}
void Controller::processWait(Min time, std::string client) {
  if (std::find(clientsInside.cbegin(), clientsInside.cend(), client) ==
      clientsInside.cend()) {
    outQueue.push({13, time, "ClientUnknown"});
    return;
  }
  // empty table found
  if (std::find(tableUsers.cbegin(), tableUsers.cend(), "") !=
      tableUsers.cend()) {
    outQueue.push({13, time, "ICanWaitNoLonger!"});
    return;
  }
  // too many people
  if (userQueue.size() > tCount) {
    outQueue.push({11, time, client});
    clientsInside.erase(
        std::find(clientsInside.cbegin(), clientsInside.cend(), client));
    return;
  }
  userQueue.push(client);
}
void Controller::processLeft(Min time, std::string client) {
  if (std::find(clientsInside.cbegin(), clientsInside.cend(), client) ==
      clientsInside.cend()) {
    outQueue.push({13, time, "ClientUnknown"});
    return;
  }
  clientsInside.erase(
      std::find(clientsInside.cbegin(), clientsInside.cend(), client));
  auto pos = std::find(tableUsers.cbegin(), tableUsers.cend(), client);
  if (pos != tableUsers.cend()) {
    auto i = std::distance(tableUsers.cbegin(), pos);
    tables[i].endJob(time);
    if (userQueue.empty()) {
      tableUsers[i] = "";
    } else {
      auto nextClient = userQueue.front();
      userQueue.pop();
      outQueue.push({12, time, std::format("{} {}", nextClient, i + 1)});
      tables[i].startJob(time);
      tableUsers[i] = nextClient;
    }
  }
}

std::string Controller::parseArrival(Message m) {
  if (!std::all_of(m.body.cbegin(), m.body.cend(), [](const unsigned char c) {
        return isalnum(c) || (c == '_');
      })) {
    throw std::invalid_argument(
        m.toString()); // what is expected to be a client name, is not
  }
  return m.body;
}
std::tuple<std::string, int> Controller::parseSitDown(Message m) {
  auto delPos = m.body.find(' ');
  if (std::string::npos == delPos || m.body.rfind(' ') != delPos) {
    throw std::invalid_argument(m.toString()); // body is not two "words"
  }
  auto client = m.body.substr(0, delPos);
  auto table = m.body.substr(delPos + 1);
  int tableNum;
  try {
    tableNum = stoi(table);
  } catch (std::invalid_argument const &ex) {
    throw std::invalid_argument(m.toString()); // second part is not a number
  }
  if (tableNum > tCount) {
    throw std::invalid_argument(m.toString()); // table number is out of range
  }
  if (!std::all_of(client.cbegin(), client.cend(), [](const unsigned char c) {
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

void Controller::queueMessage(Message m) { inQueue.push(m); }
void Controller::printOpen(std::ostream &os) {
  os << std::format("{:%R}\n", openTime);
}
void Controller::printOut(std::ostream &os) {
  while (!outQueue.empty()) {
    os << outQueue.front().toString();
    outQueue.pop();
  }
}
void Controller::printClose(std::ostream &os) {
  for (const auto &client : clientsInside) {
    outQueue.push({11, closeTime, client});
    auto pos = std::find(tableUsers.cbegin(), tableUsers.cend(), client);
    if (pos != tableUsers.cend()) {
      int i = std::distance(tableUsers.cbegin(), pos);
      tables[i].endJob(closeTime);
    }
  }
  os << std::format("{:%R}\n", closeTime);
  // don't know of an easy way to get pairs (index,element) in C++
  for (int i = 0; i < tCount; i++) {
    Table &t = tables[i];
    os << format("{} {} {:%R}\n", i + 1, t.getBillableHours() * hourRate,
                 t.getUtilization());
  }
}
