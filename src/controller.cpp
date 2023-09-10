#include "controller.h"
#include "message.h"
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <string>

Controller::Controller(int tableCount, Min openTime, Min closeTime,
                       int hourRate)
    : tables(tableCount), tableUsers(tableCount, ""), tCount(tableCount),
      openTime(openTime), closeTime(closeTime) {}

void Controller::processQueue() {
  while (!inQueue.empty()) {
    Message &m = inQueue.front();
    outQueue.push(m);
    processMessage(m);
  }
}

void Controller::processMessage(const MessageArrived &m) {
  if (std::find(clientsInside.cbegin(), clientsInside.cend(), m.client) !=
      clientsInside.cend()) {
    outQueue.push(MessageAny{13, m.time, "YouShallNotPass"});
    return;
  }
  if (m.time < openTime) {
    outQueue.push(MessageAny{13, m.time, "NotOpenYet"});
    return;
  }
  clientsInside.emplace_back(m.client);
}
void Controller::processMessage(const MessageSatDown &m) {
  if (std::find(clientsInside.cbegin(), clientsInside.cend(), m.client) ==
      clientsInside.cend()) {
    outQueue.push(MessageAny{13, m.time, "ClientUnknown"});
    return;
  }
  if (tables[m.tableNum - 1].isFull()) {
    outQueue.push(MessageAny{13, m.time, "PlaceIsBusy"});
    return;
  }
  if (int i = std::distance(
          tableUsers.cbegin(),
          std::find(tableUsers.cbegin(), tableUsers.cend(), m.client))) {
    tables[i].endJob(m.time);
    tableUsers[i] = "";
  }
  tables[m.tableNum - 1].startJob(m.time);
  tableUsers[m.tableNum - 1] = m.client;
}
void Controller::processMessage(const MessageWaiting &m) {
  if (std::find(clientsInside.cbegin(), clientsInside.cend(), m.client) ==
      clientsInside.cend()) {
    outQueue.push(MessageAny{13, m.time, "ClientUnknown"});
    return;
  }
  // empty table found
  if (std::find(tableUsers.cbegin(), tableUsers.cend(), "") !=
      tableUsers.cend()) {
    outQueue.push(MessageAny{13, m.time, "ICanWaitNoLonger!"});
    return;
  }
  // too many people
  if (userQueue.size() > tCount) {
    outQueue.push(MessageAny{11, m.time, m.client});
    clientsInside.erase(
        std::find(clientsInside.cbegin(), clientsInside.cend(), m.client));
    return;
  }
  userQueue.push(m.client);
}
void Controller::processMessage(const MessageLeft &m) {
  if (std::find(clientsInside.cbegin(), clientsInside.cend(), m.client) ==
      clientsInside.cend()) {
    outQueue.push(MessageAny{13, m.time, "ClientUnknown"});
    return;
  }
  clientsInside.erase(
      std::find(clientsInside.cbegin(), clientsInside.cend(), m.client));
  if (int i = std::distance(
          tableUsers.cbegin(),
          std::find(tableUsers.cbegin(), tableUsers.cend(), m.client))) {
    tables[i].endJob(m.time);
    if (userQueue.empty()) {
      tableUsers[i] = "";
    } else {
      auto nextClient = userQueue.front();
      userQueue.pop();
      outQueue.push(MessageAny{12, m.time, nextClient});
      tables[i].startJob(m.time);
      tableUsers[i] = nextClient;
    }
  }
}

void Controller::queueMessage(const Message&& m) { inQueue.push(m); }
void Controller::printOpen(std::ostream &os) {
  os << std::format("{:%R}\n", openTime);
}
void Controller::printOut(std::ostream &os) {
  while (!outQueue.empty()) {
    os << outQueue.front().toString() << "\n";
    outQueue.pop();
  }
}
void Controller::printClose(std::ostream &os) {
  for (const auto &client : clientsInside) {
    outQueue.push(MessageAny{11, closeTime, client});
    auto pos = std::find(tableUsers.cbegin(), tableUsers.cend(), client);
    if (pos != tableUsers.cend()) {
      int i = std::distance(tableUsers.cbegin(), pos);
      tables[i].endJob(closeTime);
    }
  }
  os << std::format("{:%R}\n", closeTime);
  // don't know of an easy way to get pairs (index,element) in C++
  for (int i = 0; i < tCount; i++) {
    Table t = tables[i];
    os << format("{} {} {:%R}\n", i + 1, t.getBillableHours() * hourRate,
                 t.getUtilization());
  }
}
