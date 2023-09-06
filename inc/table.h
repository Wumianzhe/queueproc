#ifndef TABLE_H_
#define TABLE_H_

#include <chrono>

class table {
    bool full;
    std::chrono::minutes full_since;

    int bill_hours;
    std::chrono::minutes utilization;
};

#endif // TABLE_H_
