#ifndef Board_hpp_INCLUDED
#define Board_hpp_INCLUDED

#include <algorithm>
#include <vector>
#include "Cell.hpp"
#include "Elevator.hpp"
#include "Room.hpp"

namespace im {

class Board {
public:
    enum {
        CELL_WIDTH = 640,
        CELL_HEIGHT = 480,

        WIDTH = 11,
        HEIGHT = 6,
        
        ELEVATOR_COUNT = 5


    };

    Cell const& cell(int i, int j) const;
    Cell& cell(int i, int j);

    Elevator const& elevator(int n) const;
    Elevator& elevator(int n);

    enum { CORRIDOR_HEIGHT = 104 };

    std::vector<std::vector<Room> > rooms;

    static Board generate();

private:
    Cell cells_[HEIGHT][WIDTH];
    Elevator elevators_[ELEVATOR_COUNT];

    Board(std::vector<std::vector<Room> > const& rooms
          );
};

}

#endif // ndef Board_hpp_INCLUDED

