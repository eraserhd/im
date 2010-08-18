
UNAME = $(shell uname)

ifeq ($(UNAME),windows32)
SDL_INCLUDES = /c/MingW/include/SDL

CXXFLAGS = -g -I$(SDL_INCLUDES)
LDFLAGS = -lopengl32 -lglut32 -lSDL_image

im: LDFLAGS=-lmingw32 -lSDLmain -lSDL -lSDL_image -lopengl32 -lglut32

EXESUFFIX = .exe
else
CXXFLAGS = $(shell sdl-config --cflags) $(shell pkg-config SDL_image --cflags) -g
LDFLAGS = $(shell sdl-config --libs) $(shell pkg-config SDL_image --libs) -g -lGL -lGLU

EXESUFFIX =
endif

testable_SOURCES = 	Board.cpp \
                        geometry.cpp \
			objects.cpp \
			MockGL.cpp


im_SOURCES = 		$(testable_SOURCES) \
			im.cpp

test_PROGRAMS =		$(basename $(wildcard test_*.cpp))

testable_OBJECTS =	$(addsuffix .o,$(basename $(testable_SOURCES)))
im_OBJECTS =		$(addsuffix .o,$(basename $(im_SOURCES)))

all: im test

wrapgl: wrapgl.o
	$(CXX) $^ -o $@ $(LDFLAGS)

real_glwrap.hpp mock_glwrap.hpp: wrapgl opengl_apis.txt
	./wrapgl

test: $(test_PROGRAMS)
	for p in $(test_PROGRAMS); do \
	  ./$$p$(EXESUFFIX) || exit $$?; \
	done

$(test_PROGRAMS): $(testable_OBJECTS)

$(addsuffix .o,$(test_PROGRAMS)): CPPFLAGS += -DBOOST_TEST_DYN_LINK
$(test_PROGRAMS): LDFLAGS += -lboost_unit_test_framework-mt

im: $(im_OBJECTS)
	$(CXX) $^ -o $@ $(LDFLAGS)

test_%: test_%.o
	$(CXX) $^ -o $@ $(LDFLAGS)

.PHONY: clean test all

clean:
	rm -f *.o $(addsuffix $(EXESUFFIX),$(test_PROGRAMS) im wrapgl) *_glwrap.hpp

include .depends
.depends: $(wildcard *.cpp)
	g++ -MG -MM $(CPPFLAGS) $^ > .depends
