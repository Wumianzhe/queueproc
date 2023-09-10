#include <controller.h>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace std;

Controller readController(std::istream &is);

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cout << "Input file is required as a command line parameter";
    return 1;
  }
  string filename = argv[1];
  ifstream input(filename);
  if (!input.is_open()) {
    cerr << "File not found" << endl;
    return 1;
  }
  try {
    Controller c = readController(input);
    for (string line; getline(input, line);) {c.queueMessage(parse(line));
    }
    c.processQueue();
    c.printOpen();
    c.printOut();
    c.printClose();
  } catch (const exception &ex) {
    cerr << ex.what() << endl;
    return 1;
  }
  return 0;
}

Controller readController(std::istream &is) {
  int tCount;
  int hourPrice;
  tm tm_time;
  chrono::minutes startTime, endTime;
  // need to do that as I need to output back a line which doesn't match the
  // spec
  string line;
  istringstream iss;

  getline(is, line);
  iss.str(line);
  iss >> tCount;
  if (iss.fail()) {
    throw std::invalid_argument(line);
  }

  getline(is, line);
  auto spacePos = line.find(' ');
  iss.clear(); // no idea why, but it doesn't work without clear...
  iss.str(line.substr(0, spacePos));
  iss >> get_time(&tm_time, "%H:%M");
  if (iss.fail()) {
    throw std::invalid_argument(line);
  }
  startTime = chrono::hours(tm_time.tm_hour) + chrono::minutes(tm_time.tm_min);
  iss.clear(); // no idea why, but it doesn't work without clear...
  iss.str(line.substr(spacePos));
  iss >> get_time(&tm_time, "%H:%M");
  if (iss.fail()) {
    throw std::invalid_argument(line);
  }
  endTime = chrono::hours(tm_time.tm_hour) + chrono::minutes(tm_time.tm_min);

  getline(is, line);
  iss.str(line);
  iss.clear(); // no idea why, but it doesn't work without clear...
  iss >> hourPrice;
  if (iss.fail()) {
    throw std::invalid_argument(line);
  }
  return Controller(tCount, startTime, endTime, hourPrice);
}
