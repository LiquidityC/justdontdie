ifdef CLANG
CC 			= clang++
else
CC			= g++
endif

SDL2_LDFLAGS		:= $(shell sdl2-config --static-libs)
SDL2_CFLAGS			:= $(shell sdl2-config --cflags)

CFLAGS		= -c -g -pedantic -Wall -Wpointer-arith -Wcast-qual -std=c++11 \
			  -I./flat/include -I./include -include src/NewMacro.h $(SDL2_CFLAGS) -DDEBUG
LD			= g++
LDFLAGS 	= -L./flat/lib/ -L./lib/ -Wl,-rpath,'./lib/.'
RM			= rm
ECHO		= echo
CP			= cp
MV			= mv
LIBS 		= $(SDL2_LDFLAGS) -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lBox2D -lCppUTest -lCppUTestExt -lflat

OBJDIR				= obj
DEPS				= $(wildcard src/*.h)
EXECUTABLE 			= deadgaem
PROG_SOURCES 		= $(wildcard src/*.cpp)
PROG_OBJECTS 		= $(addprefix $(OBJDIR)/,$(notdir $(PROG_SOURCES:.cpp=.o)))

TEST				= test
TEST_SOURCES 		= $(filter-out src/deadgaem.cpp,$(wildcard src/*.cpp)) $(wildcard testsrc/*.cpp)
TEST_OBJECTS 		= $(addprefix $(OBJDIR)/,$(notdir $(TEST_SOURCES:.cpp=.o)))

LIBRARIES			= flat

.PHONY: $(LIBRARIES) $(OBJDIR) dist

default: libs $(EXECUTABLE)

libs: $(LIBRARIES)

all: dirs libs default checkall

run: $(EXECUTABLE)
	./$(EXECUTABLE)

check: $(TEST)
	./$(TEST) -v -c

checkall: check
	@for d in $(LIBRARIES); do (cd $$d; $(MAKE) check ); done

dist: $(LIBRARIES) $(PROG_OBJECTS) $(DEPS)
	$(LD) $(LDFLAGS) $(PROG_OBJECTS) $(LIBS) -o dist/$(EXECUTABLE)

$(EXECUTABLE): $(LIBRARIES) $(PROG_OBJECTS) $(DEPS)
	$(LD) $(LDFLAGS) $(PROG_OBJECTS) $(LIBS) -o $@ 

$(TEST): $(LIBRARIES) $(TEST_OBJECTS) $(DEPS)
	$(LD) $(LDFLAGS) $(TEST_OBJECTS) $(LIBS) -o $@

$(OBJDIR)/%.o: src/%.cpp
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: testsrc/%.cpp
	$(CC) $(CFLAGS) $< -o $@

$(LIBRARIES):
	@$(MAKE) -C $@ 

clean:
	@$(ECHO) Cleaning project
	@$(RM) -f $(EXECUTABLE) $(TEST) $(PROG_OBJECTS) $(TEST_OBJECTS)

cleanall:
	@$(ECHO) Cleaning project and all libraries
	@for d in $(LIBRARIES); do (cd $$d; $(MAKE) clean ); done
	@$(RM) -f $(EXECUTABLE) $(TEST) $(PROG_OBJECTS) $(TEST_OBJECTS)
