#ifndef MOckGL_hpp_INCLUDED
#define MOckGL_hpp_INCLUDED

#include <vector>
#include <boost/shared_ptr.hpp>
#include <SDL_opengl.h>

namespace im {

struct MockGL {
    struct C_base {
        virtual ~C_base() {}
    };

    static std::vector<boost::shared_ptr<C_base> > calls;
#include "mock_glwrap.hpp"
};

}

#endif // ndef MOckGL_hpp_INCLUDED

