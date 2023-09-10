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

  void processArrival(Min time, std::string client);
  void processSitDown(Min time, std::string client, int tableNum);
  void processWait(Min time, std::string client);
  void processLeft(Min time, std::string client);
  std::string parseArrival(Message m);
  std::tuple<std::string, int> parseSitDown(Message m);
  std::string parseWait(Message m);
  std::string parseLeft(Message m);

public:
  Controller(int tableCount, Min openTime, Min closeTime, int hourRate);
  void queueMessage(Message m);
  void processQueue();
  void printOpen(std::ostream &os = std::cout);
  void printOut(std::ostream &os = std::cout);
  void printClose(std::ostream &os = std::cout);
};

#endif
