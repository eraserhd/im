#include <fstream>
#include <map>
#include <string>
#include "MockGL.hpp"
using namespace std;
using namespace im;

std::vector<boost::shared_ptr<MockGL::C_base> > MockGL::calls;

GLuint MockGL::next_texture = 4242;
std::map<GLuint, std::string> MockGL::loaded_from;

bool MockGL::drew_quad_texture(GLuint texture) {
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

std::string MockGL::texture_loaded_from(GLuint texture) {
    std::map<GLuint, std::string>::const_iterator it = loaded_from.find(texture);
    if (loaded_from.end() == it)
        return "";
    return it->second;
}

GLuint MockGL::load_texture(std::string const& name) {

    string fn = "data/" + name + ".png";
    ifstream in(fn.c_str(),ios::in);
    if (!in)
        throw 1;

    loaded_from[next_texture] = name;
    return next_texture++;
}

