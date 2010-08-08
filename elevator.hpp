#ifndef elevator_hpp_INCLUDED
#define elevator_hpp_INCLUDED

#include <SDL_opengl.h>

namespace im {

class elevator {
public:
    int bottom;
    int y_velocity;
    GLuint bg_texture;

};

}

#endif // ndef elevator_hpp_INCLUDED

