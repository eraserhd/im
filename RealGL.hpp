#ifndef RealGL_hpp_INCLUDED
#define RealGL_hpp_INCLUDED

#include <string>
#include <SDL_opengl.h>

namespace im {

struct RealGL {
    static GLuint load_texture(std::string const& name);

#include "real_glwrap.hpp"
};

}


#endif // ndef RealGL_hpp_INCLUDED

