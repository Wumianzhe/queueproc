#include "table.h"

int table::getBillableHours() { return bill_hours; }

void table::endJob(std::chrono::minutes curr_time) {
  full = false;
  utilization += curr_time - full_since;
  bill_hours += std::chrono::ceil<std::chrono::hours>(curr_time - full_since).count();
}

std::chrono::minutes table::getUtilization() { return utilization; }

void table::startJob(std::chrono::minutes curr_time) {
  full = true;
  full_since = curr_time;
}

bool table::isFull() { return full; }
