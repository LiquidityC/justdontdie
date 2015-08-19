ifndef CXX
CXX			= g++
endif

SDL2_LDFLAGS		:= $(shell sdl2-config --libs) -lSDL2_image -lSDL2_mixer -lSDL2_ttf
SDL2_CFLAGS			:= $(shell sdl2-config --cflags)

CXXFLAGS			= -c -g -pedantic -Wall -Wpointer-arith -Wcast-qual -std=c++11 $(SDL2_CFLAGS)

LD					= g++
LDFLAGS				=
AR					= ar
ARFLAGS				= rcs
RM					= rm
CP					= cp
ECHO				= echo
SHELL				= /bin/sh
LINT				= cpplint
CTAGS				= ctags

LIBS 				= $(SDL2_LDFLAGS) -lsqlite3

DEPS				= $(wildcard src/*.h)
OBJDIR				= obj
LIBDIR				= lib

LIBRARY 			= $(LIBDIR)/libflat.a
LIB_SOURCES			= $(wildcard src/*.cpp)
LIB_OBJECTS			= $(addprefix $(OBJDIR)/,$(notdir $(LIB_SOURCES:.cpp=.o)))

LIB_INCLUDE_DIR		= include/flat
LIB_UNIFIED_INCLUDE	= $(LIB_INCLUDE_DIR)/flat.h
LIB_INCLUDE_SOURCES	= $(wildcard src/*.h)
LIB_INCLUDES		= $(addprefix $(LIB_INCLUDE_DIR)/,$(notdir $(LIB_INCLUDE_SOURCES)))

TEST				= test
TEST_SOURCES 		= $(wildcard src/*.cpp) $(wildcard testsrc/*.cpp)
TEST_OBJECTS 		= $(addprefix $(OBJDIR)/,$(notdir $(TEST_SOURCES:.cpp=.o)))

-include ../Makefile.config

.PHONY: lint $(OBJDIR) $(LIBDIR) $(LIB_INCLUDE_DIR) $(LIB_UNIFIED_INCLUDE)

default: $(LIBRARY) $(LIB_INCLUDES) $(LIB_UNIFIED_INCLUDE)

all: $(LIBRARY) $(TEST) $(LIB_UNIFIED_INCLUDE)

check: $(TEST)
	./$(TEST)

$(LIB_INCLUDE_DIR)/%.h: src/%.h
	@$(CP) $< $@

$(LIB_UNIFIED_INCLUDE): $(LIB_INCLUDES)
	@$(ECHO) "#ifndef _FLAT_H" > $(LIB_INCLUDE_DIR)/flat.h
	@$(ECHO) "#define _FLAT_H" >> $(LIB_INCLUDE_DIR)/flat.h
	@$(ECHO) "" >> $(LIB_INCLUDE_DIR)/flat.h
	@for header in $(notdir $(LIB_INCLUDES)); do $(ECHO) "#include <flat/$$header>"; done >> $(LIB_INCLUDE_DIR)/flat.h
	@$(ECHO) "" >> $(LIB_INCLUDE_DIR)/flat.h
	@$(ECHO) "#endif" >> $(LIB_INCLUDE_DIR)/flat.h

$(TEST): $(LIBRARIES) $(TEST_OBJECTS) $(DEPS)
	$(LD) $(LDFLAGS) $(TEST_OBJECTS) $(LIBS) -o $@

$(LIBRARY): $(LIB_OBJECTS) $(DEPS)
	$(AR) $(ARFLAGS) $@ $(LIB_OBJECTS)

$(OBJDIR)/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJDIR)/%.o: testsrc/%.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

lint:
	python2.7 lint/cpplint.py --linelength=120 --root=flat/src --extensions=cpp,h \
		--filter=-whitespace/tab,-whitespace/indent,-whitespace/parens,-whitespace/braces,-whitespace/comments,-runtime/indentation_namespace,-legal \
		src/*

clean:
	@$(ECHO) Cleaning project
	@$(RM) -f $(TEST) $(LIBRARY) $(TEST_OBJECTS) $(LIB_OBJECTS) $(LIB_UI_OBJECTS) $(LIB_INCLUDES)
