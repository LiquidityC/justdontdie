ifdef CLANG
CC 			= clang++
else
CC			= g++
endif

SDL2_LDFLAGS		:= $(shell sdl2-config --static-libs)
SDL2_CFLAGS			:= $(shell sdl2-config --cflags)

CFLAGS				= -c -g -pedantic -Wall -Wpointer-arith -Wcast-qual -std=c++11 \
					  -include src/NewMacro.h $(SDL2_CFLAGS)
LD					= g++
LDFLAGS				=
AR					= ar
ARFLAGS				= rcs
RM					= rm
CP					= cp
ECHO				= echo -e
SHELL				= /bin/sh
CTAGS				= ctags

LIBS 				= $(SDL2_LDFLAGS) -lCppUTest -lCppUTestExt

DEPS				= $(wildcard src/*.h)
OBJDIR				= obj
LIBDIR				= lib

LIBRARY 			= $(LIBDIR)/libflat.a
LIB_SOURCES			= $(wildcard src/*.cpp)
LIB_OBJECTS			= $(addprefix $(OBJDIR)/,$(notdir $(LIB_SOURCES:.cpp=.o)))

LIB_INCLUDE_DIR		= include/flat
LIB_INCLUDE_SOURCES	= $(wildcard src/*.h)
LIB_INCLUDES		= $(addprefix $(LIB_INCLUDE_DIR)/,$(notdir $(LIB_INCLUDE_SOURCES)))

TEST				= test
TEST_SOURCES 		= $(wildcard src/*.cpp) $(wildcard testsrc/*.cpp)
TEST_OBJECTS 		= $(addprefix $(OBJDIR)/,$(notdir $(TEST_SOURCES:.cpp=.o)))

.PHONY: $(OBJDIR) $(LIBDIR) $(LIB_INCLUDE_DIR)

default: $(LIBRARY) $(LIB_INCLUDES)

all: $(LIBRARY) $(LIBRARY_UI) $(TEST)

check: $(TEST)
	./$(TEST) -v -c

$(LIB_INCLUDE_DIR)/%.h: src/%.h
	@$(CP) $< $@

$(TEST): $(LIBRARIES) $(TEST_OBJECTS) $(DEPS)
	$(LD) $(LDFLAGS) $(TEST_OBJECTS) $(LIBS) -o $@

$(LIBRARY): $(LIB_OBJECTS) $(DEPS)
	$(AR) $(ARFLAGS) $@ $(LIB_OBJECTS)

$(OBJDIR)/%.o: src/%.cpp
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: testsrc/%.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	@$(ECHO) Cleaning project
	@$(RM) -f $(TEST) $(LIBRARY) $(TEST_OBJECTS) $(LIBRARY_UI) $(LIB_OBJECTS) $(LIB_UI_OBJECTS) $(LIB_INCLUDES)
