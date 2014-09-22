ifdef CLANG
CC 			= clang++
else
CC			= g++
endif

CFLAGS		= -c -g -pedantic -Wall -Wpointer-arith -Wcast-qual -std=c++11
LD			= g++
LDFLAGS 	= 
AR			= ar
ARFLAGS		= rcs
RM			= rm
CP			= cp
ECHO		= echo -e
SHELL		= /bin/sh
CTAGS		= ctags

LIBS 				= -lSDL2 -lCppUTest -lCppUTestExt

OBJDIR				= obj
DEPS				= 
LIB_SOURCES 		= $(wildcard src/*.cpp)
LIB_OBJECTS 		= $(addprefix $(OBJDIR)/,$(notdir $(LIB_SOURCES:.cpp=.o)))

LIBRARY 			= lib/libflat-2d.a
LIB_INCLUDE_DIR		= include/flat-2d
LIB_INCLUDE_SOURCES	= $(wildcard src/*.h)
LIB_INCLUDES		= $(addprefix $(LIB_INCLUDE_DIR)/,$(notdir $(LIB_INCLUDE_SOURCES:.cpp=.o)))

default: $(LIBRARY)

all: $(LIBRARY) $(TEST)

$(LIBRARY): $(OBJDIR) $(LIB_OBJECTS) $(DEPS) $(LIB_INCLUDES)
	$(AR) $(ARFLAGS) $@ $(LIB_OBJECTS)

$(LIB_INCLUDE_DIR)/%.h: src/%.h
	@$(CP) $< $@

$(OBJDIR):
	@if [ ! -d "$(OBJDIR)" ]; then mkdir $(OBJDIR); fi

$(OBJDIR)/%.o: src/%.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	@$(ECHO) Cleaning project
	@$(RM) -f $(LIBRARY) $(LIB_OBJECTS) $(LIB_INCLUDES)
