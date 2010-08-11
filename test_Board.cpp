#define BOOST_TEST_MODULE board_test
#include <boost/test/unit_test.hpp>

#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
#include <utility>

#include "Board.hpp"
using namespace std;
using namespace im;

BOOST_AUTO_TEST_CASE(can_add_items_of_correct_types) {
    Board b = Board::generate();
    b.push_back(Background());
}

