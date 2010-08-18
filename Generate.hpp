#ifndef Generate_hpp_INCLUDED
#define Generate_hpp_INCLUDED

#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include "objects.hpp"
#include "Board.hpp"
#include "Room.hpp"

namespace im {

template<typename GL>
std::map<Guy::AvatarState, GLuint> load_guy_sprite();

template<typename GL>
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
    Board operator () () const {
        const int COLUMNS = 6;
        std::vector<std::vector<Room> > rooms(COLUMNS);

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
        const int height = Board::CELL_HEIGHT * Board::HEIGHT;
        for (int j = 0; j < Board::ELEVATOR_COUNT; ++j) {
            int left = Board::CELL_WIDTH/2 + j*2*Board::CELL_WIDTH, tleft = 0;
            int right = left + 2*Board::CELL_WIDTH, tright = 2*Board::CELL_WIDTH;
            if (j == 0)
                left = 0, tleft -= Board::CELL_WIDTH/2;
            if (j == Board::ELEVATOR_COUNT-1)
                right += Board::CELL_WIDTH/2, tright += Board::CELL_WIDTH/2;

            GLuint texture = load_elevator_bg(j);
            b.push_back(Background(Rect(Point(left,0), Size(right-left, height)), texture));
        }

        // elevator shafts
        GLuint es_texture = GL::load_texture("elevator-2");
        for (int j = 0; j < Board::ELEVATOR_COUNT; ++j) {
            int center = 320+640 + j*2*640;
            b.push_back(ElevatorShaft(Rect(Point(center-40,0), Size(80,height)), es_texture));
        }

        // elevators
        GLuint e_texture = GL::load_texture("elevator-0");
        for (int i = 0; i < 5; ++i) {
            int center = 320+640 + i*2*640;

            b.push_back(Elevator(Rect(Point(center-40,10), Size(80,180)), e_texture));
        }

        // guy
        b.push_back(Guy(Point(320+640-24, 58), load_guy_sprite<GL>()));
        return b;
    }

private:
    GLuint load_elevator_bg(int n) const {
        std::ostringstream o;
        o << "elevator-background-"<<n;
        return GL::load_texture(o.str());
    }
};

template<typename GL>
void load_series(std::map<Guy::AvatarState, GLuint>& r, Guy::Facing facing, Guy::AvatarStateKind kind, std::string const& base) {
    const int count = Guy::frame_count(kind);
    for (int i = 0; i < count; ++i) {
        std::ostringstream o;
        o << base << '-' << i;
        r[Guy::AvatarState(facing,kind,i)] = GL::load_texture(o.str());
    }
}

template<typename GL>
std::map<Guy::AvatarState, GLuint> load_guy_sprite() {
    std::map<Guy::AvatarState, GLuint> r;
    load_series<GL>(r, Guy::LEFT, Guy::STANDING, "standing-left");
    load_series<GL>(r, Guy::RIGHT, Guy::STANDING, "standing-right");
    load_series<GL>(r, Guy::LEFT, Guy::SEARCHING, "searching");
    load_series<GL>(r, Guy::RIGHT, Guy::SEARCHING, "searching");
    load_series<GL>(r, Guy::LEFT, Guy::RUNNING, "running-left");
    load_series<GL>(r, Guy::RIGHT, Guy::RUNNING, "running-right");
    load_series<GL>(r, Guy::LEFT, Guy::FLIPPING, "flipping-left");
    load_series<GL>(r, Guy::RIGHT, Guy::FLIPPING, "flipping-right");
    return r;
}

}

#endif // ndef Generate_hpp_INCLUDED

