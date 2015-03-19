ifndef CXX
	CXX = g++
endif

# ECHO colors
CNORMAL	= \033[0m
CGREEN	= \033[32m

SDL2_LDFLAGS		:= $(shell pkg-config --libs SDL2_image SDL2_mixer SDL2_ttf sdl2)
SDL2_CFLAGS			:= $(shell pkg-config --cflags SDL2_image SDL2_mixer SDL2_ttf sdl2)

CXXFLAGS		= -c -g -pedantic -Wall -Wpointer-arith -Wcast-qual -std=c++11 \
			  -I./flat/include -I./include $(SDL2_CFLAGS) -DDEBUG

LD			= g++
LDFLAGS 	= -L./flat/lib/ -L./lib/
RM			= rm
ECHO		= echo
CP			= cp
MV			= mv
LIBS 		= $(SDL2_LDFLAGS) -lflat

OBJDIR				= obj
DEPS				= $(wildcard src/*.h)
EXECUTABLE 			= deadgaem
PROG_SOURCES 		= $(wildcard src/*.cpp)
PROG_OBJECTS 		= $(addprefix $(OBJDIR)/,$(notdir $(PROG_SOURCES:.cpp=.o)))

TEST				= test
TEST_SOURCES 		= $(filter-out src/deadgaem.cpp,$(wildcard src/*.cpp)) $(wildcard testsrc/*.cpp)
TEST_OBJECTS 		= $(addprefix $(OBJDIR)/,$(notdir $(TEST_SOURCES:.cpp=.o)))

LIBRARIES			= flat

.PHONY: $(LIBRARIES) $(OBJDIR) libs default dist clean cleanall check checkall

default: 
	@$(MAKE) --no-print-directory libs
	@$(ECHO) -e "$(CGREEN)Building $(EXECUTABLE)...$(CNORMAL)"
	@$(MAKE) --no-print-directory $(EXECUTABLE)
	@$(ECHO) -e "$(CGREEN)Building $(EXECUTABLE) complete$(CNORMAL)"

all: default checkall

run:
	@$(MAKE) --no-print-directory libs
	@$(MAKE) --no-print-directory $(EXECUTABLE)
	@./$(EXECUTABLE)

check: 
	@$(MAKE) --no-print-directory libs
	@$(MAKE) --no-print-directory $(TEST)
	@./$(TEST) -v -c

checkall:
	@$(MAKE) --no-print-directory libs
	@for d in $(LIBRARIES); do (cd $$d; $(MAKE) --no-print-directory check ); done
	@$(MAKE) --no-print-directory $(TEST)
	@./$(TEST)

libs: $(LIBRARIES)

dist: default
	$(LD) $(LDFLAGS) -Wl,-rpath,'.:lib/.' $(PROG_OBJECTS) $(LIBS) -o dist/$(EXECUTABLE)

$(EXECUTABLE): $(PROG_OBJECTS) $(DEPS)
	$(LD) $(LDFLAGS) $(PROG_OBJECTS) $(LIBS) -o $@ 

$(TEST): $(TEST_OBJECTS) $(DEPS)
	$(LD) $(LDFLAGS) $(TEST_OBJECTS) $(LIBS) -o $@

$(OBJDIR)/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJDIR)/%.o: testsrc/%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(LIBRARIES):
	@$(ECHO) -e "$(CGREEN)Building $@lib...$(CNORMAL)"
	@$(MAKE) -C $@ 
	@$(ECHO) -e "$(CGREEN)Building $@lib complete$(CNORMAL)"

clean:
	@$(ECHO) Cleaning project
	@$(RM) -f $(EXECUTABLE) $(TEST) $(PROG_OBJECTS) $(TEST_OBJECTS)

cleanall:
	@$(ECHO) Cleaning project and all libraries
	@for d in $(LIBRARIES); do (cd $$d; $(MAKE) --no-print-directory clean ); done
	@$(RM) -f $(EXECUTABLE) $(TEST) $(PROG_OBJECTS) $(TEST_OBJECTS)
