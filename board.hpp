#ifndef board_hpp_INCLUDED
#define board_hpp_INCLUDED

#include <algorithm>
#include <vector>
#include "elevator.hpp"
#include "room.hpp"

namespace im {

class board {
public:
    enum { CORRIDOR_HEIGHT = 104 };

    std::vector<elevator> elevators;
    std::vector<std::vector<room> > rooms;

    static board generate();

private:
    board(std::vector<elevator> const& elevators,
          std::vector<std::vector<room> > const& rooms);

};

}

#endif // ndef board_hpp_INCLUDED

