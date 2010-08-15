#ifndef objects_hpp_INCLUDED
#define objects_hpp_INCLUDED

#include <SDL_opengl.h>
#include <cassert>
#include <map>
#include <utility>
#include "geometry.hpp"


namespace im {

struct Object {
    virtual ~Object();
};

class Background : public Object {
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

class ElevatorShaft : public Object {
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

class Elevator : public Object {
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

class Guy : public Object {
public:
    enum Facing {
        LEFT,
        RIGHT
    };

    enum StateKind {
        STANDING,
        FLIPPING,
        RUNNING,
        SEARCHING
    };

    struct State {
        State(Facing facing, StateKind kind, int index);

        bool operator == (const State& rhs) const;
        bool operator < (const State& rhs) const;

        State next() const;

    private:
        Facing facing;
        StateKind kind;
        int index;
    };

    Guy();
    Guy(Point const& position, std::map<State, GLuint> textures);


    inline Point const& position() const {
        return position_;
    }
    inline State const& state() const {
        return state_;
    }

    struct Tick {
        static void apply(Guy& guy);
    };
    struct Render {
        template<typename GL>
        static void apply(Guy const& g) {
            Rect r(g.position_, Size(48,84));

            std::map<State, GLuint>::const_iterator it = g.textures_.find(g.state());
            assert(it != g.textures_.end());

            GL::glEnable(GL_ALPHA_TEST);
            GL::glAlphaFunc(GL_GREATER, 0.0);
            GL::glBindTexture(GL_TEXTURE_2D, it->second);
            GL::glBegin(GL_QUADS);
                GL::glTexCoord2f(0, 0); GL::glVertex3f(r.left(), r.top(), 0);
                GL::glTexCoord2f(1, 0); GL::glVertex3f(r.right(), r.top(), 0);
                GL::glTexCoord2f(1, 1); GL::glVertex3f(r.right(), r.bottom(), 0);
                GL::glTexCoord2f(0, 1); GL::glVertex3f(r.left(), r.bottom(), 0);
            GL::glEnd();
            GL::glDisable(GL_ALPHA_TEST);
        }
    };

    static int frame_count(StateKind kind);

protected:
    State state_;

private:
    Point position_;
    std::map<State, GLuint> textures_;
};

}

#endif // ndef objects_hpp_INCLUDED

