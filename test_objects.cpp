#include <boost/foreach.hpp>
#define BOOST_TEST_MODULE ObjectsTest`
#include <boost/test/unit_test.hpp>

#include "objects.hpp"
#include <set>
#include <vector>
#include <boost/tuple/tuple.hpp>
#include <iostream>
#include "MockGL.hpp"
using namespace std;
using namespace im;

template<typename T>
void check_renders_with_correct_texture() {
    T bg(Rect(Point(10,10),Size(100,400)), 42);
    MockGL::calls.clear();

    typedef typename T::Render Render;
    Render::template apply<MockGL>(bg);
    BOOST_CHECK(MockGL::drew_quad_texture(42));
}

BOOST_AUTO_TEST_CASE(backgrounds_render_with_correct_textures) {
    check_renders_with_correct_texture<Background>();
}

BOOST_AUTO_TEST_CASE(elevator_shaft_renders_with_correct_texture) {
    check_renders_with_correct_texture<ElevatorShaft>();
}

BOOST_AUTO_TEST_CASE(elevator_renders_with_correct_texture) {
    check_renders_with_correct_texture<Elevator>();
}

Guy make_guy() {
    return Guy(Point(10,10));
}

BOOST_AUTO_TEST_CASE(guy_starts_in_standing_left_state) {
    Guy g = make_guy();
    BOOST_CHECK(g.state() == Guy::STANDING_LEFT);
}

BOOST_AUTO_TEST_CASE(guy_standing_left_tick_does_not_change_state) {
    Guy g = make_guy();
    Guy::Tick::tick(g);
    BOOST_CHECK(g.state() == Guy::STANDING_LEFT);
}
