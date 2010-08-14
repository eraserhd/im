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
        template<typename GL>
        static void apply(Background const& bg) {
            Rect const& r = bg.bounds();
            GL::glBindTexture(GL_TEXTURE_2D, bg.texture_);
            GL::glBegin(GL_QUADS);
                GL::glTexCoord2f(float(r.left())/512.0, 0); GL::glVertex3f(r.left(), r.top(), 0);
                GL::glTexCoord2f(float(r.right())/512.0, 0); GL::glVertex3f(r.right(), r.top(), 0);
                GL::glTexCoord2f(float(r.right())/512.0, float(r.bottom())/512.0); GL::glVertex3f(r.right(), r.bottom(), 0);
                GL::glTexCoord2f(float(r.left())/512.0, float(r.bottom())/512.0); GL::glVertex3f(r.left(), r.bottom(), 0);
            GL::glEnd();
        }
    };

    inline Rect const& bounds() const {
        return bounds_;
    }

private:
    GLuint texture_;
    Rect bounds_;
};

class ElevatorShaft {
public:
    ElevatorShaft();
    ElevatorShaft(Rect const& bounds, GLuint texture);

    struct Render {
        template<typename GL>
        static void apply(ElevatorShaft const& es) {
            Rect const& r = es.bounds();
            GL::glBindTexture(GL_TEXTURE_2D, es.texture_);
            GL::glBegin(GL_QUADS);
                GL::glTexCoord2f(0, 0); GL::glVertex3f(r.left(), r.top(), 0);
                GL::glTexCoord2f(1, 0); GL::glVertex3f(r.right(), r.top(), 0);
                GL::glTexCoord2f(1, float(r.bottom())/20.0); GL::glVertex3f(r.right(), r.bottom(), 0);
                GL::glTexCoord2f(0, float(r.bottom())/20.0); GL::glVertex3f(r.left(), r.bottom(), 0);
            GL::glEnd();
        }
    };

    inline Rect const& bounds() const {
        return bounds_;
    }

private:
    GLuint texture_;
    Rect bounds_;
};

class Elevator {
public:
    Elevator();
    Elevator(Rect const& bounds, GLuint texture);
    
    struct Render {
        template<typename GL>
        static void apply(Elevator const& e) {
            Rect const& r = e.bounds();
            GL::glBindTexture(GL_TEXTURE_2D, e.texture_);
            GL::glBegin(GL_QUADS);
                GL::glTexCoord2f(0, 0); GL::glVertex3f(r.left(), r.top(), 0);
                GL::glTexCoord2f(1, 0); GL::glVertex3f(r.right(), r.top(), 0);
                GL::glTexCoord2f(1, 1); GL::glVertex3f(r.right(), r.bottom(), 0);
                GL::glTexCoord2f(0, 1); GL::glVertex3f(r.left(), r.bottom(), 0);
            GL::glEnd();
        }
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

