#ifndef TABLE_H_
#define TABLE_H_

#include <chrono>
#include <string>

class table {
  bool full = false;
  std::chrono::minutes full_since;

  int bill_hours = 0;
  std::chrono::minutes utilization{0};

public:
  int getBillableHours();
  std::chrono::minutes getUtilization();
  bool isFull();
  // naming things is hard
  void startJob(std::chrono::minutes curr_time);
  void endJob(std::chrono::minutes curr_time);
};

#endif // TABLE_H_
