#include <algorithm>
#define BOOST_TEST_MODULE board_test
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <queue>
#include <set>
#include <utility>

#include "board.hpp"
using namespace std;
using namespace im;

BOOST_AUTO_TEST_CASE(board_contains_five_elevators) {
    board b = board::generate();
    BOOST_CHECK_EQUAL(5, b.elevators.size());
}

BOOST_AUTO_TEST_CASE(board_contains_six_columns_of_rooms) {
    board b = board::generate();
    BOOST_CHECK_EQUAL(6, b.rooms.size());
}

BOOST_AUTO_TEST_CASE(generated_room_positions_have_no_off_board_exits) {
    for (int i = 0; i < 100; ++i) {
        board b = board::generate();
        for (int j = 0; j < b.rooms.front().size(); ++j) {
            BOOST_CHECK_EQUAL(0, b.rooms.front()[j].left_exits.size());
        }
        for (int j = 0; j < b.rooms.back().size(); ++j) {
            BOOST_CHECK_EQUAL(0, b.rooms.back()[j].right_exits.size());
        }
    }
}

BOOST_AUTO_TEST_CASE(all_columns_have_rooms) {
    for (int i = 0; i < 100; ++i) {
        board b = board::generate();
        for (int j = 0; j < b.rooms.size(); ++j) {
            BOOST_CHECK(b.rooms[j].size() > 0);
        }
    }
}

struct ConnectedChecker {
    enum QType { SHAFT, ROOM };
    queue<int> q;
    set<int> shafts_seen;
    set<pair<int,int> > rooms_seen;
    board const& b;

    ConnectedChecker(board const& b) : b(b) {}

    void visit_room(int j, int i) {
        BOOST_REQUIRE(j>=0);
        BOOST_REQUIRE(j<b.rooms.size());
        BOOST_REQUIRE(i>=0);
        BOOST_REQUIRE(i<b.rooms[j].size());
        pair<int,int> rid = make_pair(j,i);
        if (!rooms_seen.count(rid)) {
            rooms_seen.insert(rid);
            q.push(ROOM);
            q.push(rid.first);
            q.push(rid.second);
        }
    }

    void visit_shaft(int n) {
        BOOST_REQUIRE(n>=0);
        BOOST_REQUIRE(n<b.elevators.size());
        if (!shafts_seen.count(n)) {
            shafts_seen.insert(n);
            q.push(SHAFT);
            q.push(n);
        }
    }

    void operator () () {
        visit_room(0,0);
        while (!q.empty()) {
            const QType t = (QType) q.front(); q.pop();
            if (t == SHAFT) {
                const int shaft = q.front(); q.pop();

                // Rooms left of shaft
                for (int i = 0; i < b.rooms[shaft].size(); ++i) {
                    room const& r = b.rooms[shaft][i];
                    if (!r.right_exits.empty())
                        visit_room(shaft,i);
                }

                // Rooms right of shaft
                for (int i = 0; i < b.rooms[shaft+1].size(); ++i) {
                    room const& r = b.rooms[shaft+1][i];
                    if (!r.left_exits.empty())
                        visit_room(shaft+1,i);
                }
            } else {
                const int j = q.front(); q.pop();
                const int i = q.front(); q.pop();

                BOOST_REQUIRE(j < b.rooms.size());
                BOOST_REQUIRE(i < b.rooms[j].size());

                room const& r = b.rooms[j][i];
                if (!r.left_exits.empty()) {
                    visit_shaft(j-1);
                }
                if (!r.right_exits.empty()) {
                    BOOST_REQUIRE(j<b.elevators.size());
                    visit_shaft(j);
                }
            }
        }

        // Make sure we've seen all shafts
        for (int i = 0; i < b.elevators.size(); ++i) {
            BOOST_CHECK(shafts_seen.count(i));
        }

        // Make sure we've seen all rooms
        for (int j = 0; j < b.rooms.size(); ++j)
            for (int i = 0; i < b.rooms[j].size(); ++i) {
                BOOST_CHECK(rooms_seen.count(make_pair(j,i)));
            }
    }
};
BOOST_AUTO_TEST_CASE(all_rooms_and_shafts_are_connected) {
    for (int i = 0; i < 100; ++i) {
        board b = board::generate();
        ConnectedChecker checker(b);
        checker();
    }
}

