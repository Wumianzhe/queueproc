#include "doctest.h"
#include "table.h"
#include <chrono>

TEST_CASE("Table time accounting") {
    Table t;
    using namespace std::chrono_literals;
    std::chrono::minutes startTime = 1h + 15min;
    std::chrono::minutes endTime = 1h + 20min;
    t.startJob(startTime);
    REQUIRE_EQ(t.getUtilization(),startTime - endTime);
    REQUIRE_EQ(t.getBillableHours(), 1);
}
