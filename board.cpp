#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>
#include "board.hpp"
using namespace std;
#define ALL(c) (c).begin(),(c).end()

namespace im {

vector<room> load_rooms() {
    vector<room> result;

    const int BOTTOM = room::HEIGHT - board::CORRIDOR_HEIGHT;
    int i = 1;
    while (result.size() < 36) {
        if ((i&15) == 0) {
            ++i;
            continue;
        }
        room r;
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

board board::generate() {
    vector<elevator> elevators;
    const int COLUMNS = 6;
    vector<vector<room> > rooms(COLUMNS);

    for (int i = 0; i < 5; ++i)
        elevators.push_back(elevator());

    vector<room> available_rooms = load_rooms();
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

    return board(elevators, rooms);
}

board::board(vector<elevator> const& elevators, vector<vector<room> > const& rooms)
    : elevators(elevators)
    , rooms(rooms)
{
}

}
