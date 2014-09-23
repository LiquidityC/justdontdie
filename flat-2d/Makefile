ifdef CLANG
CC 			= clang++
else
CC			= g++
endif

CFLAGS		= -c -g -pedantic -Wall -Wpointer-arith -Wcast-qual -std=c++11 \
			  -include src/NewMacro.h
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

DEPS				= $(wildcard src/*.h) $(wildcard uisrc/*.h)
OBJDIR				= obj
LIBDIR				= lib

LIBRARY_MAIN 		= $(LIBDIR)/libflat-2d.a
LIB_MAIN_SOURCES	= $(wildcard src/*.cpp)
LIB_MAIN_OBJECTS	= $(addprefix $(OBJDIR)/,$(notdir $(LIB_MAIN_SOURCES:.cpp=.o)))

LIBRARY_UI 			= $(LIBDIR)/libflat-2d-ui.a
LIB_UI_SOURCES 		= $(wildcard uisrc/*.cpp)
LIB_UI_OBJECTS		= $(addprefix $(OBJDIR)/,$(notdir $(LIB_UI_SOURCES:.cpp=.o)))

LIB_INCLUDE_DIR		= include/flat-2d
LIB_INCLUDE_SOURCES	= $(wildcard src/*.h) $(wildcard uisrc/*.h)
LIB_INCLUDES		= $(addprefix $(LIB_INCLUDE_DIR)/,$(notdir $(LIB_INCLUDE_SOURCES)))

TEST				= test
TEST_SOURCES 		= $(wildcard src/*.cpp) $(wildcard testsrc/*.cpp) $(wildcard uisrc/*.cpp)
TEST_OBJECTS 		= $(addprefix $(OBJDIR)/,$(notdir $(TEST_SOURCES:.cpp=.o)))

.PHONY: $(OBJDIR) $(LIBDIR) $(LIB_INCLUDE_DIR)

default: $(LIBRARY_MAIN) $(LIBRARY_UI) $(LIB_INCLUDES)

dirs: $(OBJDIR) $(LIB_INCLUDE_DIR) $(LIBDIR)

all: $(LIBRARY_MAIN) $(LIBRARY_UI) $(TEST)

check: $(TEST)
	./$(TEST) -v -c

$(LIB_INCLUDE_DIR)/%.h: src/%.h
	@$(CP) $< $@

$(LIB_INCLUDE_DIR)/%.h: uisrc/%.h
	@$(CP) $< $@

$(TEST): $(LIBRARIES) $(TEST_OBJECTS) $(DEPS)
	$(LD) $(LDFLAGS) $(TEST_OBJECTS) $(LIBS) -o $@

$(LIBRARY_MAIN): $(LIB_MAIN_OBJECTS) $(DEPS)
	$(AR) $(ARFLAGS) $@ $(LIB_MAIN_OBJECTS)

$(LIBRARY_UI): $(LIB_UI_OBJECTS) $(DEPS)
	$(AR) $(ARFLAGS) $@ $(LIB_UI_OBJECTS)

$(OBJDIR):
	@if [ ! -d "$(OBJDIR)" ]; then mkdir $(OBJDIR); fi

$(LIBDIR):
	@if [ ! -d "$(LIBDIR)" ]; then mkdir $(LIBDIR); fi

$(LIB_INCLUDE_DIR):
	@if [ ! -d "$(LIB_INCLUDE_DIR)" ]; then mkdir -p $(LIB_INCLUDE_DIR); fi

$(OBJDIR)/%.o: src/%.cpp
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: uisrc/%.cpp
	$(CC) $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: testsrc/%.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	@$(ECHO) Cleaning project
	@$(RM) -f $(LIBRARY_MAIN) $(LIBRARY_UI) $(LIB_MAIN_OBJECTS) $(LIB_UI_OBJECTS) $(LIB_INCLUDES)
