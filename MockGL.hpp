#ifndef MOckGL_hpp_INCLUDED
#define MOckGL_hpp_INCLUDED

#include <vector>
#include <boost/shared_ptr.hpp>
#include <SDL_opengl.h>
#include <boost/foreach.hpp>
#include <map>
#include <string>

namespace im {

struct MockGL {
    struct C_base {
        virtual ~C_base() {}
    };

    static std::vector<boost::shared_ptr<C_base> > calls;

    static GLuint next_texture;
    static std::map<GLuint, std::string> loaded_from;

    static GLuint load_texture(std::string const& name);

#include "mock_glwrap.hpp"

    // Assertions for testing
    
    static bool drew_quad_texture(GLuint texture);
    static std::string texture_loaded_from(GLuint texture);

};

}

#endif // ndef MOckGL_hpp_INCLUDED

