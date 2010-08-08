#include <algorithm>
#define BOOST_TEST_MODULE board_test
#include <boost/test/unit_test.hpp>

#include "board.hpp"
using namespace std;
using namespace im;

BOOST_AUTO_TEST_CASE(board_contains_five_elevators) {
    board b = board::generate();

    BOOST_CHECK_EQUAL(5, b.elevators.size());
}

