#ifndef objects_hpp_INCLUDED
#define objects_hpp_INCLUDED

#include <SDL_opengl.h>
#include "geometry.hpp"

namespace im {

class Background {
public:
    Background();
    Background(Rect const& bounds, GLuint texture);

    struct Render {
        void operator () (Background const& bg) const;
    };

    inline Rect const& bounds() const {
        return bounds_;
    }

private:
    GLuint texture_;
    Rect bounds_;
};



}

#endif // ndef objects_hpp_INCLUDED

