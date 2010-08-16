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
#include <utility>
#include <vector>
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

    // Our four byte PNGs have alpha
    GLint internal_format = 3;
    GLenum format = GL_RGB;
    if (s->format->BytesPerPixel == 4) {
        format = GL_RGBA; 
        internal_format = 4;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, s->w, s->h, 0, 
                 format, GL_UNSIGNED_BYTE, s->pixels);

    SDL_FreeSurface(s);
    return result;
}

void render(Board const& b) {
    glClear(GL_COLOR_BUFFER_BIT);
    b.render<RealGL>();
    SDL_GL_SwapBuffers();
}

void event_loop() {
    SDL_Event event;
    std::vector<TickKey> keys;
    while (SDL_WaitEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN:
            if (SDLK_q == event.key.keysym.sym)
                return;
            keys.push_back(TickKey(true, event.key.keysym.sym));
            break;

        case SDL_KEYUP:
            keys.push_back(TickKey(false, event.key.keysym.sym));
            break;

        case SDL_QUIT:
            return;

        case SDL_USEREVENT:
            g_board.tick(keys);
            keys.clear();

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0.0f + 640, 640 + 640, 480, 0.0f, -1.0f, 1.0f);

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

    SDL_AddTimer(60, timer, NULL);
    event_loop();
    return 0;
}

