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
