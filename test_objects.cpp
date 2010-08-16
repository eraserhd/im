#define BOOST_TEST_MODULE ObjectsTest`
#include <boost/test/unit_test.hpp>

#include <SDL_keysym.h>
#include <algorithm>
#include "objects.hpp"
#include <set>
#include <vector>
#include <boost/foreach.hpp>
#include <boost/tuple/tuple.hpp>
#include <iostream>
#include <map>
#include <utility>
#include "MockGL.hpp"
#include "Generate.hpp"
using namespace std;
using namespace im;
#define ALL(c) (c).begin(),(c).end()

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
    return Guy(Point(10,10), load_guy_sprite<MockGL>());
}

BOOST_AUTO_TEST_CASE(guy_starts_in_standing_left_state) {
    Guy g = make_guy();
    BOOST_CHECK(g.state() == Guy::State(Guy::LEFT,Guy::STANDING,0));
}

vector<Guy::State> all_guy_states() {
    vector<Guy::State> r;
    for (int d = (int) Guy::LEFT; d <= (int) Guy::RIGHT; ++d)
        for (int k = (int) Guy::STANDING; k <= (int) Guy::SEARCHING; ++k) {
            const int frames = Guy::frame_count((Guy::StateKind) k);
            for (int i = 0; i < frames; ++i)
                r.push_back(Guy::State((Guy::Facing) d, (Guy::StateKind) k, i));
        }
    random_shuffle(ALL(r));
    return r;
}

struct TestableGuy : public Guy {
    inline State& state() { return state_; }
};
BOOST_AUTO_TEST_CASE(guy_renders_with_texture_corresponding_to_state) {
    vector<Guy::State> ss = all_guy_states();
    std::map<Guy::State,GLuint> ts = load_guy_sprite<MockGL>();
    BOOST_FOREACH(Guy::State s, ss) {
        Guy g(Point(99,99), ts);
        reinterpret_cast<TestableGuy&>(g).state() = s;

        MockGL::calls.clear();
        Guy::Render::apply<MockGL>(g);

        BOOST_CHECK(MockGL::drew_quad_texture(ts[s]));
    }
}

BOOST_AUTO_TEST_CASE(guy_renders_with_alpha_clamping) {
    Guy g = make_guy();
    Guy::Render::apply<MockGL>(g);
    
    bool alpha_test = false;
    bool alpha_func = false;
    BOOST_FOREACH(boost::shared_ptr<MockGL::C_base> p, MockGL::calls) {
        if (typeid(*p) == typeid(MockGL::C_glEnable) && ((MockGL::C_glEnable&) *p).what == GL_ALPHA_TEST)
            alpha_test = true;
        if (typeid(*p) == typeid(MockGL::C_glAlphaFunc))
            alpha_func = true;
    }
    BOOST_CHECK(alpha_test);
    BOOST_CHECK(alpha_func);
}

BOOST_AUTO_TEST_CASE(guy_standing_left_tick_does_not_change_state) {
    std::vector<TickKey> keys;
    BOOST_CHECK(Guy::State(Guy::LEFT, Guy::STANDING, 0) == Guy::State(Guy::LEFT, Guy::STANDING, 0).next(keys));
}

BOOST_AUTO_TEST_CASE(tick_cycles_state_index) {
    std::vector<TickKey> keys;
    BOOST_CHECK(Guy::State(Guy::LEFT, Guy::RUNNING, 1) == Guy::State(Guy::LEFT, Guy::RUNNING, 0).next(keys));
    BOOST_CHECK(Guy::State(Guy::LEFT, Guy::RUNNING, 0) == Guy::State(Guy::LEFT, Guy::RUNNING, Guy::frame_count(Guy::RUNNING)-1).next(keys));
}

BOOST_AUTO_TEST_CASE(kb_left_down_changes_from_standing_to_running_left) {
    Guy::State s(Guy::LEFT,Guy::STANDING,0);

    std::vector<TickKey> keys;
    keys.push_back(TickKey(true, SDLK_LEFT));
    Guy::State n = s.next(keys);

    BOOST_CHECK(Guy::State(Guy::LEFT, Guy::RUNNING, 0) == n);
}

BOOST_AUTO_TEST_CASE(kb_left_up_changes_from_running_to_standing_left) {
    Guy::State s(Guy::LEFT,Guy::RUNNING,0);

    std::vector<TickKey> keys;
    keys.push_back(TickKey(false, SDLK_LEFT));
    Guy::State n = s.next(keys);

    BOOST_CHECK(Guy::State(Guy::LEFT, Guy::STANDING, 0) == n);
}

BOOST_AUTO_TEST_CASE(kb_right_down_while_running_left_changes_direction) {
    Guy::State s(Guy::LEFT,Guy::RUNNING,0);

    std::vector<TickKey> keys;
    keys.push_back(TickKey(true, SDLK_RIGHT));
    Guy::State n = s.next(keys);

    BOOST_CHECK(Guy::State(Guy::RIGHT, Guy::RUNNING, 0) == n);
}

BOOST_AUTO_TEST_CASE(kb_left_down_while_running_right_changes_direction) {
    Guy::State s(Guy::RIGHT,Guy::RUNNING,0);

    std::vector<TickKey> keys;
    keys.push_back(TickKey(true, SDLK_LEFT));
    Guy::State n = s.next(keys);

    BOOST_CHECK(Guy::State(Guy::LEFT, Guy::RUNNING, 0) == n);
}

BOOST_AUTO_TEST_CASE(kb_left_up_while_running_right_does_nothing) {
    Guy::State s(Guy::RIGHT,Guy::RUNNING,6);

    std::vector<TickKey> keys;
    keys.push_back(TickKey(false, SDLK_LEFT));
    Guy::State n = s.next(keys);

    BOOST_CHECK(Guy::State(Guy::RIGHT, Guy::RUNNING, 7) == n);
}

BOOST_AUTO_TEST_CASE(kb_right_up_while_running_left_does_nothing) {
    Guy::State s(Guy::LEFT,Guy::RUNNING,6);

    std::vector<TickKey> keys;
    keys.push_back(TickKey(false, SDLK_RIGHT));
    Guy::State n = s.next(keys);

    BOOST_CHECK(Guy::State(Guy::LEFT, Guy::RUNNING, 7) == n);
}

BOOST_AUTO_TEST_CASE(kb_space_transitions_standing_and_running_to_flipping) {
    const Guy::Facing D[] = {Guy::LEFT,Guy::RIGHT};
    for (int d = 0; d < 2; ++d) {
        const Guy::StateKind K[] = {Guy::STANDING,Guy::RUNNING};
        for (int k = 0; k < 2; ++k) {
            Guy::State s(D[d], K[k], 5 % Guy::frame_count(K[k]));

            std::vector<TickKey> keys;
            keys.push_back(TickKey(true, SDLK_SPACE));
            Guy::State n = s.next(keys);

            BOOST_CHECK(Guy::State(D[d], Guy::FLIPPING, 0) == n);
        }
    }
}
