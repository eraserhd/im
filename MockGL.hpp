#ifndef MOckGL_hpp_INCLUDED
#define MOckGL_hpp_INCLUDED

#include <vector>
#include <boost/shared_ptr.hpp>
#include <SDL_opengl.h>
#include <boost/foreach.hpp>

namespace im {

struct MockGL {
    struct C_base {
        virtual ~C_base() {}
    };

    static std::vector<boost::shared_ptr<C_base> > calls;
#include "mock_glwrap.hpp"

    // Assertions for testing
    
    static bool drew_quad_texture(GLuint texture) {
        enum State {
            WANT_BIND,
            WANT_BEGIN,
            WANT_TEXCOORD,
            WANT_VERTEX,
            SUCCESS
        };

        State s = WANT_BIND;
        int vertex_count = 0;

        BOOST_FOREACH(boost::shared_ptr<C_base> p, calls) {
            switch (s) {
            case WANT_BIND:
                if (typeid(*p) == typeid(C_glBindTexture) &&
                    (texture == ((MockGL::C_glBindTexture const&) *p).texture))
                    s = WANT_BEGIN;
                break;

            case WANT_BEGIN:
                if (typeid(*p) == typeid(C_glBegin) &&
                    (GL_QUADS == ((MockGL::C_glBegin const&) *p).what))
                    s = WANT_TEXCOORD;
                break;

            case WANT_TEXCOORD:
                if (typeid(*p) == typeid(C_glTexCoord2f))
                    s = WANT_VERTEX;
                else if (typeid(*p) == typeid(C_glEnd))
                    s = SUCCESS;
                break;

            case WANT_VERTEX:
                if (typeid(*p) == typeid(C_glVertex3f)) {
                    s = WANT_TEXCOORD;
                    ++vertex_count;
                }
                break;
            }
        }

        if (0 == vertex_count)
            return false;
        if (vertex_count%4 != 0)
            return false;

        return SUCCESS == s;
    }

};

}

#endif // ndef MOckGL_hpp_INCLUDED

