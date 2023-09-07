#include "doctest.h"
#include "controller.h"
#include <sstream>

TEST_CASE("Controller tests") {
    using namespace std::chrono_literals;
    Controller c{3,9h,19h,10};
    std::ostringstream oss;
    c.printOpen(oss);
    CHECK_EQ(oss.str(),"09:00\n");
    oss.str("");

    SUBCASE("Parsing errors") {
    }
}
