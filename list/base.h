#ifndef BASE_H
#define BASE_H
#include <stdlib.h>

#define SUCCESS 1
#define FAILED  0

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

#define container_of(ptr, type, member) \
	({									\
		const typeof(((type *) 0)-> member)  *_mptr = (ptr); \
		(type *)((char*) _mptr - offsetof(type, member));  \
	})

#define new(TYPE, args...) TYPE ## _constructor(args)

#define delete(TYPE, ptr)			\
	do {							\
		TYPE ## _destructor(ptr);	\
	} while(0)

#endif
