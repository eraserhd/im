
UNAME = $(shell uname)

ifeq ($(UNAME),windows32)
SDL_INCLUDES = /c/MingW/include/SDL

CXXFLAGS = -g -I$(SDL_INCLUDES)
LDFLAGS = -lopengl32 -lglut32

im: LDFLAGS=-lmingw32 -lSDLmain -lSDL -lopengl32 -lglut32

EXESUFFIX = .exe
else
CXXFLAGS = $(shell sdl-config --cflags) -g
LDFLAGS = $(shell sdl-config --libs) -g -lGL -lGLU

EXESUFFIX =
endif

testable_SOURCES = 	Board.cpp \
                        Object.cpp


im_SOURCES = 		$(testable_SOURCES) \
			im.cpp

test_PROGRAMS =		test_Board

testable_OBJECTS =	$(addsuffix .o,$(basename $(testable_SOURCES)))
im_OBJECTS =		$(addsuffix .o,$(basename $(im_SOURCES)))

all: im test

test: $(test_PROGRAMS)
	for p in $(test_PROGRAMS); do \
	  ./$$p || exit $$?; \
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
	rm -f *.o $(addsuffix $(EXESUFFIX),$(test_PROGRAMS)) im$(EXESUFFIX)

include .depends
.depends: $(wildcard *.cpp)
	g++ -MM $(CPPFLAGS) $^ > .depends


