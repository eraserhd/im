#ifndef Board_hpp_INCLUDED
#define Board_hpp_INCLUDED

#include <algorithm>
#include <vector>
#include "Elevator.hpp"
#include "Room.hpp"

namespace im {

class Board {
public:
    enum { CORRIDOR_HEIGHT = 104 };

    std::vector<Elevator> elevators;
    std::vector<std::vector<Room> > rooms;

    static Board generate();

private:
    Board(std::vector<Elevator> const& elevators,
          std::vector<std::vector<Room> > const& rooms);

};

}

#endif // ndef Board_hpp_INCLUDED

