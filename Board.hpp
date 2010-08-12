#ifndef Board_hpp_INCLUDED
#define Board_hpp_INCLUDED

#include <algorithm>
#include <list>
#include <map>
#include <string>
#include <vector>
#include "geometry.hpp"
#include "objects.hpp"
#include "Room.hpp"
#include <boost/any.hpp>
#include <boost/mpl/contains.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/vector.hpp>
#include <utility>

namespace im {

class Board {
public:
    enum {
        CELL_WIDTH = 640,
        CELL_HEIGHT = 480,

        WIDTH = 11,
        HEIGHT = 6,
        
        ELEVATOR_COUNT = 5
    };

    enum { CORRIDOR_HEIGHT = 104 };

private:
    typedef boost::mpl::vector<Background> object_types;

public:
    // Add different classes of values (but only the ones in the object_types type-vector).
    template<typename T>
    void push_back(
            T t,
            typename boost::enable_if<
                typename boost::mpl::contains<
                    object_types,
                    T
                    >::type
                >::type* dummy = 0
            )
    {
        objects_.push_back(boost::any(t));
    }

private:
    template<typename VisitorT>
    struct VisitorAdaptor {
        VisitorAdaptor(VisitorT const& visitor, std::vector<boost::any> objects)
            : visitor(visitor)
            , objects(objects)
        {}

        template<typename T>
        void operator () (T&) const
        {
            for (std::vector<boost::any>::const_iterator i = objects.begin();
                 i != objects.end();
                 ++i)
            {
                if (typeid(T) == i->type())
                    visitor(boost::any_cast<T>(*i));
            }
        }

    private:
        VisitorT const& visitor;
        std::vector<boost::any> const& objects;
    };

    struct Render {
        template<typename T>
        void operator () (
                T const& o, 
                typename T::Render* dummy = 0
                ) const
        {
            typename T::Render()(o);
        }

        void operator () (...) const {
        }
    };

public:
    template<typename T>
    void visit(T const& visitor) const
    {
        boost::mpl::for_each<object_types>(VisitorAdaptor<T>(visitor, objects_));
    }

    void render() const
    {
        visit(Render());
    }

    Board();

private:
    std::vector<boost::any> objects_;

};

}

#endif // ndef Board_hpp_INCLUDED

