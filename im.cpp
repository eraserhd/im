#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include "Board.hpp"
#include "Generate.hpp"
#include <boost/foreach.hpp>
#include "RealGL.hpp"
using namespace std;
using namespace im;

#define VERSION "0.3"

SDL_Surface* screen;

Board g_board;

void die(string const& msg) {
    cerr<<msg<<endl;
    exit(EXIT_FAILURE);
}

GLuint RealGL::load_texture(string const& name) {
    SDL_Surface *s = IMG_Load(("data/" + name + ".png").c_str());
    if (NULL == s)
        die("Could not load " + name);

    GLuint result;
    glGenTextures(1, &result);
    glBindTexture(GL_TEXTURE_2D, result);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, 3, s->w, s->h, 0, 
                 GL_BGR, GL_UNSIGNED_BYTE, s->pixels);

    SDL_FreeSurface(s);
    return result;
}

void render(Board const& b) {
    glClear(GL_COLOR_BUFFER_BIT);

    b.render<RealGL>();

    /*
    // exits
    for (int j = 0; j < b.rooms.size(); ++j) {
        for (int i = 0; i < b.rooms[j].size(); ++i) {
            Room const& r = b.rooms[j][i];

            glBindTexture(GL_TEXTURE_2D, right_hallway);
            BOOST_FOREACH(int o, r.left_exits) {
                int ecenter = 320+640 + j*2*640;
                int left = ecenter+40;
                int right = left+326;
                int top = o + 480*i;
                int bottom = top + Board::CORRIDOR_HEIGHT;

                glBegin(GL_QUADS);
                    glTexCoord2f(0,0); glVertex3f(left, top, 0);
                    glTexCoord2f(1,0); glVertex3f(right, top, 0);
                    glTexCoord2f(1,1); glVertex3f(right, bottom, 0);
                    glTexCoord2f(0,1); glVertex3f(left, bottom, 0);
                glEnd();
            }

            glBindTexture(GL_TEXTURE_2D, left_hallway);
            BOOST_FOREACH(int o, r.right_exits) {
                int ecenter = 320+640 + (j-1)*2*640;
                int right = ecenter-40;
                int left = right-324;

                int top = o + 480*i;
                int bottom = top + Board::CORRIDOR_HEIGHT;

                glBegin(GL_QUADS);
                    glTexCoord2f(0,0); glVertex3f(left, top, 0);
                    glTexCoord2f(1,0); glVertex3f(right, top, 0);
                    glTexCoord2f(1,1); glVertex3f(right, bottom, 0);
                    glTexCoord2f(0,1); glVertex3f(left, bottom, 0);
                glEnd();
            }
        }
    }

    // rooms
    //glColor3f(0,0,0);
    for (int j = 0; j < b.rooms.size(); ++j) {
        for (int i = 0; i < b.rooms[j].size(); ++i) {
            / *
            glBegin(GL_QUADS);
                glVertex3f(left, top, 0);
                glVertex3f(right, top, 0);
                glVertex3f(right, bottom, 0);
                glVertex3f(left, bottom, 0);
            glEnd();
        }
    }
     */


    SDL_GL_SwapBuffers();
}

void event_loop() {
    SDL_Event event;
    while (SDL_WaitEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case 'q':
                return;
            }
            break;

        case SDL_QUIT:
            return;

        case SDL_USEREVENT:
            /*
            for (int i = 0; i < Board::ELEVATOR_COUNT; ++i) {
                g_board.elevator(i).bottom += g_board.elevator(i).y_velocity;
                if (g_board.elevator(i).y_velocity>0 && g_board.elevator(i).bottom > 480*11 - 20)
                    g_board.elevator(i).y_velocity = -g_board.elevator(i).y_velocity;
                else if (g_board.elevator(i).y_velocity<0 && g_board.elevator(i).bottom < 200)
                    g_board.elevator(i).y_velocity = -g_board.elevator(i).y_velocity;
            }
            */

            {
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                
                //int top = max(0,g_board.elevator(0).bottom - 180/2 - 480/2);

                //glOrtho(0.0f + 640, 640 + 640, 480 + top, 0.0f + top, -1.0f, 1.0f);
                glOrtho(0.0f, 640*3, 480*3, 0.0f, -1.0f, 1.0f);
            }

            render(g_board);
            break;
        }
    }
}

Uint32 timer(Uint32 interval, void* p) {
    SDL_Event ev;
    ev.type = SDL_USEREVENT;
    ev.user.code = 0;
    ev.user.data1 = 0;
    ev.user.data2 = 0;
    SDL_PushEvent(&ev);
    return interval;
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_AUDIO) != 0)
        die("SDL_Init failed");

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
        die("IMG_Init failed");

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    if ((screen = SDL_SetVideoMode(640,480,16, SDL_OPENGL)) == NULL)
        die("SDL_SetVideoMode failed");

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_TEXTURE_2D);
    glViewport(0, 0, 640, 480);
     
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    g_board = Generate<RealGL>() ();

    SDL_AddTimer(30, timer, NULL);
    event_loop();

    return 0;
}

