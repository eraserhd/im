#ifndef board_hpp_INCLUDED
#define board_hpp_INCLUDED

#include <algorithm>
#include <vector>
#include "elevator.hpp"

namespace im {

class board {
public:
    std::vector<elevator> elevators;

    static board generate();

private:
    board(std::vector<elevator> const& elevators);

};

}

#endif // ndef board_hpp_INCLUDED

