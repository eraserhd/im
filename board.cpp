#include <algorithm>
#include <vector>
#include "board.hpp"
using namespace std;

namespace im {

board board::generate() {
    vector<elevator> elevators;

    for (int i = 0; i < 5; ++i)
        elevators.push_back(elevator());

    return board(elevators);
}

board::board(vector<elevator> const& elevators)
    : elevators(elevators)
{}

}
