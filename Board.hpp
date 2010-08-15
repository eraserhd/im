#ifndef Board_hpp_INCLUDED
#define Board_hpp_INCLUDED

#include <algorithm>
#include <list>
#include <map>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "geometry.hpp"
#include "objects.hpp"
#include "Room.hpp"
#include <boost/mpl/contains.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/vector.hpp>
#include <iostream>
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
    typedef boost::mpl::vector<
        Background,
        ElevatorShaft,
        Elevator,
        Guy
        > object_types;

    typedef std::vector<boost::shared_ptr<Object> > ObjectVector;

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
        objects_.push_back(boost::shared_ptr<T>(new T(t)));
    }

private:
    struct ConstVisitorTraits {
        typedef ObjectVector::const_iterator Iterator;

        template<typename T>
        struct ValueRefType {
            typedef T const& type;
        };
    };
    struct VisitorTraits {
        typedef ObjectVector::iterator Iterator;

        template<typename T>
        struct ValueRefType {
            typedef T& type;
        };
    };

    template<typename VisitorT, typename Traits>
    struct VisitorAdaptor {
        typedef typename Traits::template ValueRefType<ObjectVector>::type VectorRefType;

        VisitorAdaptor(VisitorT const& visitor, VectorRefType objects)
            : visitor(visitor)
            , objects(objects)
        {}

        template<typename T>
        void operator () (T&) const {
            for (typename Traits::Iterator i = objects.begin();
                 i != objects.end();
                 ++i)
                if (typeid(T) == typeid(**i)) {
                    typedef typename Traits::template ValueRefType<T>::type RefType;
                    visitor((RefType) **i);
                }
        }

    private:
        VisitorT const& visitor;
        VectorRefType objects;
    };

    // Is type T renderable?
    template<typename T>
    struct Renderable {
    private:
        template<typename U>
        static char test(typename U::Render*);

        template<typename U>
        static char (&test(...))[2];

    public:
        static const bool value = sizeof(test<T>(0)) == 1;
    };

    // Is type T tickable?
    template<typename T>
    struct Tickable {
    private:
        template<typename U>
        static char test(typename U::Tick*);

        template<typename U>
        static char (&test(...))[2];

    public:
        static const bool value = sizeof(test<T>(0)) == 1;
    };

    // Render renderable types (NOP for other types)
    template<typename GLImplT>
    struct RenderVisitor {
        template<typename T>
        typename boost::enable_if<Renderable<T>, void>::type
        operator () (T const& o, char* dummy = 0) const
        {
            typedef typename T::Render R;
            R::template apply<GLImplT>(o);
        }

        template<typename T>
        typename boost::disable_if<Renderable<T>, void>::type
        operator () (T const&, int* dummy = 0) const {
        }
    };

    // Update state (by 1 tick) of tickable objects
    struct TickVisitor {
        template<typename T>
        typename boost::enable_if<Tickable<T>, void>::type
        operator () (T& o, char* dummy = 0) const
        {
            typedef typename T::Tick TT;
            TT::apply(o);
        }

        template<typename T>
        typename boost::disable_if<Tickable<T>, void>::type
        operator () (T&, int* dummy = 0) const {
        }
    };

public:
    template<typename T>
    void visit(T const& visitor) const
    {
        boost::mpl::for_each<object_types>(VisitorAdaptor<T, ConstVisitorTraits>(visitor, objects_));
    }

    template<typename T>
    void visit(T const& visitor)
    {
        boost::mpl::for_each<object_types>(VisitorAdaptor<T, VisitorTraits>(visitor, objects_));
    }

    template<typename GLImplT>
    void render() const
    {
        visit(RenderVisitor<GLImplT>());
    }

    void tick()
    {
        visit(TickVisitor());
    }

    Board();

private:
    std::vector<boost::shared_ptr<Object> > objects_;
};

}

#endif // ndef Board_hpp_INCLUDED

