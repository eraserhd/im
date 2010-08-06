#include <SDL.h>
#include <SDL_opengl.h>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

#define VERSION "0.3"

SDL_Surface* screen;

#define ELEVATOR_COUNT 5
GLuint elevator_backgrounds[ELEVATOR_COUNT];
GLuint elevator_shaft;
GLuint elevator;
int elevator_y[ELEVATOR_COUNT];

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

void redraw() {
    glClear(GL_COLOR_BUFFER_BIT);

    const int height = 480*11;

    // elevator backgrounds
    for (int i = 0; i < ELEVATOR_COUNT; ++i) {
        int left = 320 + i*2*640, tleft = 0;
        int right = left + 2*640, tright = 2*640;
        if (i == 0)
            left = 0, tleft -= 320;
        if (i == ELEVATOR_COUNT-1)
            right += 320, tright += 320;

        const int height = 480*11;

        glBindTexture(GL_TEXTURE_2D, elevator_backgrounds[i]);
        glBegin(GL_QUADS);
            glTexCoord2f(float(tleft)/512.0, 0); glVertex3f(left, 0, 0);
            glTexCoord2f(float(tright)/512.0, 0); glVertex3f(right, 0, 0);
            glTexCoord2f(float(tright)/512.0, float(height)/512.0); glVertex3f(right, height, 0);
            glTexCoord2f(float(tleft)/512.0, float(height)/512.0); glVertex3f(left, height, 0);
        glEnd();
    }

    // elevator shafts
    glBindTexture(GL_TEXTURE_2D, elevator_shaft);
    for (int i = 0; i < ELEVATOR_COUNT; ++i) {
        int center = 320+640 + i*2*640;

        glBegin(GL_QUADS);
            glTexCoord2f(0,0); glVertex3f(center-40, 0, 0);
            glTexCoord2f(1,0); glVertex3f(center+40, 0, 0);
            glTexCoord2f(1,float(height)/20.0); glVertex3f(center+40, height, 0);
            glTexCoord2f(0,float(height)/20.0); glVertex3f(center-40, height, 0);
        glEnd();
    }

    // elevators
    glBindTexture(GL_TEXTURE_2D, elevator);
    for (int i = 0; i < ELEVATOR_COUNT; ++i) {
        int center = 320+640 + i*2*640;

        glBegin(GL_QUADS);
            glTexCoord2f(0,0); glVertex3f(center-40, elevator_y[i]-180, 0);
            glTexCoord2f(1,0); glVertex3f(center+40, elevator_y[i]-180, 0);
            glTexCoord2f(1,1); glVertex3f(center+40, elevator_y[i], 0);
            glTexCoord2f(0,1); glVertex3f(center-40, elevator_y[i], 0);
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
            redraw();
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
    for (int i = 0; i < ELEVATOR_COUNT; ++i) {
        elevator_backgrounds[i] = load_elevator_background(i);
        elevator_y[i] = rand()%480*10+100;
    }

    elevator_shaft = load_bmp_as_texture("elevator-2.bmp");
    elevator = load_bmp_as_texture("elevator-0.bmp");
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
     
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
     
    glOrtho(0.0f, 640*4, 480*4, 0.0f, -1.0f, 1.0f);
            
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    init();

    SDL_AddTimer(100, timer, NULL);
    event_loop();

    return 0;
}

