#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "message.h"
#include "table.h"
#include <chrono>
#include <iostream>
#include <ostream>
#include <queue>

class controller {
  using Min = std::chrono::minutes;
  controller(int tableCount, Min openTime, Min closeTime, int hourRate);
  std::queue<message> inQueue;
  std::queue<message> outQueue;

  std::vector<table> tables;
  std::vector<std::string> users;
  std::queue<std::string> userQueue;

  int tCount;
  Min openTime;
  Min closeTime;
    int hourRate;

  void processArrival(Min time, std::string client);
  void processSitDown(Min time, std::string client, int tableNum);
  void processWait(Min time, std::string client);
  void processLeft(Min time, std::string client);

public:
  void queueMessage(message m);
  void processQueue();
  void printOpen(std::ostream &os = std::cout);
  void printOut(std::ostream &os = std::cout);
  void printFooter(std::ostream &os = std::cout);
};

#endif
