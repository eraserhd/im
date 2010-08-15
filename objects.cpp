#include <map>
#include <utility>
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

//////////////////////////////////////////////////////////////////////////////
// Elevator

Elevator::Elevator()
{
}

Elevator::Elevator(Rect const& bounds, GLuint texture)
    : bounds_(bounds)
    , texture_(texture)
{
}

//////////////////////////////////////////////////////////////////////////////
// Guy

Guy::Guy()
    : state_(LEFT,STANDING,0)
{
}

Guy::Guy(Point const& position, std::map<Guy::State, GLuint> textures)
    : position_(position)
    , state_(LEFT,STANDING,0)
    , textures_(textures)
{
}

// Guy::State

Guy::State::State(Facing facing, StateKind kind, int index)
    : facing(facing)
    , kind(kind)
    , index(index)
{
}

bool Guy::State::operator == (const Guy::State& rhs) const {
    return facing == rhs.facing && kind == rhs.kind && index == rhs.index;
}

bool Guy::State::operator < (const Guy::State& rhs) const {
    if (facing < rhs.facing) return true;
    if (facing > rhs.facing) return false;
    if (kind < rhs.kind) return true;
    if (kind > rhs.kind) return false;
    return index < rhs.index;
}

// Guy::Tick

void Guy::Tick::apply(Guy& guy) {
}

int Guy::frame_count(StateKind k) {
    switch (k) {
    case FLIPPING:
        return 12;
    case RUNNING:
        return 14;
    default:
        return 1;
    }
}
