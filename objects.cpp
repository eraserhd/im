#include <SDL_keysym.h>
#include <boost/foreach.hpp>
#include <map>
#include <utility>
#include <vector>
#include "objects.hpp"
using namespace std;
using namespace im;

//////////////////////////////////////////////////////////////////////////////
// Object

Object::~Object()
{
}

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
    : avatar_state_(LEFT,STANDING,0)
{
}

Guy::Guy(Point const& position, std::map<Guy::AvatarState, GLuint> textures)
    : position_(position)
    , avatar_state_(LEFT,STANDING,0)
    , textures_(textures)
{
}

// Guy::AvatarState

Guy::AvatarState::AvatarState(Facing facing, AvatarStateKind kind, int index)
    : facing(facing)
    , kind(kind)
    , index(index)
{
}

bool Guy::AvatarState::operator == (const Guy::AvatarState& rhs) const {
    return facing == rhs.facing && kind == rhs.kind && index == rhs.index;
}

bool Guy::AvatarState::operator < (const Guy::AvatarState& rhs) const {
    if (facing < rhs.facing) return true;
    if (facing > rhs.facing) return false;
    if (kind < rhs.kind) return true;
    if (kind > rhs.kind) return false;
    return index < rhs.index;
}

Guy::AvatarState Guy::AvatarState::next(std::vector<TickKey>  const& keys) const {
    Guy::AvatarState ns(*this);

    bool changed = false;
    BOOST_FOREACH(TickKey const& k, keys) {
        if (k.down && (k.sym == SDLK_LEFT || k.sym == SDLK_RIGHT) && (kind == STANDING || kind == RUNNING)) {
            changed = true;
            ns.facing = (k.sym == SDLK_LEFT) ? LEFT : RIGHT;
            ns.kind = RUNNING;
            ns.index = 0;
            continue;
        }
        if (!k.down && ((k.sym == SDLK_LEFT && facing == LEFT) || (k.sym == SDLK_RIGHT && facing == RIGHT)) && kind == RUNNING) {
            changed = true;
            ns.facing = (k.sym == SDLK_LEFT) ? LEFT : RIGHT;
            ns.kind = STANDING;
            ns.index = 0;
            continue;
        }
        if (k.down && k.sym == SDLK_SPACE && (kind == STANDING || kind == RUNNING)) {
            changed = true;
            ns.kind = FLIPPING;
            ns.index = 0;
            continue;
        }
    }

    if (!changed)
        ns.index = (ns.index + 1) % frame_count(kind);

    return ns;
}

// Guy::Tick

void Guy::Tick::apply(Guy& guy, std::vector<TickKey> const& keys) {
    guy.avatar_state_ = guy.avatar_state_.next(keys);
}

int Guy::frame_count(AvatarStateKind k) {
    switch (k) {
    case FLIPPING:
        return 12;
    case RUNNING:
        return 14;
    default:
        return 1;
    }
}
