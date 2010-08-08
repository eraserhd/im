#include <vector>
#ifndef room_hpp_INCLUDED
#define room_hpp_INCLUDED

namespace im {

class room {
public:
    enum { HEIGHT = 400 };
    enum { WIDTH = 560 };
    enum { HGUTTER = 40 };
    enum { VGUTTER = 40 };

    std::vector<int> left_exits;
    std::vector<int> right_exits;
};

}

#endif // ndef room_hpp_INCLUDED

