#define BOOST_TEST_MODULE BoardTest
#include <boost/test/unit_test.hpp>

#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
#include <utility>

#include "Board.hpp"
using namespace std;
using namespace im;

struct Visitor {
    int* count;
    Visitor(int* count) : count(count) {}

    template<typename T>
    void operator () (T const& t) const {
        ++*count;
    }
};
BOOST_AUTO_TEST_CASE(invokes_visitor_correct_number_of_times) {
    Board b;

    int count1 = 0, count2 = 0;
    b.visit(Visitor(&count1));
    b.push_back(Background(Rect(),0));
    b.visit(Visitor(&count2));
    BOOST_CHECK_EQUAL(count2, count1+1);
}

