#define BOOST_TEST_MODULE ObjectsTest`
#include <boost/test/unit_test.hpp>

#include "objects.hpp"
#include <set>
#include <vector>
#include <boost/tuple/tuple.hpp>
#include "MockGL.hpp"
using namespace std;
using namespace im;

BOOST_AUTO_TEST_CASE(background_) {
    Background bg(Rect(Point(10,10),Size(100,400)), 42);
    MockGL::calls.clear();
    Background::Render::apply<MockGL>(bg);
}
