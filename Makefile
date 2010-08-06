
CXXFLAGS = $(shell sdl-config --cflags) -lGL -lGLU
LDFLAGS = $(shell sdl-config --libs)


im: im.cpp
