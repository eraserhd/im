#ifndef Cell_hpp_INCLUDED
#define Cell_hpp_INCLUDED

#include <boost/function.hpp>
#include <cassert>

namespace im {

struct Point {
    int x,y;

    inline Point()
        : x(0)
        , y(0)
    {}
    
    inline Point(int x, int y)
        : x(x)
        , y(y)
    {}
};

class Size {
public:
    inline Size()
        : width_(0)
        , height_(0)
    {}

    inline Size(int width, int height)
        : width_(width)
        , height_(height)
    {}

    inline int width() const {
        return width_;
    }
    inline int height() const {
        return height_;
    }

private:
    int width_;
    int height_;
};

class Rect {
public:
    inline Point const& upper_left() const {
        return upper_left_;
    }
    inline Point& upper_left() {
        return upper_left_;
    }
    inline Size const& size() const {
        return size_;
    }
    inline Size& size() {
        return size_;
    }

private:
    Point upper_left_;
    Size size_;
};

}

#endif // ndef Cell_hpp_INCLUDED

