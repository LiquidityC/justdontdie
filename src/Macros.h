#ifndef MACROS_H_
#define	MACROS_H_

#include <cstdio>

#ifdef DEBUG
#define LOG(fmt, ...) printf("[%s:%d] --> ", __FILE__, __LINE__); \
	printf((fmt), __VA_ARGS__);
#endif
#ifndef DEBUG
#define LOG(fmt, ...) do { } while(0);
#endif

#endif // MACROS_H_
