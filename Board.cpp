#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>
#include "Board.hpp"
using namespace std;
#define ALL(c) (c).begin(),(c).end()

namespace im {

Cell const& Board::cell(int i, int j) const {
    assert(i>=0);
    assert(i<HEIGHT);
    assert(j>=0);
    assert(j<WIDTH);
    return cells_[i][j];
}

Cell& Board::cell(int i, int j) {
    assert(i>=0);
    assert(i<HEIGHT);
    assert(j>=0);
    assert(j<WIDTH);
    return cells_[i][j];
}

Elevator const& Board::elevator(int n) const {
    assert(n>=0);
    assert(n<ELEVATOR_COUNT);
    return elevators_[n];
}

Elevator& Board::elevator(int n) {
    assert(n>=0);
    assert(n<ELEVATOR_COUNT);
    return elevators_[n];
}

vector<Room> load_rooms() {
    vector<Room> result;

    const int BOTTOM = Room::HEIGHT - Board::CORRIDOR_HEIGHT;
    int i = 1;
    while (result.size() < 36) {
        if ((i&15) == 0) {
            ++i;
            continue;
        }
        Room r;
        if (i&1)
            r.left_exits.push_back(0);
        if (i&2)
            r.left_exits.push_back(BOTTOM);
        if (i&4)
            r.right_exits.push_back(0);
        if (i&8)
            r.right_exits.push_back(BOTTOM);
        result.push_back(r);
        ++i;
    }

    return result;
}

Board Board::generate() {
    vector<Elevator> elevators;
    const int COLUMNS = 6;
    vector<vector<Room> > rooms(COLUMNS);

    for (int i = 0; i < 5; ++i)
        elevators.push_back(Elevator());

    vector<Room> available_rooms = load_rooms();
    random_shuffle(ALL(available_rooms));
    vector<char> used(available_rooms.size(),false);

    // Pick one room for each column between shafts to ensure connectedness
    for (int j = 1; j <= 4; ++j)
        for (int i = 0; i < available_rooms.size(); ++i)
            if (!available_rooms[i].left_exits.empty() &&
                !available_rooms[i].right_exits.empty() &&
                !used[i])
            {
                rooms[j].push_back(available_rooms[i]);
                used[i]=true;
                break;
            }

    // Fill remaining rooms
    const int ROOMS_PER_COLUMN = available_rooms.size()/COLUMNS;
    assert((available_rooms.size()%COLUMNS) == 0);

    {
        int taken = 0;
        for (int i = 0; taken < ROOMS_PER_COLUMN && i < available_rooms.size(); ++i) {
            if (used[i]) continue;
            if (!available_rooms[i].left_exits.empty()) continue;
            used[i]=true;
            rooms.front().push_back(available_rooms[i]);
            ++taken;
        }
    }

    {
        int taken = 0;
        for (int i = 0; taken < ROOMS_PER_COLUMN && i < available_rooms.size(); ++i) {
            if (used[i]) continue;
            if (!available_rooms[i].right_exits.empty()) continue;
            used[i]=true;
            rooms.back().push_back(available_rooms[i]);
            ++taken;
        }
    }

    return Board(rooms);
}

Board::Board(vector<vector<Room> > const& rooms)
    : rooms(rooms)
{
    for (int i = 0; i < HEIGHT; ++i)
        for (int j = 0; j < WIDTH; ++j) {
            cells_[i][j].position().upper_left() = Point(j*CELL_WIDTH, i*CELL_HEIGHT);
            cells_[i][j].position().size() = Size(CELL_WIDTH, CELL_HEIGHT);
        }
}

}
