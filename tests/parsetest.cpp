#include "doctest.h"
#include "message.h"
#include <stdexcept>

TEST_CASE("Message parsing") {
    SUBCASE("Regex mismatch throws") {
      CHECK_THROWS_AS(parse("08:48_1_client1"), std::invalid_argument);
      CHECK_THROWS_AS(parse("8:48 1 client1"), std::invalid_argument);
      CHECK_THROWS_AS(parse("08:48 11 client1"), std::invalid_argument);
      CHECK_THROWS_AS(parse("8:48 5 client1"), std::invalid_argument);
    }

    SUBCASE ("Wrong time throws") {
      CHECK_THROWS_AS(parse("25:48 1 client1"), std::invalid_argument);
      CHECK_THROWS_AS(parse("08:75 1 client1"), std::invalid_argument);
    }

    using namespace std::chrono_literals;
    CHECK_EQ(parse("08:48 1 client1"),Message{1,8h + 48min,"client1"});
    CHECK_EQ(parse("09:54 2 client1 1"),Message{2,9h + 54min,"client1 1"});
}
