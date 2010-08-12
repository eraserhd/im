#include <iostream>
#ifndef Generate_hpp_INCLUDED
#define Generate_hpp_INCLUDED

#include <sstream>
#include <vector>
#include "Elevator.hpp"

namespace im {

template<typename ObjectLoader>
class Generate {
private:
    std::vector<Room> load_rooms() const {
        std::vector<Room> result;

        const int BOTTOM = Room::HEIGHT - Board::CORRIDOR_HEIGHT;
        int i = 1;
        while (result.size() < 36) {
            if ((i&15) == 0) {
                ++i;
                continue;
            }
            Room r;
            if (i&1)
                r.left_exits.push_back(0);
            if (i&2)
                r.left_exits.push_back(BOTTOM);
            if (i&4)
                r.right_exits.push_back(0);
            if (i&8)
                r.right_exits.push_back(BOTTOM);
            result.push_back(r);
            ++i;
        }

        return result;
    }

public:
    Generate(ObjectLoader loader = ObjectLoader())
        : loader(loader)
    {}

    Board operator () () const {
        std::vector<Elevator> elevators;
        const int COLUMNS = 6;
        std::vector<std::vector<Room> > rooms(COLUMNS);

        for (int i = 0; i < 5; ++i)
            elevators.push_back(Elevator());

        std::vector<Room> available_rooms = load_rooms();
        random_shuffle(available_rooms.begin(), available_rooms.end());
        std::vector<char> used(available_rooms.size(),false);

        // Pick one room for each column between shafts to ensure connectedness
        for (int j = 1; j <= 4; ++j)
            for (int i = 0; i < available_rooms.size(); ++i)
                if (!available_rooms[i].left_exits.empty() &&
                    !available_rooms[i].right_exits.empty() &&
                    !used[i])
                {
                    rooms[j].push_back(available_rooms[i]);
                    used[i]=true;
                    break;
                }

        // Fill remaining rooms
        const int ROOMS_PER_COLUMN = available_rooms.size()/COLUMNS;
        assert((available_rooms.size()%COLUMNS) == 0);

        {
            int taken = 0;
            for (int i = 0; taken < ROOMS_PER_COLUMN && i < available_rooms.size(); ++i) {
                if (used[i]) continue;
                if (!available_rooms[i].left_exits.empty()) continue;
                used[i]=true;
                rooms.front().push_back(available_rooms[i]);
                ++taken;
            }
        }

        {
            int taken = 0;
            for (int i = 0; taken < ROOMS_PER_COLUMN && i < available_rooms.size(); ++i) {
                if (used[i]) continue;
                if (!available_rooms[i].right_exits.empty()) continue;
                used[i]=true;
                rooms.back().push_back(available_rooms[i]);
                ++taken;
            }
        }

        Board b;

        // add elevator backgrounds
        for (int j = 0; j < Board::ELEVATOR_COUNT; ++j) {
            int left = Board::CELL_WIDTH/2 + j*2*Board::CELL_WIDTH, tleft = 0;
            int right = left + 2*Board::CELL_WIDTH, tright = 2*Board::CELL_WIDTH;
            if (j == 0)
                left = 0, tleft -= Board::CELL_WIDTH/2;
            if (j == Board::ELEVATOR_COUNT-1)
                right += Board::CELL_WIDTH/2, tright += Board::CELL_WIDTH/2;

            const int height = Board::CELL_HEIGHT * Board::HEIGHT;


            GLuint texture = load_elevator_bg(j);
            std::cerr<<"left="<<left<<", right="<<right<<", height="<<height<<std::endl;
            std::cerr<<"teture="<<texture<<std::endl;
            b.push_back(Background(Rect(Point(left,0), Size(right-left, height)), texture));
        }

        return b;
    }

private:
    GLuint load_elevator_bg(int n) const {
        std::ostringstream o;
        o << "elevator-background-"<<n<<".bmp";
        return loader.load_texture(o.str());
    }

    ObjectLoader loader;
};

}

#endif // ndef Generate_hpp_INCLUDED

