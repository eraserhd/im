#include "objects.hpp"
using namespace std;
using namespace im;

//////////////////////////////////////////////////////////////////////////////
// Background

Background::Background()
{
}

Background::Background(Rect const& bounds, GLuint texture)
    : bounds_(bounds)
    , texture_(texture)
{
}

void Background::Render::operator () (Background const& bg) const {
    Rect const& r = bg.bounds();
    glBindTexture(GL_TEXTURE_2D, bg.texture_);
    glBegin(GL_QUADS);
        glTexCoord2f(float(r.left())/512.0, 0); glVertex3f(r.left(), r.top(), 0);
        glTexCoord2f(float(r.right())/512.0, 0); glVertex3f(r.right(), r.top(), 0);
        glTexCoord2f(float(r.right())/512.0, float(r.bottom())/512.0); glVertex3f(r.right(), r.bottom(), 0);
        glTexCoord2f(float(r.left())/512.0, float(r.bottom())/512.0); glVertex3f(r.left(), r.bottom(), 0);
    glEnd();
}

//////////////////////////////////////////////////////////////////////////////
// ElevatorShaft

ElevatorShaft::ElevatorShaft()
{
}

ElevatorShaft::ElevatorShaft(Rect const& bounds, GLuint texture)
    : bounds_(bounds)
    , texture_(texture)
{
}

void ElevatorShaft::Render::operator () (ElevatorShaft const& bg) const {
    Rect const& r = bg.bounds();
    glBindTexture(GL_TEXTURE_2D, bg.texture_);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(r.left(), r.top(), 0);
        glTexCoord2f(1, 0); glVertex3f(r.right(), r.top(), 0);
        glTexCoord2f(1, float(r.bottom())/20.0); glVertex3f(r.right(), r.bottom(), 0);
        glTexCoord2f(0, float(r.bottom())/20.0); glVertex3f(r.left(), r.bottom(), 0);
    glEnd();
}
