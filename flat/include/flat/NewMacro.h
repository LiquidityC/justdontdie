#ifndef _NEW_MACRO_H
#define _NEW_MACRO_H

/**
 * This is a workaround for the memory leak detection in
 * cpputest conflicting with stl and boost libs.
 *
 * Documented here:
 * http://cpputest.github.io/manual.html#memory_leak_detection
 */

#include <map>
#include <boost/lexical_cast.hpp>

// Include this last or everything breaks
#include <CppUTest/MemoryLeakDetectorNewMacros.h>

#endif
