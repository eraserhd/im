#ifndef Board_hpp_INCLUDED
#define Board_hpp_INCLUDED

#include <algorithm>
#include <list>
#include <map>
#include <string>
#include <vector>
#include "Object.hpp"
#include "Elevator.hpp"
#include "Room.hpp"
#include <boost/any.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/contains.hpp>

namespace im {

struct Background {};

class Board {
public:
    enum {
        CELL_WIDTH = 640,
        CELL_HEIGHT = 480,

        WIDTH = 11,
        HEIGHT = 6,
        
        ELEVATOR_COUNT = 5
    };

    enum { CORRIDOR_HEIGHT = 104 };

    static Board generate();

private:
    typedef boost::mpl::vector<Background> object_types;

public:
    // Add different classes of values (but only the ones in the object_types type-vector).
    template<typename T>
    void push_back(
            T t,
            typename boost::enable_if<typename boost::mpl::contains<object_types, T>::type >::type* dummy = 0
            )
    {
        objects_.push_back(boost::any(t));
    }

private:
    std::vector<boost::any> objects_;

    Board();
};

}

#endif // ndef Board_hpp_INCLUDED

