#include <SDL.h>
#include <SDL_opengl.h>
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include "board.hpp"
#include <boost/foreach.hpp>
using namespace std;
using namespace im;

#define VERSION "0.3"

SDL_Surface* screen;

GLuint elevator_shaft;
GLuint elevator_texture;
GLuint left_hallway, right_hallway;

board g_board(board::generate());

void die(string const& msg) {
    cerr<<msg<<endl;
    exit(EXIT_FAILURE);
}

GLuint load_bmp_as_texture(string const& name) {
    SDL_Surface *s = SDL_LoadBMP(("data/" + name).c_str());
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

GLuint load_elevator_background(int n) {
    ostringstream out;
    out << "elevator-background-" << n << ".bmp";
    return load_bmp_as_texture(out.str());
}

void render(board const& b) {
    glClear(GL_COLOR_BUFFER_BIT);

    const int height = 480*11;

    // elevator backgrounds
    for (int i = 0; i < b.elevators.size(); ++i) {
        int left = 320 + i*2*640, tleft = 0;
        int right = left + 2*640, tright = 2*640;
        if (i == 0)
            left = 0, tleft -= 320;
        if (i == b.elevators.size()-1)
            right += 320, tright += 320;

        const int height = 480*11;

        glBindTexture(GL_TEXTURE_2D, b.elevators[i].bg_texture);
        glBegin(GL_QUADS);
            glTexCoord2f(float(tleft)/512.0, 0); glVertex3f(left, 0, 0);
            glTexCoord2f(float(tright)/512.0, 0); glVertex3f(right, 0, 0);
            glTexCoord2f(float(tright)/512.0, float(height)/512.0); glVertex3f(right, height, 0);
            glTexCoord2f(float(tleft)/512.0, float(height)/512.0); glVertex3f(left, height, 0);
        glEnd();
    }

    // elevator shafts
    glBindTexture(GL_TEXTURE_2D, elevator_shaft);
    for (int i = 0; i < b.elevators.size(); ++i) {
        int center = 320+640 + i*2*640;

        glBegin(GL_QUADS);
            glTexCoord2f(0,0); glVertex3f(center-40, 0, 0);
            glTexCoord2f(1,0); glVertex3f(center+40, 0, 0);
            glTexCoord2f(1,float(height)/20.0); glVertex3f(center+40, height, 0);
            glTexCoord2f(0,float(height)/20.0); glVertex3f(center-40, height, 0);
        glEnd();
    }

    // exits
    for (int j = 0; j < b.rooms.size(); ++j) {
        for (int i = 0; i < b.rooms[j].size(); ++i) {
            room const& r = b.rooms[j][i];

            glBindTexture(GL_TEXTURE_2D, right_hallway);
            BOOST_FOREACH(int o, r.left_exits) {
                int ecenter = 320+640 + j*2*640;
                int left = ecenter+40;
                int right = left+326;
                int top = o + 480*i;
                int bottom = top + board::CORRIDOR_HEIGHT;

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
                int bottom = top + board::CORRIDOR_HEIGHT;

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
            /*
            glBegin(GL_QUADS);
                glVertex3f(left, top, 0);
                glVertex3f(right, top, 0);
                glVertex3f(right, bottom, 0);
                glVertex3f(left, bottom, 0);
            glEnd();
            */
        }
    }

    // elevators
    glBindTexture(GL_TEXTURE_2D, elevator_texture);
    for (int i = 0; i < b.elevators.size(); ++i) {
        int center = 320+640 + i*2*640;

        glBegin(GL_QUADS);
            glTexCoord2f(0,0); glVertex3f(center-40, b.elevators[i].bottom-180, 0);
            glTexCoord2f(1,0); glVertex3f(center+40, b.elevators[i].bottom-180, 0);
            glTexCoord2f(1,1); glVertex3f(center+40, b.elevators[i].bottom, 0);
            glTexCoord2f(0,1); glVertex3f(center-40, b.elevators[i].bottom, 0);
        glEnd();
    }


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
            for (int i = 0; i < g_board.elevators.size(); ++i) {
                g_board.elevators[i].bottom += g_board.elevators[i].y_velocity;
                if (g_board.elevators[i].y_velocity>0 && g_board.elevators[i].bottom > 480*11 - 20)
                    g_board.elevators[i].y_velocity = -g_board.elevators[i].y_velocity;
                else if (g_board.elevators[i].y_velocity<0 && g_board.elevators[i].bottom < 200)
                    g_board.elevators[i].y_velocity = -g_board.elevators[i].y_velocity;
            }

            {
                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
                
                int top = max(0,g_board.elevators[0].bottom - 180/2 - 480/2);

                //glOrtho(0.0f + 640, 640 + 640, 480 + top, 0.0f + top, -1.0f, 1.0f);
                glOrtho(0.0f, 640*6, 480*6, 0.0f, -1.0f, 1.0f);
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

void init() {
    for (int i = 0; i < g_board.elevators.size(); ++i) {
        g_board.elevators[i].bg_texture = load_elevator_background(i);
        g_board.elevators[i].bottom = rand()%480*10+100;
        g_board.elevators[i].y_velocity = -10;
    }

    elevator_shaft = load_bmp_as_texture("elevator-2.bmp");
    elevator_texture = load_bmp_as_texture("elevator-0.bmp");
    left_hallway = load_bmp_as_texture("elevator-3.bmp");
    right_hallway = load_bmp_as_texture("elevator-4.bmp");
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_AUDIO) != 0)
        die("SDL_Init failed");

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    if ((screen = SDL_SetVideoMode(640,480,16, SDL_OPENGL)) == NULL)
        die("SDL_SetVideoMode failed");

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_TEXTURE_2D);
    glViewport(0, 0, 640, 480);
     
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    init();

    SDL_AddTimer(30, timer, NULL);
    event_loop();

    return 0;
}

