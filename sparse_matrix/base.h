#ifndef BASE_H
#define BASE_H
#include <stdlib.h>

#define container_of(ptr, type, member) \
	({									\
		const typeof(((type *) 0)-> member)  *_mptr = (ptr); \
		(type *)((char*) _mptr - offsetof(type, member));  \
	})

#define new(TYPE, args...) TYPE ## _constructor((TYPE *) malloc(sizeof(TYPE)), ## args)

#define delete(TYPE, ptr)			\
	do {							\
		TYPE ## _destructor(ptr);	\
	} while(0)

#endif
