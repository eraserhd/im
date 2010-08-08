
CXXFLAGS = $(shell sdl-config --cflags) -g
LDFLAGS = $(shell sdl-config --libs) -g -lGL -lGLU

testable_SOURCES = 	board.cpp

im_SOURCES = 		$(testable_SOURCES) \
			im.cpp

test_PROGRAMS =		test_board

testable_OBJECTS =	$(addsuffix .o,$(basename $(testable_SOURCES)))
im_OBJECTS =		$(addsuffix .o,$(basename $(testable_OBJECTS)))

all: test im

test: $(test_PROGRAMS)
	for p in $(test_PROGRAMS); do \
	  ./$$p || exit $$?; \
	done

$(test_PROGRAMS): $(testable_OBJECTS)
$(addsuffix .o,$(test_PROGRAMS)): CPPFLAGS += -DBOOST_TEST_DYN_LINK
$(test_PROGRAMS): LDFLAGS += -lboost_unit_test_framework

im: $(im_OBJECTS)

.PHONY: clean test all

clean:
	rm -f *.o $(test_PROGRAMS) im

include .depends
.depends: $(wildcard *.cpp) $(wildcard *.hpp)
	g++ -MM $(CPPFLAGS) $(wildcard *.cpp) $(wildcard *.hpp) > .depends


