#include "table.h"

int Table::getBillableHours() { return bill_hours; }

void Table::endJob(std::chrono::minutes curr_time) {
  full = false;
  utilization += curr_time - full_since;
  bill_hours += std::chrono::ceil<std::chrono::hours>(curr_time - full_since).count();
}

std::chrono::minutes Table::getUtilization() { return utilization; }

void Table::startJob(std::chrono::minutes curr_time) {
  full = true;
  full_since = curr_time;
}

bool Table::isFull() { return full; }
