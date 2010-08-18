using namespace std;
#define BOOST_TEST_MODULE GenerateTest
#include <boost/test/unit_test.hpp>
#include <map>
#include <sstream>
#include <string>
#include <utility>

#include "Generate.hpp"
#include "MockGL.hpp"
using namespace std;
using namespace im;

std::string guy_texture(Guy::AvatarState s) {
    std::map<Guy::AvatarState,GLuint> tt = load_guy_sprite<MockGL>();
    std::map<Guy::AvatarState,GLuint>::const_iterator i = tt.find(s);
    if (i == tt.end())
        return "";
    const GLuint texture = i->second;
    return MockGL::texture_loaded_from(texture);
}

BOOST_AUTO_TEST_CASE(load_guy_sprite_data) {
    BOOST_CHECK_EQUAL("standing-left-0", guy_texture(Guy::AvatarState(Guy::LEFT,Guy::STANDING,0)));
    BOOST_CHECK_EQUAL("standing-right-0", guy_texture(Guy::AvatarState(Guy::RIGHT,Guy::STANDING,0)));
}

BOOST_AUTO_TEST_CASE(loads_guy_running_left_frames) {
    for (int i = 0; i < 14; ++i) {
        ostringstream o;
        o << "running-left-" << i;
        BOOST_CHECK_EQUAL(o.str(), guy_texture(Guy::AvatarState(Guy::LEFT,Guy::RUNNING,i)));
    }
}

BOOST_AUTO_TEST_CASE(loads_guy_running_right_frames) {
    for (int i = 0; i < 14; ++i) {
        ostringstream o;
        o << "running-right-" << i;
        BOOST_CHECK_EQUAL(o.str(), guy_texture(Guy::AvatarState(Guy::RIGHT,Guy::RUNNING,i)));
    }
}

BOOST_AUTO_TEST_CASE(loads_guy_flipping_left_frames) {
    for (int i = 0; i < 12; ++i) {
        ostringstream o;
        o << "flipping-left-" << i;
        BOOST_CHECK_EQUAL(o.str(), guy_texture(Guy::AvatarState(Guy::LEFT,Guy::FLIPPING,i)));
    }
}

BOOST_AUTO_TEST_CASE(loads_guy_flipping_right_frames) {
    for (int i = 0; i < 12; ++i) {
        ostringstream o;
        o << "flipping-right-" << i;
        BOOST_CHECK_EQUAL(o.str(), guy_texture(Guy::AvatarState(Guy::RIGHT,Guy::FLIPPING,i)));
    }
}

BOOST_AUTO_TEST_CASE(loads_searching) {
    BOOST_CHECK_EQUAL("searching-0", guy_texture(Guy::AvatarState(Guy::LEFT,Guy::SEARCHING,0)));
    BOOST_CHECK_EQUAL("searching-0", guy_texture(Guy::AvatarState(Guy::RIGHT,Guy::SEARCHING,0)));
}
