#ifndef Room_hpp_INCLUDED
#define Room_hpp_INCLUDED

#include <vector>

namespace im {

class Room {
public:
    enum { HEIGHT = 400 };
    enum { WIDTH = 560 };
    enum { HGUTTER = 40 };
    enum { VGUTTER = 40 };

    std::vector<int> left_exits;
    std::vector<int> right_exits;
};

}

#endif // ndef Room_hpp_INCLUDED

