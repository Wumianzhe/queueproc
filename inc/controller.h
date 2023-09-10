#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "message.h"
#include "table.h"
#include <chrono>
#include <iostream>
#include <ostream>
#include <queue>

class Controller {
  using Min = std::chrono::minutes;
  std::queue<Message> inQueue;
  std::queue<Message> outQueue;

  std::vector<Table> tables;
  std::vector<std::string> clientsInside;
  std::vector<std::string> tableUsers;
  std::queue<std::string> userQueue;

  int tCount;
  Min openTime;
  Min closeTime;
  int hourRate;

  void processMessage(const Message &m) {}
  void processMessage(const MessageArrived &m);
  void processMessage(const MessageSatDown &m);
  void processMessage(const MessageWaiting &m);
  void processMessage(const MessageLeft &m);

public:
  Controller(int tableCount, Min openTime, Min closeTime, int hourRate);
  void queueMessage(const Message&& m);
  void processQueue();
  void printOpen(std::ostream &os = std::cout);
  void printOut(std::ostream &os = std::cout);
  void printClose(std::ostream &os = std::cout);
};

#endif
